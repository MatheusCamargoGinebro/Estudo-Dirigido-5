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
    int cont;
} list;



/*------------------------------< Functions >------------------------------*/

/*-------------< Funções de modificação de lista/arquivo >-------------*/
// Função estética.
void proxTela(){
    printf("\n\n");
    system("pause");
    system("cls");
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
    printf("Salvo no arquivo == %d.\n\n", userToPrint->isOnFile);
}
void printList(list *lista){
    for(node *I = lista->inicio; I!=NULL; I = I->proximo){
        printuser(I);
    }
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
        lista->cont = 0;

        return lista;
    }
}

// Limpar a lista.
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


int idGenerator(list *lista){
    int cont=lista->cont;
    for(node *I = lista->inicio; I!=NULL; I = I->proximo){
        if(cont==I->UserNode.id){
            cont--;
        }else{
            break;
        }
    }
    return cont;
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
        lista->cont++;
    }
}


int removeUser(list *Lista, FILE *fileToEdit, user *Removedor, int Id){
    node *anterior;
    for(node *I = Lista->inicio; I!=NULL; anterior = I, I = I->proximo){ // Não há necessidade de testar se a lista está vazia, já que sempre haverá ao menos um elemento dentro dela.
            if(Id == I->UserNode.id){
                if(I->UserNode.tipo=='S'){
                    return -1;
                }else{
                    if(Removedor->tipo=='S' || (Removedor->tipo == 'A' && I->UserNode.tipo == 'C')){ // Indica permissão para remover.
                        if(I == Lista->inicio){ // Se o elemento está no início da lista
                        // O segundo elemento passa a ser o início da lista
                        Lista->inicio = I->proximo;

                        }else{// O Elemento está no meio da lista.
                            // O proximo do nó anterior passa a ser o proximo do nó encontrado;
                            anterior->proximo = I->proximo;
                        }
                        
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
                                    uRemFil->isOnFile = -1;

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


int loadListToMemo(list *lista, FILE *fileToLoad){
    int cont=0;
    while (1){
        node *getUser = (node*)malloc(sizeof(node));
        fread(getUser, sizeof(node), 1, fileToLoad);

        if(feof(fileToLoad)){
            break;
        }
        if(getUser->isOnFile!=-1){
            getUser->UserNode.id = cont;
            
            addUser(lista, getUser);
            cont++;
        }
    }

    if (lista->inicio!=NULL){
        return 0; // Carregado com sucesso.
    }else{
        return 1; // Deu algum problema.
    }
}


void addUserToFile(FILE *fileToAdd, node *NewUser){
    fileToAdd = fopen("userDataBase.bin", "r+b");
    
    while (1){
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



/*-------------< Funções de verificação e afins >-------------*/
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
    /*      Gabarito do return (verif):
        return 1 == Usuário encontrado, senha inválida.
        return 0 == usuário e senha corretos.
        return -1 == não foi encontrado usuário.
    */
}

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
            addUserToFile(usersDataBase,superUser);
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
                    printf("\nDigite sua senha.\nR: ");
                    setbuf(stdin, NULL);
                    fgets(atualUser->senha, 64, stdin);

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
                                        char newPassword[64];

                                        printf("Digite a nova senha.\nR:");
                                        setbuf(stdin, NULL);
                                        fgets(newPassword, 64, stdin);

                                        if(changePassword(Lista, usersDataBase, atualUser, newPassword)==0){
                                            printf("\nNão foi possível alterar a senha.");
                                        }else{
                                            printf("\nSenha alterada com sucesso.");
                                        }
                                        proxTela();
                                    break;
                                    
                                    case 2:
                                        system("cls");
                                        printf("O=============================O\n| Você escolheu [2] deslogar. |\nO=============================O");
                                        
                                        proxTela();
                                    break;

                                    case 3: // Debug.
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
                                        printf("\nDigite a senha do usuário que você quer adicionar.\nR: ");
                                        setbuf(stdin, NULL);
                                        fgets(novoUsuario->UserNode.senha, 64, stdin);

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
                                        char newPassword[64];

                                        printf("Digite a nova senha.\nR:");
                                        setbuf(stdin, NULL);
                                        fgets(newPassword, 64, stdin);

                                        if(changePassword(Lista, usersDataBase, atualUser, newPassword)==0){
                                            printf("\nNão foi possível alterar a senha.");
                                        }else{
                                            printf("\nSenha alterada com sucesso.");
                                        }

                                        proxTela();
                                    break;
                                    
                                    case 5:
                                        system("cls");
                                        printf("O=============================O\n| Você escolheu [5] deslogar. |\nO=============================O");

                                        proxTela();
                                    break;

                                    case 6: // Debug.
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

                case 3: // Debug.
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