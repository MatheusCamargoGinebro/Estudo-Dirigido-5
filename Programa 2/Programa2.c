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

    int cont;

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
        printf(
        "ID: %d\nNome: %sUsu�rio: %sTipo: %s\nEndere�o:\nRua %s, bairro %s, n�mero %d, cep %d\n\n", 
        I->UserNode.id, 
        I->UserNode.nome, 
        I->UserNode.usuario, 
        I->UserNode.tipo, 
        I->UserNode.endereco.rua, 
        I->UserNode.endereco.bairro, 
        I->UserNode.endereco.numero, 
        I->UserNode.endereco.cep);
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
    user rUser;// = (user*)malloc(1*sizeof(user));

    FILE *usersDataBase = fopen("userDataBase.bin", "r+");

    if(usersDataBase == NULL){ // Lista vazia
        usersDataBase = fopen("userDataBase.bin", "w+");
        if(usersDataBase!=NULL){// Arquivo criado com sucesso.
            printf("O==================================================================================O\n| A lista foi rec�m criada, sendo assim ser� necess�rio cadastrar o *SUPERUSU�RIO* |\nO==================================================================================O");
            proxTela();

            // Dar Id ao *SUPERUSU�RIO*.
            rUser.id = idGenerator(Lista);
            printf("O Id do *SUPERUSU�RIO* � %d.\n", rUser.id);

            // Dar Nome ao *SUPERUSU�RIO*.
            printf("\nInsira o nome do *SUPERUSU�RIO*.\nR:");
            setbuf(stdin, NULL);
            fgets(rUser.nome, 128, stdin);

            // PEGAR ENDERE�O DO *SUPERUSU�RIO*:
            // Rua do *SUPERUSU�RIO*
            printf("\nInsira a rua do *SUPERUSU�RIO*.\nR:");
            setbuf(stdin, NULL);
            fgets(rUser.endereco.rua, 128, stdin);

            // Bairro do *SUPERSU�RIO*
            printf("\nInsira o bairro do *SUPERUSU�RIO*.\nR:");
            setbuf(stdin, NULL);
            fgets(rUser.endereco.bairro, 128, stdin);

            // N�mero da casa do *SUPERSU�RIO*
            printf("\nInsira o n�mero da casa do *SUPERSU�RIO*.\nR:");
            scanf("%d", &rUser.endereco.numero);

            // CEP do *SUPERSU�RIO*
            printf("\nInsira o CEP do *SUPERSU�RIO*.\nR:");
            scanf("%d", &rUser.endereco.cep);
            
            // Dar o par�metro de login do *SUPERUSU�RIO*.
            printf("\nInsira o Nick do *SUPERUSU�RIO* (par�metro para fazer o login).\nR:");
            setbuf(stdin, NULL);
            fgets(rUser.usuario, 64, stdin);

            // Dar a senha para o login do *SUPERUSU�RIO*
            printf("\nInsira a Senha do *SUPERUSU�RIO* (par�metro para fazer o login).\nR:");
            setbuf(stdin, NULL);
            fgets(rUser.senha, 64, stdin);

            //Definir o tipo de usu�rio (no caso, � fixo em *SUPERUSU�RIO*).
            printf("\nO *SUPERUSU�RIO* recebe o tipo S (*SUPERUSU�RIO*).\n");
            rUser.tipo = 'S';
            
            proxTela();

            fwrite(&rUser, sizeof(user), 1, usersDataBase);
        }else{
            printf("\nERRO: n�o foi poss�vel criar/carregar o arquivo. \"adios\".");
            proxTela();
            exit(1);
        }
       

        
    }else{
        printf("Arquivo j� criado");

        //pode ter coisa


    }

    

    


    fclose(usersDataBase);
    system("pause");
    return 0;
}