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
O=======================================================================O
*/

/*------------------------------< Structs >------------------------------*/
typedef struct{
    char rua[128];
    char bairro[128];
    int numero;
    int cep;
} address;


typedef struct{
    // Dados obrigat�rios da atividade
    int id;
    char nome[128]; // Desculpa professor, mas doia muito no cora��o colocar o tamanho das strings como 100 e 50... colocamos n�meros que est�o na base 2 (2^7==128 e 2^6==64).
    char usuario[64];
    char senha[64];
    char tipo;
    address endereco;
} user;


// Estrutura de n� (node).
typedef struct NODE{
    // Estrutura do usu�rio
    user UserNode;

    // vari�vel para verificar se o n� est� ou n�o no arquivo criado pelo programa.
    int isOnFile;

    // Ponteiro que aponta para o endere�o do pr�ximo n�.
    struct NODE *proximo;
} node;

// Estrutura de lista.
typedef struct{
    node *inicio;
    node *fim;
} list;



/*------------------------------< Functions >------------------------------*/
// Fun��o est�tica.
void proxTela(){
    printf("\n\n");
    system("pause");
    system("cls");
}



list *CreateList(){
    // Declara uma lista vazia din�micamente.
    list *lista = (list *)malloc(sizeof(list));

    // Verifica se a lista foi realmente criada.
    if (lista == NULL){ // Caso haja erro, filaizar� o programa.
        printf("\nErro ao alocar mem�ria para a lista.\n");

        exit(1);
    }else{ // Caso n�o haja erro, o programa inicializa os elementos da lista como NULL (indica lista vazia).
        lista->inicio = NULL;
        lista->fim = NULL;

        return lista;
    }
}

int idGenerator(list *lista){
    int cont=0;
    for(node *I = lista->inicio; I!=NULL; I = I->proximo){
        cont++;
    }
    return cont;
}



void addUser(list *lista, user usuarioNode){
    //cria o node da lista din�mincamente.
    node *NewUser = (node*)malloc(sizeof(node));

    //Verifica se o n� foi criado com �xito.
    if(NewUser == NULL){
        printf("\nErro ao adicionar novo usu�rio.\n");
        proxTela();
        exit(1);  //finaliza o programa retornando c�digo de erro 1.
    }else{
        // Inserindo elemento no inicio da lista.
        NewUser->UserNode = usuarioNode;
        NewUser->proximo = lista->inicio;

        // Verificando se a lista est� vazia.
        if(lista->inicio == NULL){ // Caso esteja vazia, o primeiro elemento tamb�m ser� o �ltimo.
            lista->fim = NewUser;
        }
        lista->inicio = NewUser;
    }
}


// Imprimer lista (para debug).
void printList(list *lista){
    for(node *I = lista->inicio; I!=NULL; I = I->proximo){
        printf("ID: %d\nNome: %sUsu�rio: %sTipo: %s\nEndere�o:\nRua %s, bairro %s, n�mero %d, cep %d\n\n", I->UserNode.id, I->UserNode.nome, I->UserNode.usuario, I->UserNode.tipo, I->UserNode.endereco.rua, I->UserNode.endereco.bairro, I->UserNode.endereco.numero, I->UserNode.endereco.cep);
    }
}


// Limpar a lista (apenas no final do programa).a
int clearUers(list *lista){
    if(lista == NULL || lista->inicio == NULL){ // Ou n�o h� lista, ou a lista est� vazia.
        return 0;
    }else{ // H� lista, e ela ser� limpa.
    node *atualUser;

    //enquanto o inicio da lista apontar para um n�
    while(lista->inicio != NULL){
            //recupera o primeiro elemento da lista
            atualUser = lista->inicio;

            //faz com que o pr�ximo elemento seja o primeiro elemento da lista;
            lista->inicio = atualUser->proximo;

            //remove o nodo da lista
            free(atualUser);
    }
    //libera a mem�ria da vari�vel que representa a lista
    free(lista);
    }
    return 1;
}
/*------------------------------< Main >------------------------------*/
int main(){
    setlocale(LC_ALL, "portuguese");

    //Iniciando arquvos e listas.

    list *Lista = CreateList();

    FILE *usersDataBase = fopen("userDataBase.bin", "rb");

    if(usersDataBase == NULL){ // N�o foi poss�vel abrir o arquivo.
        usersDataBase = fopen("userDataBase.bin", "wb");
        if(usersDataBase!=NULL){// Arquivo criado com sucesso.
            printf("O==================================================================================O\n| A lista foi rec�m criada, sendo assim ser� necess�rio cadastrar o *SUPERUSU�RIO* |\nO==================================================================================O");
            proxTela();
            node superUser;

            // Dar Id ao *SUPERUSU�RIO*.
            superUser.UserNode.id = idGenerator(Lista);
            printf("O Id do *SUPERUSU�RIO* � %d.\n", superUser.UserNode.id);

            // Dar Nome ao *SUPERUSU�RIO*.
            printf("\nInsira o nome do *SUPERUSU�RIO*.\nR: ");
            setbuf(stdin, NULL);
            fgets(superUser.UserNode.nome, 128, stdin);

            // PEGAR ENDERE�O DO *SUPERUSU�RIO*:
            // Rua do *SUPERUSU�RIO*
            printf("\nInsira a rua do *SUPERUSU�RIO*.\nR: ");
            setbuf(stdin, NULL);
            fgets(superUser.UserNode.endereco.rua, 128, stdin);

            // Bairro do *SUPERSU�RIO*
            printf("\nInsira o bairro do *SUPERUSU�RIO*.\nR: ");
            setbuf(stdin, NULL);
            fgets(superUser.UserNode.endereco.bairro, 128, stdin);

            // N�mero da casa do *SUPERSU�RIO*
            printf("\nInsira o n�mero da casa do *SUPERSU�RIO*.\nR: ");
            scanf("%d", &superUser.UserNode.endereco.numero);

            // CEP do *SUPERSU�RIO*
            printf("\nInsira o CEP do *SUPERSU�RIO*.\nR: ");
            scanf("%d", &superUser.UserNode.endereco.cep);
            
            // Dar o par�metro de login do *SUPERUSU�RIO*.
            printf("\nInsira o Nick do *SUPERUSU�RIO* (par�metro para fazer o login).\nR: ");
            setbuf(stdin, NULL);
            fgets(superUser.UserNode.usuario, 64, stdin);

            // Dar a senha para o login do *SUPERUSU�RIO*
            printf("\nInsira a Senha do *SUPERUSU�RIO* (par�metro para fazer o login).\nR: ");
            setbuf(stdin, NULL);
            fgets(superUser.UserNode.senha, 64, stdin);

            //Definir o tipo de usu�rio (no caso, � fixo em *SUPERUSU�RIO*).
            printf("\nO *SUPERUSU�RIO* recebe o tipo S (*SUPERUSU�RIO*).\n");
            superUser.UserNode.tipo = 'S';

            // Definindo que o *SUPERUSU�RIO* estar� no arquivo.
            superUser.isOnFile = 1;
            superUser.proximo = NULL;

            printf("\nO *SUPERUSU�RIO* foi cadastrado.\n");
            printf("Id: %d\n", superUser.UserNode.id);
            printf("Nome: %s", superUser.UserNode.nome);
            printf("Bairro: %s", superUser.UserNode.endereco.bairro);
            printf("Rua: %s", superUser.UserNode.endereco.rua);
            printf("N�mero: %d\n", superUser.UserNode.endereco.numero);
            printf("CEP: %d\n", superUser.UserNode.endereco.cep);
            printf("Nick: %s", superUser.UserNode.usuario);
            printf("Senha: %s", superUser.UserNode.senha);
            printf("Tipo: %c\n", superUser.UserNode.tipo);

            printf("\nInforma��es adicionais:\n");
            printf("Usu�rio no arquivo == %d.\n", superUser.isOnFile);
            printf("endere�o do pr�ximo usu�rio: %d.\n", superUser.proximo);
    
            proxTela();

            fwrite(&superUser, sizeof(node), 1, usersDataBase);
            fclose(usersDataBase);

            usersDataBase = fopen("userDataBase.bin", "rb");
        }else{
            printf("\nERRO: n�o foi poss�vel criar/carregar o arquivo. adios.");
            proxTela();
            exit(1);
        }
    }
    
    node rUser;

    fread(&rUser, sizeof(node), 1, usersDataBase);

            printf("\nO *SUPERUSU�RIO* foi cadastrado.\n\nInforma��es:\n");
            printf("Id: %d\n", rUser.UserNode.id);
            printf("Nome: %s", rUser.UserNode.nome);
            printf("Bairro: %s", rUser.UserNode.endereco.bairro);
            printf("Rua: %s", rUser.UserNode.endereco.rua);
            printf("N�mero: %d\n", rUser.UserNode.endereco.numero);
            printf("CEP: %d\n", rUser.UserNode.endereco.cep);
            printf("Nick: %s", rUser.UserNode.usuario);
            printf("Senha: %s", rUser.UserNode.senha);
            printf("Tipo: %c\n", rUser.UserNode.tipo);

            printf("\nInforma��es adicionais:\n");
            printf("Usu�rio no arquivo == %d.\n", rUser.isOnFile);
            printf("endere�o do pr�ximo usu�rio: %d.\n", rUser.proximo);
    
    system("pause");
    return 0;
}