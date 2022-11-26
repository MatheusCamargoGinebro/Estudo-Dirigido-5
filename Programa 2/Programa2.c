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
    // Dados obrigatórios da atividade
    int id;
    char nome[128]; // Desculpa professor, mas doia muito no coração colocar o tamanho das strings como 100 e 50... colocamos números que estão na base 2 (2^7==128 e 2^6==64).
    char usuario[64];
    char senha[64];
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
} list;



/*------------------------------< Functions >------------------------------*/

/*-------------< Funções de modificação de lista/arquivo >-------------*/
// Função estética.
void proxTela(){
    printf("\n\n");
    system("pause");
    system("cls");
}

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

        return lista;
    }
}

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
    if(lista == NULL || lista->inicio == NULL){ // Ou não há lista, ou a lista está vazia.
        return 0;
    }else{ // Há lista, e ela será limpa.
    node *atualUser;

    //enquanto o inicio da lista apontar para um nó
    while(lista->inicio != NULL){
            //recupera o primeiro elemento da lista
            atualUser = lista->inicio;

            //faz com que o próximo elemento seja o primeiro elemento da lista;
            lista->inicio = atualUser->proximo;

            //remove o nodo da lista
            free(atualUser);
    }
    //libera a memória da variável que representa a lista
    free(lista);
    }
    return 1;
}*/


/*-------------< Funções de exibição e locomoção na lista/arquivo >-------------*/
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
        return 1 == Usuário encontrado, senha inválida.
        return 0 == usuário e senha corretos.
        return -1 == não foi encontrado usuário.
    */
}

// Imprimer lista (para debug).
void printuser(node *userToPrint){
    printf("Id: %d\n", userToPrint->UserNode.id);
    printf("Nome: %s", userToPrint->UserNode.nome);
    printf("Bairro: %s", userToPrint->UserNode.endereco.bairro);
    printf("Rua: %s", userToPrint->UserNode.endereco.rua);
    printf("Número: %d\n", userToPrint->UserNode.endereco.numero);
    printf("CEP: %d\n", userToPrint->UserNode.endereco.cep);
    printf("Usuário: %s", userToPrint->UserNode.usuario);
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

    if(usersDataBase == NULL){ // Não foi possível abrir o arquivo. (tentará criar outro arquivo).
        usersDataBase = fopen("userDataBase.bin", "wb");
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

            // Dar a senha para o login do *SUPERUSUÁRIO*
            printf("\nInsira a Senha do *SUPERUSUÁRIO* (parâmetro para fazer o login).\nR: ");
            setbuf(stdin, NULL);
            fgets(superUser->UserNode.senha, 64, stdin);

            //Definir o tipo de usuário (no caso, é fixo em *SUPERUSUÁRIO*).
            printf("\nO *SUPERUSUÁRIO* recebe o tipo S (*SUPERUSUÁRIO*).");
            superUser->UserNode.tipo = 'S';

            // Definindo que o *SUPERUSUÁRIO* estará no arquivo.
            superUser->isOnFile = 1;
    
            proxTela();

            // Insere o *SUPERUSUÁRIO* na lista.
            fwrite(superUser, sizeof(node), 1, usersDataBase);
            // Salva a lista.
            fclose(usersDataBase);
            // Libera a memória usada para guardar os dados do *SUPERUSUÁRIO*.
            free(superUser);
            // Abre a lista novamente, desta vez em modo de leitura.
            usersDataBase = fopen("userDataBase.bin", "rb");

            if(usersDataBase == NULL){
                printf("O Super usuário foi salvo, porém não foi possível abrir o arquivo em modo leitura.\n");
                proxTela();
                exit(1);
            }
        }else{
            printf("\nERRO: não foi possível criar/carregar o arquivo. adios.");
            proxTela();
            exit(1);
        }
    }

    // Carregando dados dos usuários para a lista.
    if(loadListToMemo(Lista, usersDataBase)!=0){
        printf("O============================================================O\n| Não foi possível carregar os nós do arquivo para a memória |\nO============================================================O");
        proxTela();
        exit(1);
    }else{
        printf("O========================================================O\n| Os nós salvos no arquivo foram carregados com sucesso. |\nO========================================================O");
        proxTela();

        int rLog; // Variável de resposta para login.

        do
        {
            printf("O=============================O\n| Sistema de Login e arquivos |\nO=============================O\n|     O que deseja fazer?     |\n|          [1] Login.         |\n|          [2] Sair.          |\nO=============================O\n\nR: ");
            scanf("%d", &rLog);

            switch (rLog){
                case 1:
                printf("a");
                    // Reservando um espaço na memória para o usuário atual, de forma dinâmica.
                    user *atualUser = (user*)malloc(sizeof(user));
                    int verif;
                    do
                    {
                        system("cls");
                        printf("O===========================O\n| Você escolheu fazer login |\nO===========================O\n\n");
                    
                        // Pegando o nome do usuário.
                        printf("Digite seu de usuário.\nR: ");
                        setbuf(stdin, NULL);
                        fgets(atualUser->usuario, 64, stdin);

                        // Pegando a senha do usuário.
                        printf("Digite sua senha de usuário.\nR: ");
                        setbuf(stdin, NULL);
                        fgets(atualUser->senha, 64, stdin);

                        // Criando um variável de verificação e testando o login do usuário.
                        verif = verifLogin(Lista, atualUser);

                        if(verif == 0){
                            printf("Usuário e senha corretos.");
                        }else if(verif == 1){
                            printf("Usuário correto, senha inválida.");
                            proxTela();
                        }else{
                            printf("Não foi possível encontrar o usuário.");
                            proxTela();
                        }
                    }while(verif != 0);

                    printf("passou");
                    proxTela();
                break;

                case 2:
                    printf("\nVocê escolheu sair do programa. tenha um bom dia!");
                break;

                case 3: // Debug.
                    system("cls");
                    printf("O===================O\n| Opção 3: DEBUG :D |\nO===================O\n\nOs usuários que estão na lista:\n\n");
                    
                    printList(Lista);
                    proxTela;
                break;
            
                default:
                printf("\nOpção inválida. tente escolher entre 1 e 2.");
                break;
            }
            proxTela();
        } while (rLog!=2);
        

    }

    
    
    
    system("pause");
    return 0;
}