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

/*-------------< Fun��es de modifica��o de lista/arquivo >-------------*/
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

void addUser(list *lista, node *usuarioNode){
    //cria o node da lista din�mincamente.
    node *NewUser = (node*)malloc(sizeof(node));

    //Verifica se o n� foi criado com �xito.
    if(NewUser == NULL){
        printf("\nErro ao adicionar novo usu�rio.\n");
        proxTela();
        exit(1);  //finaliza o programa retornando c�digo de erro 1.
    }else{
        // Inserindo elemento no inicio da lista.
        NewUser = usuarioNode;
        NewUser->proximo = lista->inicio;

        // Verificando se a lista est� vazia.
        if(lista->inicio == NULL){ // Caso esteja vazia, o primeiro elemento tamb�m ser� o �ltimo.
            lista->fim = NewUser;
        }
        lista->inicio = NewUser;
    }
}

int idGenerator(list *lista){
    int cont=0;
    for(node *I = lista->inicio; I!=NULL; I = I->proximo){
        cont++;
    }
    return cont;
}


int loadListToMemo(list *lista, FILE *fileToLoad){
    node *getUser = (node*)malloc(sizeof(node));
    int cont=0;
    do{
        fread(getUser, sizeof(node), 1, fileToLoad);
        addUser(lista, getUser);
        cont++;
    }while(fseek(fileToLoad, cont*sizeof(node), SEEK_SET)!=0);

    if (lista->inicio!=NULL){
        return 0; // Carregado com sucesso.
    }else{
        return 1; // Deu algum problema.
    }
    
}
/*
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
}*/


/*-------------< Fun��es de exibi��o e locomo��o na lista/arquivo >-------------*/
int verifLogin(list *Lista, user *atualUser){
    int verif;
    for(node *I = Lista->inicio; I!=NULL; I = I->proximo){
        if(strcmp(atualUser->usuario, I->UserNode.usuario)==0){
            verif = 1;
            if(strcmp(atualUser->senha, I->UserNode.senha)==0){
                verif = 0;
            } 
        }
        else{
            verif=-1;
        }
    }
    return verif; 
    /*      Gabarito do return (verif):
        return 1 == Usu�rio encontrado, senha inv�lida.
        return 0 == usu�rio e senha corretos.
        return -1 == n�o foi encontrado usu�rio.
    */
}

// Imprimer lista (para debug).
void printuser(node *userToPrint){
    printf("Id: %d\n", userToPrint->UserNode.id);
    printf("Nome: %s", userToPrint->UserNode.nome);
    printf("Bairro: %s", userToPrint->UserNode.endereco.bairro);
    printf("Rua: %s", userToPrint->UserNode.endereco.rua);
    printf("N�mero: %d\n", userToPrint->UserNode.endereco.numero);
    printf("CEP: %d\n", userToPrint->UserNode.endereco.cep);
    printf("Usu�rio: %s", userToPrint->UserNode.usuario);
    printf("Senha: %s", userToPrint->UserNode.senha);
    printf("Tipo: %c\n", userToPrint->UserNode.tipo);
}
void printList(list *lista){
    for(node *I = lista->inicio; I!=NULL; I = I->proximo){
        printuser(I);
    }
}





/*------------------------------< Main >------------------------------*/
int main(){
    setlocale(LC_ALL, "portuguese");

    //Iniciando arquvo e lista.
    list *Lista = CreateList();

    FILE *usersDataBase = fopen("userDataBase.bin", "rb");

    if(usersDataBase == NULL){ // N�o foi poss�vel abrir o arquivo. (tentar� criar outro arquivo).
        usersDataBase = fopen("userDataBase.bin", "wb");
        if(usersDataBase!=NULL){// Arquivo criado com sucesso.
            node *superUser = (node*)malloc(sizeof(node));
            printf("O==================================================================================O\n| A lista foi rec�m criada, sendo assim ser� necess�rio cadastrar o *SUPERUSU�RIO* |\nO==================================================================================O");
            proxTela();

            // Dar Id ao *SUPERUSU�RIO*.
            superUser->UserNode.id = idGenerator(Lista);
            printf("O Id do *SUPERUSU�RIO* � %d.\n", superUser->UserNode.id);

            // Dar Nome ao *SUPERUSU�RIO*.
            printf("\nInsira o nome do *SUPERUSU�RIO*.\nR: ");
            setbuf(stdin, NULL);
            fgets(superUser->UserNode.nome, 128, stdin);

            // PEGAR ENDERE�O DO *SUPERUSU�RIO*:
            // Rua do *SUPERUSU�RIO*
            printf("\nInsira a rua do *SUPERUSU�RIO*.\nR: ");
            setbuf(stdin, NULL);
            fgets(superUser->UserNode.endereco.rua, 128, stdin);

            // Bairro do *SUPERSU�RIO*
            printf("\nInsira o bairro do *SUPERUSU�RIO*.\nR: ");
            setbuf(stdin, NULL);
            fgets(superUser->UserNode.endereco.bairro, 128, stdin);

            // N�mero da casa do *SUPERSU�RIO*
            printf("\nInsira o n�mero da casa do *SUPERSU�RIO*.\nR: ");
            scanf("%d", &superUser->UserNode.endereco.numero);

            // CEP do *SUPERSU�RIO*
            printf("\nInsira o CEP do *SUPERSU�RIO*.\nR: ");
            scanf("%d", &superUser->UserNode.endereco.cep);
            
            // Dar o par�metro de login do *SUPERUSU�RIO*.
            printf("\nInsira o Nick do *SUPERUSU�RIO* (par�metro para fazer o login).\nR: ");
            setbuf(stdin, NULL);
            fgets(superUser->UserNode.usuario, 64, stdin);

            // Dar a senha para o login do *SUPERUSU�RIO*
            printf("\nInsira a Senha do *SUPERUSU�RIO* (par�metro para fazer o login).\nR: ");
            setbuf(stdin, NULL);
            fgets(superUser->UserNode.senha, 64, stdin);

            //Definir o tipo de usu�rio (no caso, � fixo em *SUPERUSU�RIO*).
            printf("\nO *SUPERUSU�RIO* recebe o tipo S (*SUPERUSU�RIO*).");
            superUser->UserNode.tipo = 'S';

            // Definindo que o *SUPERUSU�RIO* estar� no arquivo.
            superUser->isOnFile = 1;
    
            proxTela();

            // Insere o *SUPERUSU�RIO* na lista.
            fwrite(superUser, sizeof(node), 1, usersDataBase);
            // Salva a lista.
            fclose(usersDataBase);
            // Libera a mem�ria usada para guardar os dados do *SUPERUSU�RIO*.
            free(superUser);
            // Abre a lista novamente, desta vez em modo de leitura.
            usersDataBase = fopen("userDataBase.bin", "rb");

            if(usersDataBase == NULL){
                printf("O Super usu�rio foi salvo, por�m n�o foi poss�vel abrir o arquivo em modo leitura.\n");
                proxTela();
                exit(1);
            }
        }else{
            printf("\nERRO: n�o foi poss�vel criar/carregar o arquivo. adios.");
            proxTela();
            exit(1);
        }
    }

    // Carregando dados dos usu�rios para a lista.
    if(loadListToMemo(Lista, usersDataBase)!=0){
        printf("O============================================================O\n| N�o foi poss�vel carregar os n�s do arquivo para a mem�ria |\nO============================================================O");
        proxTela();
        exit(1);
    }else{
        printf("O========================================================O\n| Os n�s salvos no arquivo foram carregados com sucesso. |\nO========================================================O");
        proxTela();

        int rLog; // Vari�vel de resposta para login.

        do
        {
            printf("O=============================O\n| Sistema de Login e arquivos |\nO=============================O\n|     O que deseja fazer?     |\n|          [1] Login.         |\n|          [2] Sair.          |\nO=============================O\n\nR: ");
            scanf("%d", &rLog);

            switch (rLog){
                case 1:
                printf("a");
                    // Reservando um espa�o na mem�ria para o usu�rio atual, de forma din�mica.
                    user *atualUser = (user*)malloc(sizeof(user));
                    int verif;
                    do
                    {
                        system("cls");
                        printf("O===========================O\n| Voc� escolheu fazer login |\nO===========================O\n\n");
                    
                        // Pegando o nome do usu�rio.
                        printf("Digite seu de usu�rio.\nR: ");
                        setbuf(stdin, NULL);
                        fgets(atualUser->usuario, 64, stdin);

                        // Pegando a senha do usu�rio.
                        printf("Digite sua senha de usu�rio.\nR: ");
                        setbuf(stdin, NULL);
                        fgets(atualUser->senha, 64, stdin);

                        // Criando um vari�vel de verifica��o e testando o login do usu�rio.
                        verif = verifLogin(Lista, atualUser);

                        if(verif == 0){
                            printf("Usu�rio e senha corretos.");
                        }else if(verif == 1){
                            printf("Usu�rio correto, senha inv�lida.");
                            proxTela();
                        }else{
                            printf("N�o foi poss�vel encontrar o usu�rio.");
                            proxTela();
                        }
                    }while(verif != 0);

                    printf("passou");
                    proxTela();
                break;

                case 2:
                    printf("\nVoc� escolheu sair do programa. tenha um bom dia!");
                break;

                case 3: // Debug.
                    system("cls");
                    printf("O===================O\n| Op��o 3: DEBUG :D |\nO===================O\n\nOs usu�rios que est�o na lista:\n\n");
                    
                    printList(Lista);
                    proxTela;
                break;
            
                default:
                printf("\nOp��o inv�lida. tente escolher entre 1 e 2.");
                break;
            }
            proxTela();
        } while (rLog!=2);
        

    }

    
    
    
    system("pause");
    return 0;
}