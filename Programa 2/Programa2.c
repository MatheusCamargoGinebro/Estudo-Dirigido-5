#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

/*
O=======================================================================O
| Estudo dirigido 5 - Programa 2 - Sistema de login com Lista encadeada |
|    Nomes:                                                             |
|        Matheus Camargo Ginebro CP3016153                              |
|        Lucas Haiter Leoni CP3017362                                   |
|     Por favor, contar este programa como o que vai valer a nota do    |
|     Estudo Dirigido.                                                  |
O=======================================================================O
*/
/*------------------------------< Implementação do HASH com SHA256 >------------------------------*/
//Código pego em <https://www.programmingalgorithms.com/algorithm/sha256/c/>.

/* Perdão professor, mas nós não conseguiriamos fazer isso por nós mesmos 
(bolar todo essas funções de hash). portanto, para não deixar a atividade com partes faltando, pegamos
o HASH de um site. O link está logo acima.*/

#define uchar unsigned char
#define uint unsigned int
#define DBL_INT_ADD(a,b,c) if (a > 0xffffffff - (c)) ++b; a += c;
#define ROTLEFT(a,b) (((a) << (b)) | ((a) >> (32-(b))))
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))
#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
#define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))
typedef struct {
	uchar data[64];
	uint datalen;
	uint bitlen[2];
	uint state[8];
} SHA256_CTX;
uint k[64] = {
	0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
	0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
	0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
	0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
	0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
	0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
	0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
	0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};
void SHA256Transform(SHA256_CTX *ctx, uchar data[]){
	uint a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];
	for (i = 0, j = 0; i < 16; ++i, j += 4)
		m[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | (data[j + 3]);
	for (; i < 64; ++i)
		m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];
	a = ctx->state[0];
	b = ctx->state[1];
	c = ctx->state[2];
	d = ctx->state[3];
	e = ctx->state[4];
	f = ctx->state[5];
	g = ctx->state[6];
	h = ctx->state[7];
	for (i = 0; i < 64; ++i) {
		t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
		t2 = EP0(a) + MAJ(a, b, c);
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}
	ctx->state[0] += a;
	ctx->state[1] += b;
	ctx->state[2] += c;
	ctx->state[3] += d;
	ctx->state[4] += e;
	ctx->state[5] += f;
	ctx->state[6] += g;
	ctx->state[7] += h;
}
void SHA256Init(SHA256_CTX *ctx)
{
	ctx->datalen = 0;
	ctx->bitlen[0] = 0;
	ctx->bitlen[1] = 0;
	ctx->state[0] = 0x6a09e667;
	ctx->state[1] = 0xbb67ae85;
	ctx->state[2] = 0x3c6ef372;
	ctx->state[3] = 0xa54ff53a;
	ctx->state[4] = 0x510e527f;
	ctx->state[5] = 0x9b05688c;
	ctx->state[6] = 0x1f83d9ab;
	ctx->state[7] = 0x5be0cd19;
}
void SHA256Update(SHA256_CTX *ctx, uchar data[], uint len)
{
	for (uint i = 0; i < len; ++i) {
		ctx->data[ctx->datalen] = data[i];
		ctx->datalen++;
		if (ctx->datalen == 64) {
			SHA256Transform(ctx, ctx->data);
			DBL_INT_ADD(ctx->bitlen[0], ctx->bitlen[1], 512);
			ctx->datalen = 0;
		}
	}
}
void SHA256Final(SHA256_CTX *ctx, uchar hash[])
{
	uint i = ctx->datalen;

	if (ctx->datalen < 56) {
		ctx->data[i++] = 0x80;
		while (i < 56)
			ctx->data[i++] = 0x00;
	}
	else {
		ctx->data[i++] = 0x80;
		while (i < 64)
			ctx->data[i++] = 0x00;
		SHA256Transform(ctx, ctx->data);
		memset(ctx->data, 0, 56);
	}
	DBL_INT_ADD(ctx->bitlen[0], ctx->bitlen[1], ctx->datalen * 8);
	ctx->data[63] = ctx->bitlen[0];
	ctx->data[62] = ctx->bitlen[0] >> 8;
	ctx->data[61] = ctx->bitlen[0] >> 16;
	ctx->data[60] = ctx->bitlen[0] >> 24;
	ctx->data[59] = ctx->bitlen[1];
	ctx->data[58] = ctx->bitlen[1] >> 8;
	ctx->data[57] = ctx->bitlen[1] >> 16;
	ctx->data[56] = ctx->bitlen[1] >> 24;
	SHA256Transform(ctx, ctx->data);
	for (i = 0; i < 4; ++i) {
		hash[i] = (ctx->state[0] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 4] = (ctx->state[1] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 8] = (ctx->state[2] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 12] = (ctx->state[3] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 16] = (ctx->state[4] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 20] = (ctx->state[5] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 24] = (ctx->state[6] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 28] = (ctx->state[7] >> (24 - i * 8)) & 0x000000ff;
	}
}
char* H(char* data) {
	int strLen = strlen(data);
	SHA256_CTX ctx;
	unsigned char hash[32];
	char* hashStr = malloc(65);
	strcpy(hashStr, "");
	SHA256Init(&ctx);
	SHA256Update(&ctx, data, strLen);
	SHA256Final(&ctx, hash);
	char s[3];
	for (int i = 0; i < 32; i++) {
		sprintf(s, "%02x", hash[i]);
		strcat(hashStr, s);
	}
	return hashStr;
}
/*---------------------------< ^ Fim das funções de HASH/SHA256 ^ >---------------------------*/

/*------------------------------< Structs >------------------------------*/

//Estrutura de endereço.
typedef struct{
    char rua[128];
    char bairro[128];
    int numero;
    int cep;
} address;

//Estrutura que guarda os dados do usuário (incluindo uma estrutura de endereço).
typedef struct{
    // Dados obrigatórios da atividade
    int id;
    char nome[128];
    char usuario[64];
    char senha[65]; // Se fosse 50, como pedido no enunciado, ou até mesmo 64 não haveria espaço para por toda a chave e mais o \0.
    char tipo;
    address endereco;
} user;

// Estrutura de nó (node).
typedef struct NODE{
    // Estrutura do usuário
    user UserNode;

    // variável para verificar se o nó está ou não no arquivo criado pelo programa.
    int isOnFile;

    // Ponteiro que aponta para o endereço do próximo nó.
    struct NODE *proximo;
} node;

// Estrutura de lista.
typedef struct{
    node *inicio;
    node *fim;
    int cont;
} list;



/*------------------------------< Functions >------------------------------*/
/*-------------< Funções de exibição >-------------*/
// Função de limpar a tela (é um system("cls"); só que aprimorado).
void proxTela(){
    printf("\n\n");
    system("pause");
    system("cls");
}

// Imprime um único node.
void printuser(node *userToPrint){
    printf("Usuário: %s", userToPrint->UserNode.usuario);
    printf("Nome: %s", userToPrint->UserNode.nome);
    printf("Tipo: %c\n", userToPrint->UserNode.tipo);
    printf("Id: %d\n", userToPrint->UserNode.id);
    printf("Rua: %s", userToPrint->UserNode.endereco.rua);
    printf("Bairro: %s", userToPrint->UserNode.endereco.bairro);
    printf("Número: %d\n", userToPrint->UserNode.endereco.numero);
    printf("CEP: %d\n", userToPrint->UserNode.endereco.cep);
    printf("Salvo no arquivo == %d.\n\n", userToPrint->isOnFile);
}

// Imprime uma lista inteira.
void printList(list *lista){
    for(node *I = lista->inicio; I!=NULL; I = I->proximo){
        printuser(I);
    }
}

/*-------------< Funções de modificação de lista/arquivo >-------------*/
// Função para Criar a lista.
list *CreateList(){
    // Declara uma lista vazia dinâmicamente.
    list *lista = (list *)malloc(sizeof(list));

    // Verifica se a lista foi realmente criada.
    if (lista == NULL){ // Caso haja erro, filaizará o programa.
        printf("\nErro ao alocar memória para a lista.\n");
        exit(1);

    }else{ // Caso não haja erro, o programa inicializa os elementos da lista como NULL (indica lista vazia).
        lista->inicio = NULL;
        lista->fim = NULL;
        lista->cont = 0;

        return lista;
    }
}

// Função para Limpar a lista.
void clearList(list *Lista){
    node *user;

    // Enquanto o inicio da lista apontar para um nó.
    while(Lista->inicio != NULL){
        // Recupera o primeiro elemento da lista.
        user = Lista->inicio;

        // Faz com que o próximo elemento seja o primeiro elemento da lista.
        Lista->inicio = user->proximo;

        // Remove o nodo da lista.
        free(user);
    }
    // Libera a memória da variável que representa a lista.
    free(Lista);
}

// Função para gerar um Id para um novo usuário.
int idGenerator(list *lista){
    // Declara uma variável de verificação e um cont.
    int verif, cont=0;
    // A função testa para achar um valor que seja igual à algum Id.
    do{
        verif=0;
        for(node *I = lista->inicio; I!=NULL; I = I->proximo){
            if(cont == I->UserNode.id){
                verif=1;
                break;
            }
        }
        if (verif==1){ // Quando a função encontra um Id igual ao cont, o cont é incrementado.
            cont++;
        }
    }while(verif!=0); // Enquanto a função não encontrar um valor para cont, que seja diferente de todos os IDs, ela não para.

    return cont; // Quando a função acha um valor para o novo Id, esse valor é retornado par a main.
}

// Função para adicionar um usuário à lista. *(também serve para carregar usuários de um arquivo, trabalhando em conjunto com a função loadToMemo)
void addUser(list *lista, node *usuarioNode){
    //cria o node da lista dinâmincamente.
    node *NewUser = (node*)malloc(sizeof(node));

    //Verifica se o nó foi criado com êxito.
    if(NewUser == NULL){
        printf("\nErro ao adicionar novo usuário.\n");
        proxTela();
        exit(1);  //finaliza o programa retornando código de erro 1.
    }else{
        // Inserindo elemento no inicio da lista.
        NewUser = usuarioNode;
        NewUser->proximo = lista->inicio;

        // Verificando se a lista está vazia.
        if(lista->inicio == NULL){ // Caso esteja vazia, o primeiro elemento também será o último.
            lista->fim = NewUser;
        }
        lista->inicio = NewUser;
        lista->cont++;
    }
}

// Função para remover da lista. (e dependendo do caso, do arquivo também).
int removeUser(list *Lista, FILE *fileToEdit, user *Removedor, int Id){
    node *anterior;
    for(node *I = Lista->inicio; I!=NULL; anterior = I, I = I->proximo){ // Não há necessidade de testar se a lista está vazia, já que sempre haverá ao menos um elemento dentro dela.
            if(Id == I->UserNode.id){ // Achou um usuário com o ID parecido.
                if(I->UserNode.tipo=='S'){ // Se esse usuário for o SuperUsuário, a função retorna -1, não permitindo a remoção.
                    return -1;
                }else{
                    if(Removedor->tipo=='S' || (Removedor->tipo == 'A' && I->UserNode.tipo == 'C')){ // Indica permissão para remover.
                        if(I == Lista->inicio){ // Se o elemento está no início da lista.
                        // O segundo elemento passa a ser o início da lista.
                        Lista->inicio = I->proximo;

                        }else{// O Elemento está no meio da lista.
                            // O proximo do nó anterior passa a ser o proximo do nó encontrado;
                            anterior->proximo = I->proximo;
                        }

                        // Testando se o elemento está salvo no arquivo.
                        if (I->isOnFile==1){
                            node *uRemFil = (node*)malloc(sizeof(node));

                            fileToEdit = fopen("userDataBase.bin", "r+b");
    
                            while (1){
                                int posicao = ftell(fileToEdit);
                                fread(uRemFil, sizeof(node), 1, fileToEdit);
                                if(feof(fileToEdit)){
                                    break;
                                }
                                if(I->UserNode.id == uRemFil->UserNode.id){ 
                                    uRemFil->isOnFile = -1; // O item IsOnfile define se o arquivo está ou não no arquivo.
                                    /*  Caso ele esteja, seu valor é 1. ------ Quando removido, seu valor passa a ser -1, indicando
                                     que o espaço que ele ocupa pode ser reaproveitado.
                                        Foi feito desta maneira, pois aparentemente você não pode simplesmente "remover" 
                                    algo da memória, mas pode sobrescrever."*/

                                    fseek(fileToEdit, posicao, SEEK_SET);
                                    fwrite(uRemFil, sizeof(node), 1, fileToEdit);
                                    break;
                                }
                            }
                            fclose(fileToEdit);
                        }

                        //libera a memória ocupada pelo nó.
                        free(I);
                        Lista->cont--;
                        //retorna 1 (true) indicando que encontrou e removeu o elemento da lista
                        return 1;
                    }else if(Removedor->tipo=='A' && I->UserNode.tipo == 'A'){ // Não tem permissão para remover.
                        return -2;
                    }
                }
            }
        }
    // Retorna 2 (false) indicando que não foi possível encontrar o usuário.
    return 0;
}
/*Gabarito do return:
    return == -1 == O Id era o do superusuário, ele não pode ser removido.
    return == -2 == não havia permissão para remover o usuário.
    return == 0 == não foi possível encontrar o usuário.
    return == 1 == usuário removido.
*/


// Função para carregar os dados de um arquivo para a memória.
int loadListToMemo(list *lista, FILE *fileToLoad){
    while(1){
        node *getUser = (node*)malloc(sizeof(node));
        fread(getUser, sizeof(node), 1, fileToLoad);

        if(feof(fileToLoad)){
            break;
        }
        if(getUser->isOnFile!=-1){
            
            addUser(lista, getUser);
        }
    }
    if (lista->inicio!=NULL){
        return 0; // Carregado com sucesso.
    }else{
        return 1; // Deu algum problema.
    }
}


// Função para salvar um usuário no arquivo.
void addUserToFile(FILE *fileToAdd, node *NewUser){
    fileToAdd = fopen("userDataBase.bin", "r+b");
    while(1){
        node *getUser = (node*)malloc(sizeof(node));
        int posicao = ftell(fileToAdd);
        fread(getUser, sizeof(node), 1, fileToAdd);

        if(feof(fileToAdd) || getUser->isOnFile==-1){
            if(getUser->isOnFile==-1){
                fseek(fileToAdd, posicao, SEEK_SET);
            }

            fwrite(NewUser, sizeof(node), 1, fileToAdd);
            break;
        }
    }
    fclose(fileToAdd);
}



/*-------------< Funções de verificação e etc >-------------*/
// Função para verificar os parâmetros de senha e de node de usuário. (dependendo da situação, pode-se testar apenas um dos itens (se quiser)).
int verifLogin(list *Lista, user *atualUser){
    int verif = -1;
    for(node *I = Lista->inicio; I!=NULL; I = I->proximo){
        if(strcmp(atualUser->usuario, I->UserNode.usuario)==0){
            verif = 1; // Usuário verificado
            if(strcmp(atualUser->senha, I->UserNode.senha)==0){
                verif = 0; // Senha verificada
                *atualUser = I->UserNode;
            } 
        }
    }
    return verif; 
}
/*  
    Gabarito do return (verif):
    return 1 == Usuário encontrado, senha inválida.
    return 0 == usuário e senha corretos.
    return -1 == não foi encontrado usuário.
*/


// Função para procurar um usuário pelo nome.
int searchForName(list *Lista, char nameToCheck[]){
    int verif = 0;
    for(node *I = Lista->inicio; I!=NULL; I= I->proximo){
        if (strcmp(I->UserNode.nome, nameToCheck)==0){
            printuser(I);
            verif = 1;
        }
    }
    return verif;
}


// Função para trocar a senha.
int changePassword(list *Lista, FILE *fileToEdit, user *atualUser, char newPassword[]){
    int verif=0;
    for(node *I = Lista->inicio; I!=NULL; I= I->proximo){
        if (strcmp(atualUser->usuario, I->UserNode.usuario)==0){
            strcpy(I->UserNode.senha, newPassword);
            strcpy(atualUser->senha, newPassword);

            if(I->isOnFile==1){
                fileToEdit = fopen("userDataBase.bin", "r+b");
                while(1){
                    int posicao = ftell(fileToEdit);
                    node *userToEdit = (node*)malloc(sizeof(node));
                    fread(userToEdit, sizeof(node), 1, fileToEdit);

                    if(feof(fileToEdit)){
                        break;
                    }
                    if(strcmp(atualUser->usuario, I->UserNode.usuario)==0){
                        strcpy(userToEdit->UserNode.senha, newPassword);
                        
                        fseek(fileToEdit, posicao, SEEK_SET);
                        fwrite(userToEdit, sizeof(node), 1, fileToEdit);
                        
                        break;
                    }
                }
                fclose(fileToEdit);
            }
            verif=1;
            break;
        }
    }
    return verif;
}




/*------------------------------< Main >------------------------------*/
int main(){
    setlocale(LC_ALL, "portuguese");

    //Iniciando arquvo e lista.
    list *Lista = CreateList();

    FILE *usersDataBase = fopen("userDataBase.bin", "rb");
    
    if(usersDataBase == NULL){ // Não foi possível abrir o arquivo. (tentará criar outro arquivo).
        fclose(usersDataBase);
        usersDataBase = fopen("userDataBase.bin", "wb");
        
        // Salva a lista.
        fclose(usersDataBase);
        if(usersDataBase!=NULL){// Arquivo criado com sucesso.
            node *superUser = (node*)malloc(sizeof(node));
            printf("O==================================================================================O\n| A lista foi recém criada, sendo assim será necessário cadastrar o *SUPERUSUÁRIO* |\nO==================================================================================O");
            proxTela();

            // Dar Id ao *SUPERUSUÁRIO*.
            superUser->UserNode.id = idGenerator(Lista);
            printf("O Id do *SUPERUSUÁRIO* é %d.\n", superUser->UserNode.id);

            // Dar Nome ao *SUPERUSUÁRIO*.
            printf("\nInsira o nome do *SUPERUSUÁRIO*.\nR: ");
            setbuf(stdin, NULL);
            fgets(superUser->UserNode.nome, 128, stdin);

            // PEGAR ENDEREÇO DO *SUPERUSUÁRIO*:
            // Rua do *SUPERUSUÁRIO*
            printf("\nInsira a rua do *SUPERUSUÁRIO*.\nR: ");
            setbuf(stdin, NULL);
            fgets(superUser->UserNode.endereco.rua, 128, stdin);

            // Bairro do *SUPERSUÁRIO*
            printf("\nInsira o bairro do *SUPERUSUÁRIO*.\nR: ");
            setbuf(stdin, NULL);
            fgets(superUser->UserNode.endereco.bairro, 128, stdin);

            // Número da casa do *SUPERSUÁRIO*
            printf("\nInsira o número da casa do *SUPERSUÁRIO*.\nR: ");
            scanf("%d", &superUser->UserNode.endereco.numero);

            // CEP do *SUPERSUÁRIO*
            printf("\nInsira o CEP do *SUPERSUÁRIO*.\nR: ");
            scanf("%d", &superUser->UserNode.endereco.cep);
            
            // Dar o parâmetro de login do *SUPERUSUÁRIO*.
            printf("\nInsira o Nick do *SUPERUSUÁRIO* (parâmetro para fazer o login).\nR: ");
            setbuf(stdin, NULL);
            fgets(superUser->UserNode.usuario, 64, stdin);

            // Dar a senha (criptografada) para o login do *SUPERUSUÁRIO*.
             // Definindo uma variável temporária para pegar a senha.
            char *TempPassW = (char*)malloc(64*sizeof(char));
            //Pegando a senha
            printf("\nInsira a Senha do *SUPERUSUÁRIO* (parâmetro para fazer o login).\nR: ");
            setbuf(stdin, NULL);
            fgets(TempPassW, 64, stdin);

            // Fazendo o hashing da senha.
            TempPassW = H(TempPassW);

            // Passando a senha (com o HASH já executado) para o node.
            strcpy(superUser->UserNode.senha, TempPassW);

            /* Limpando a variável temporária para garantir mais segurança. 
            (não somos peritos em segurança da computação/hacking, então não sabemos se realmente 
            funcionaria isso... Mas tá aí, por desencargo de conciência).*/
            free(TempPassW);

            //Definir o tipo de usuário (no caso, é fixo em *SUPERUSUÁRIO*).
            printf("\nO *SUPERUSUÁRIO* recebe o tipo S (*SUPERUSUÁRIO*).");
            superUser->UserNode.tipo = 'S';

            // Definindo que o *SUPERUSUÁRIO* estará no arquivo.
            superUser->isOnFile = 1;
    
            proxTela();
            // Insere o *SUPERUSUÁRIO* na lista.
            addUserToFile(usersDataBase,superUser);
            // Libera a memória usada para guardar os dados do *SUPERUSUÁRIO*.
            free(superUser);
            // Tenta novamente abrir o arquivo em modo leitura, para que seja possível carregar o superusuário na lista.
            usersDataBase = fopen("userDataBase.bin", "rb");

            if(usersDataBase == NULL){
                printf("O Super usuário foi salvo, porém não foi possível abrir o arquivo em modo leitura.\n");
                proxTela();
                exit(1);
            }
        }else{
            printf("\nERRO: não foi possível criar/carregar o arquivo. adios.\n");
            proxTela();
            exit(1);
        }
    }

    // Carregando dados dos usuários para a lista.
    if(loadListToMemo(Lista, usersDataBase)!=0){
        printf("ERROR: Não foi possível carregar os nós do arquivo para a memória.O programa será finalizado.\n");
        proxTela();
        exit(1);
    }else{
        printf("Os nós salvos no arquivo foram carregados com sucesso!\n");
        fclose(usersDataBase); // Fechando/salvando o arquivo após carregá-lo para a memória.
        proxTela();

        int rLog; // Variável de resposta para login.

        do
        {
            printf("O=============================O\n| Sistema de Login e arquivos |\nO=============================O\n|     O que deseja fazer?     |\n|          [1] Login.         |\n|          [2] Sair.          |\nO=============================O\n\nR: ");
            scanf("%d", &rLog);

            switch (rLog){
                case 1:
                    system("cls");
                    // Reservando um espaço na memória para o usuário atual, de forma dinâmica.
                    user *atualUser = (user*)malloc(1*sizeof(user));
                    int verif;

                    
                    printf("O===========================O\n| Você escolheu fazer login |\nO===========================O\n\n");
                
                    // Pegando o nome do usuário.
                    printf("Digite seu nome de usuário.\nR: ");
                    setbuf(stdin, NULL);
                    fgets(atualUser->usuario, 64, stdin);

                    // Pegando a senha do usuário.
                     // Definindo uma string temporária para a pegar a senha do usuário.
                    char *TempPassW = (char*)malloc(64*sizeof(char));

                    printf("\nDigite sua senha.\nR: ");
                    setbuf(stdin, NULL);
                    fgets(TempPassW, 64, stdin);

                    // Fazendo o HASH dessa senha.
                    TempPassW = H(TempPassW);

                    // Passando a senha com HASH para a estrutura que ajuda na verificação de login.
                    strcpy(atualUser->senha, TempPassW);

                    //Limpando a string temporária.
                    free(TempPassW);

                    // Passando o resultado da função para uma variável de verificação.
                    verif = verifLogin(Lista, atualUser);
                    
                    if(verif == 0){
                        printf("\nUsuário e senha corretos.\n\nlogado com sucesso!\n\nVocê logou como %s", atualUser->usuario);
                        proxTela();
                        int rOpc;
                            
                        switch (atualUser->tipo)
                        {
                            case 'C': // Caso o usuário seja um cliente.
                            do{
                                system("cls");
                                printf("O======================================O\t\t\tInformações do usuário logado:\n|     Sistema de usuários - Logado     |\t\t\tNome: %sO======================================O\t\t\tUsuário: %s|    [1] Alterar senha.                |\t\t\tTipo: %c\n|    [2] Sair.                         |\nO======================================O\n\nR: ", atualUser->nome, atualUser->usuario, atualUser->tipo);
                                scanf("%d", &rOpc);

                                switch (rOpc){
                                    case 1:
                                        system("cls");
                                        printf("O====================================O\n| Você escolheu [1] alretar a senha. |\nO====================================O\n");
                                        
                                        //Declarando uma string temporária para pegar a nova senha digitada pelo usuário.
                                        char *TempNewPassW = (char*)malloc(64*sizeof(char));

                                        // Pegando a nova senha
                                        printf("\nDigite a sua nova senha.\nR: ");
                                        setbuf(stdin, NULL);
                                        fgets(TempNewPassW, 64, stdin);

                                        // Fazendo o HASH dessa senha.
                                        TempNewPassW = H(TempNewPassW);

                                        // Passando a senha com HASH para a estrutura que ajuda na verificação de login.
                                        strcpy(atualUser->senha, TempNewPassW);

                                        if(changePassword(Lista, usersDataBase, atualUser, TempNewPassW)==0){
                                            printf("\nNão foi possível alterar a senha.");
                                        }else{
                                            printf("\nSenha alterada com sucesso.");
                                        }
                                        free(TempNewPassW);
                                        proxTela();
                                    break;
                                    
                                    case 2:
                                        system("cls");
                                        printf("O=============================O\n| Você escolheu [2] deslogar. |\nO=============================O");
                                        
                                        proxTela();
                                    break;

                                    case 598752185463186196751981535249151: // Debug.
                                        system("cls");
                                        printf("Os usuários que estão na lista:\t\t\tQuantidade de usuários: %d\n\n", Lista->cont);
                                        
                                        printList(Lista);
                                        proxTela();
                                    break;
                                    default:
                                        printf("Opção inválida. escolha apenas entre 1 e 2");
                                        proxTela();
                                    break;
                                }
                            }while(rOpc!=2);
                            break;
                    
                            default: // Caso o usuário seja o *SUPERUSUÁRIO* ou um Administrador.
                            do
                            {
                                system("cls");
                                printf("O======================================O\t\t\tInformações do usuário logado:\n|     Sistema de usuários - Logado     |\t\t\tNome: %sO======================================O\t\t\tUsuário: %s|    [1] Cadastrar usuário.            |\t\t\tTipo: %c\n|    [2] Remover usuário.              |\n|    [3] Pesquisar usuário por nome.   |\n|    [4] Alterar senha.                |\n|    [5] Sair.                         |\nO======================================O\n\nR: ", atualUser->nome, atualUser->usuario, atualUser->tipo);
                                scanf("%d", &rOpc);

                                switch (rOpc){
                                    case 1:
                                        system("cls");
                                        printf("O======================================O\n| Você escolheu [1] Cadastrar usuário. |\nO======================================O\n");

                                        node *novoUsuario = (node*)malloc(sizeof(node));
                                        int verif2;
                                        
                                        // pegar ID do novo usuário.
                                        novoUsuario->UserNode.id = idGenerator(Lista);

                                        // Pegar nome do novo usuário.
                                        printf("\nDigite o nome do usuário que você quer adicionar.\nR: ");
                                        setbuf(stdin, NULL);
                                        fgets(novoUsuario->UserNode.nome, 128, stdin);

                                        // Pegar dados de ENDEREÇO.
                                        // Rua do novo usuário.
                                        printf("\nDigite a rua do usuário que você quer adicionar.\nR: ");
                                        setbuf(stdin, NULL);
                                        fgets(novoUsuario->UserNode.endereco.rua, 128, stdin);

                                        // Bairro do novo usuário.
                                        printf("\nDigite o bairro no usuário que você quer adicionar.\nR: ");
                                        setbuf(stdin, NULL);
                                        fgets(novoUsuario->UserNode.endereco.bairro, 128, stdin);

                                        // Número do novo usuário.
                                        printf("\nDigite o número do usuário que você quer adicionar.\nR: ");
                                        scanf("%d", &novoUsuario->UserNode.endereco.numero);

                                        // Cep do novo usuário.
                                        printf("\nDigite o CEP do usuário que você quer adicionar.\nR: ");
                                        scanf("%d", &novoUsuario->UserNode.endereco.cep);

                                        // Nick de usuário.
                                        do{
                                            printf("\nDigite o nick do usuário (parâmetro de login).\nR: ");
                                            setbuf(stdin, NULL);
                                            fgets(novoUsuario->UserNode.usuario, 64, stdin);

                                            verif2 = verifLogin(Lista, novoUsuario);
                                            if(verif2 == 1 || verif2 == 0){
                                                printf("\nUsuário já cadastrado. tente outro nick.\n");
                                            }
                                        }while(verif2!=-1);
                                        
                                        // Senha do usuário.
                                        // String de senha temporária.
                                        char *TempPassW = (char*)malloc(64*sizeof(char));
                                        printf("\nDigite a senha do usuário que você quer adicionar.\nR: ");
                                        setbuf(stdin, NULL);
                                        fgets(TempPassW, 64, stdin);

                                        // Fazendo o HASH da senha.
                                        TempPassW = H(TempPassW);

                                        // Passando a senha já criptografada para o novo node.
                                        strcpy(novoUsuario->UserNode.senha, TempPassW);

                                        // Limpando a String temporária.
                                        free(TempPassW);

                                        // Tipo de usuário.
                                        do{
                                            printf("\nDigite o tipo de usuário.\nR: ");
                                            setbuf(stdin, NULL);
                                            scanf("%c", &novoUsuario->UserNode.tipo);

                                            if(atualUser->tipo == 'S'){
                                                if(novoUsuario->UserNode.tipo == 'A' || novoUsuario->UserNode.tipo == 'a'){
                                                    printf("\nUsuário será do tipo Administrador.\n");
                                                    verif2 = 0;
                                                }else if(novoUsuario->UserNode.tipo == 'C' || novoUsuario->UserNode.tipo == 'c'){
                                                    printf("\nUsuário será do tipo Cliente.\n");
                                                    verif2 = 0;
                                                }else if(novoUsuario->UserNode.tipo == 'S' || novoUsuario->UserNode.tipo == 's'){
                                                    printf("\nEste programa é pequeno demais para 2 *SUPERUSUÁRIOS*, parceiro. opção inválida.\n");
                                                    verif2 = 1;
                                                }else{
                                                    printf("\nOpção inválida. escolha [A/C].\n");
                                                    verif2 = 1;
                                                }
                                            }else if(atualUser->tipo == 'A'){
                                                if(novoUsuario->UserNode.tipo == 'A' || novoUsuario->UserNode.tipo == 'a'){
                                                    printf("\nVocê não tem permissão para adicionar um Administrador.\n");
                                                    verif2 = 1;
                                                }else if(novoUsuario->UserNode.tipo == 'C' || novoUsuario->UserNode.tipo == 'c'){
                                                    printf("\nUsuário será do tipo Cliente.\n");
                                                    verif2 = 0;
                                                }else if(novoUsuario->UserNode.tipo == 'S' || novoUsuario->UserNode.tipo == 's'){
                                                    printf("\nVocê não tem permissão para adicionar um Superusuário.\n");
                                                    verif2 = 1;
                                                }else{
                                                    printf("\nOpção inválida. você só pode escolher [C].\n");
                                                    verif2 = 1;
                                                }
                                            }
                                        } while(verif2);

                                        // Saber se o usuário estará no arquivo.
                                        do{
                                            char r;
                                            printf("\nDeseja salvar o novo usuário no arquivo? [S/N].\nR: ");
                                            setbuf(stdin, NULL);
                                            scanf("%c", &r);
                                            if(r=='s' || r=='S'){
                                                printf("\nO usuário será salvo no arquivo.");
                                                novoUsuario->isOnFile = 1;
                                                addUserToFile(usersDataBase, novoUsuario);
                                            }else if(r=='n' || r=='N'){
                                                printf("\nO usuário não será salvo no arquivo.");
                                                novoUsuario->isOnFile = 0;
                                            }else{
                                                printf("\nOpção inválida. escolha [S/N].\n");
                                                novoUsuario->isOnFile = -1;
                                            }
                                        }while(novoUsuario->isOnFile == -1);
                                        addUser(Lista, novoUsuario);
                                        proxTela();
                                    break;

                                    case 2:
                                        system("cls");
                                        printf("O=======================================O\n| Você escolheu [2] Remover um usuário. |\nO=======================================O\n");
                                        int Id, check;

                                        printf("Digite a Id do usuário que você quer remover.\nR: ");
                                        scanf("%d", &Id);
                                        
                                        check = removeUser(Lista, usersDataBase, atualUser, Id);
                                        if(check == -2){
                                            printf("\nVocê não tem permissão para remover este usuário.");
                                        }else if(check == -1){
                                            printf("\nNão é possível remover o SuperUsuário.");
                                        }else if(check==0){
                                            printf("\nNão foi possível encontrar o usuário.");
                                        }else{
                                            printf("\nUsuáruio removido com sucesso.");
                                        }

                                        proxTela();
                                    break;

                                    case 3:
                                        system("cls");
                                        printf("O===============================================O\n| Você escolheu [3] Pesquisar usuário por nome. |\nO===============================================O\n");
                                        char rNameUser[128];

                                        // Pegar o nome a ser pesquisado.
                                        printf("Digite o nome do usuário que você quer pesquisar.\nR: ");
                                        setbuf(stdin, NULL);
                                        fgets(rNameUser, 128, stdin);
                                        printf("\n");

                                        if(searchForName(Lista, rNameUser)==0){
                                            printf("Não foi possível encontrar o usuário.\n");
                                        }else{
                                            printf("Usuário(s) encontrado(s) com sucesso.\n");
                                        }
                                         // OBS: não ficou totalmente claro no enunciado se é para pesquisar o nome do usuário (Lista->inicio->UserNode.nome) ou o nome de usuário (Lista->inicio->UserNode.usuario), sendo assim, foi pesquisado o parâmetro nome (Lista->inicio->UserNode.nome).
                                        
                                        proxTela();
                                    break;

                                    case 4:
                                        system("cls");
                                        printf("O==================================O\n| Você escolheu [4] Alterar senha. |\nO==================================O\n");
                                        //Declarando uma string temporária para pegar a nova senha digitada pelo usuário.
                                        char *TempNewPassW = (char*)malloc(64*sizeof(char));

                                        // Pegando a nova senha
                                        printf("\nDigite a sua nova senha.\nR: ");
                                        setbuf(stdin, NULL);
                                        fgets(TempNewPassW, 64, stdin);

                                        // Fazendo o HASH dessa senha.
                                        TempNewPassW = H(TempNewPassW);

                                        // Passando a senha com HASH para a estrutura que ajuda na verificação de login.
                                        strcpy(atualUser->senha, TempNewPassW);

                                        if(changePassword(Lista, usersDataBase, atualUser, TempNewPassW)==0){
                                            printf("\nNão foi possível alterar a senha.");
                                        }else{
                                            printf("\nSenha alterada com sucesso.");
                                        }
                                        // Limpando a string temporária.
                                        free(TempNewPassW);
                                        proxTela();
                                    break;
                                    
                                    case 5:
                                        system("cls");
                                        printf("O=============================O\n| Você escolheu [5] deslogar. |\nO=============================O");

                                        proxTela();
                                    break;

                                    case 598752185463186196751981535249151: // Debug.
                                        system("cls");
                                        printf("Os usuários que estão na lista:\t\t\tQuantidade de usuários: %d\n\n", Lista->cont);
                                        
                                        printList(Lista);
                                        proxTela();
                                    break;
                                    default:
                                        printf("\nOpção inválida. escolha apenas entre 1 e 5.");
                                        proxTela();
                                    break;
                                }
                            } while(rOpc!=5);
                            break;
                        }
                    }else if(verif == 1){
                        printf("\nUsuário correto, senha inválida.");
                        proxTela();
                    }else{
                        printf("\nNão foi possível encontrar o usuário.");
                        proxTela();
                    }
                break;

                case 2:
                    printf("\nVocê escolheu sair do programa. tenha um bom dia!");

                    clearList(Lista); // É necessário apenas apagar a lista da memória, já que o que devia ser salvo já foi salvo :D.

                    proxTela();
                break;

                case 598752185463186196751981535249151: // Debug.
                    system("cls");
                    printf("Os usuários que estão na lista:\t\t\tQuantidade de usuários: %d\n\n", Lista->cont);
                    
                    printList(Lista);
                    proxTela();
                break;
            
                default:
                printf("\nOpção inválida. tente escolher entre 1 e 2.");
                                        
                proxTela();
                break;
            }
        } while (rLog!=2);
    }
    return 0;
}