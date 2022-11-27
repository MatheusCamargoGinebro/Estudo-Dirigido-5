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
    int cont;
} list;



/*------------------------------< Functions >------------------------------*/

/*-------------< Fun��es de modifica��o de lista/arquivo >-------------*/
// Fun��o est�tica.
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
    printf("N�mero: %d\n", userToPrint->UserNode.endereco.numero);
    printf("CEP: %d\n", userToPrint->UserNode.endereco.cep);
    printf("Usu�rio: %s", userToPrint->UserNode.usuario);
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
    // Declara uma lista vazia din�micamente.
    list *lista = (list *)malloc(sizeof(list));

    // Verifica se a lista foi realmente criada.
    if (lista == NULL){ // Caso haja erro, filaizar� o programa.
        printf("\nErro ao alocar mem�ria para a lista.\n");

        exit(1);
    }else{ // Caso n�o haja erro, o programa inicializa os elementos da lista como NULL (indica lista vazia).
        lista->inicio = NULL;
        lista->fim = NULL;
        lista->cont = 0;

        return lista;
    }
}

// Limpar a lista.
void clearList(list *Lista){
    node *user;

    // Enquanto o inicio da lista apontar para um n�.
    while(Lista->inicio != NULL){
        // Recupera o primeiro elemento da lista.
        user = Lista->inicio;

        // Faz com que o pr�ximo elemento seja o primeiro elemento da lista.
        Lista->inicio = user->proximo;

        // Remove o nodo da lista.
        free(user);
    }
    // Libera a mem�ria da vari�vel que representa a lista.
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
        lista->cont++;
    }
}


int removeUser(list *Lista, FILE *fileToEdit, user *Removedor, int Id){
    node *anterior;
    for(node *I = Lista->inicio; I!=NULL; anterior = I, I = I->proximo){ // N�o h� necessidade de testar se a lista est� vazia, j� que sempre haver� ao menos um elemento dentro dela.
            if(Id == I->UserNode.id){
                if(I->UserNode.tipo=='S'){
                    return -1;
                }else{
                    if(Removedor->tipo=='S' || (Removedor->tipo == 'A' && I->UserNode.tipo == 'C')){ // Indica permiss�o para remover.
                        if(I == Lista->inicio){ // Se o elemento est� no in�cio da lista
                        // O segundo elemento passa a ser o in�cio da lista
                        Lista->inicio = I->proximo;

                        }else{// O Elemento est� no meio da lista.
                            // O proximo do n� anterior passa a ser o proximo do n� encontrado;
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

                        //libera a mem�ria ocupada pelo n�.
                        free(I);
                        Lista->cont--;
                        //retorna 1 (true) indicando que encontrou e removeu o elemento da lista
                        return 1;
                    }else if(Removedor->tipo=='A' && I->UserNode.tipo == 'A'){ // N�o tem permiss�o para remover.
                        return -2;
                    }
                }
            }
        }
    // Retorna 2 (false) indicando que n�o foi poss�vel encontrar o usu�rio.
    return 0;
}
/*Gabarito do return:
    return == -1 == O Id era o do superusu�rio, ele n�o pode ser removido.
    return == -2 == n�o havia permiss�o para remover o usu�rio.
    return == 0 == n�o foi poss�vel encontrar o usu�rio.
    return == 1 == usu�rio removido.
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



/*-------------< Fun��es de verifica��o e afins >-------------*/
int verifLogin(list *Lista, user *atualUser){
    int verif = -1;
    for(node *I = Lista->inicio; I!=NULL; I = I->proximo){
        if(strcmp(atualUser->usuario, I->UserNode.usuario)==0){
            verif = 1; // Usu�rio verificado
            if(strcmp(atualUser->senha, I->UserNode.senha)==0){
                verif = 0; // Senha verificada
                *atualUser = I->UserNode;
            } 
        }
    }
    return verif; 
    /*      Gabarito do return (verif):
        return 1 == Usu�rio encontrado, senha inv�lida.
        return 0 == usu�rio e senha corretos.
        return -1 == n�o foi encontrado usu�rio.
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
    
    if(usersDataBase == NULL){ // N�o foi poss�vel abrir o arquivo. (tentar� criar outro arquivo).
        fclose(usersDataBase);
        usersDataBase = fopen("userDataBase.bin", "wb");
        
        // Salva a lista.
        fclose(usersDataBase);
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
            addUserToFile(usersDataBase,superUser);
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
        fclose(usersDataBase); // Fechando/salvando o arquivo ap�s carreg�-lo para a mem�ria.
        proxTela();

        int rLog; // Vari�vel de resposta para login.

        do
        {
            printf("O=============================O\n| Sistema de Login e arquivos |\nO=============================O\n|     O que deseja fazer?     |\n|          [1] Login.         |\n|          [2] Sair.          |\nO=============================O\n\nR: ");
            scanf("%d", &rLog);

            switch (rLog){
                case 1:
                    system("cls");
                    // Reservando um espa�o na mem�ria para o usu�rio atual, de forma din�mica.
                    user *atualUser = (user*)malloc(1*sizeof(user));
                    int verif;

                    
                    printf("O===========================O\n| Voc� escolheu fazer login |\nO===========================O\n\n");
                
                    // Pegando o nome do usu�rio.
                    printf("Digite seu nome de usu�rio.\nR: ");
                    setbuf(stdin, NULL);
                    fgets(atualUser->usuario, 64, stdin);

                    // Pegando a senha do usu�rio.
                    printf("\nDigite sua senha.\nR: ");
                    setbuf(stdin, NULL);
                    fgets(atualUser->senha, 64, stdin);

                    // Passando o resultado da fun��o para uma vari�vel de verifica��o.
                    verif = verifLogin(Lista, atualUser);
                    
                    if(verif == 0){
                        printf("\nUsu�rio e senha corretos.\n\nlogado com sucesso!\n\nVoc� logou como %s", atualUser->usuario);
                        proxTela();
                        int rOpc;
                            
                        switch (atualUser->tipo)
                        {
                            case 'C': // Caso o usu�rio seja um cliente.
                            do{
                                system("cls");
                                printf("O======================================O\t\t\tInforma��es do usu�rio logado:\n|     Sistema de usu�rios - Logado     |\t\t\tNome: %sO======================================O\t\t\tUsu�rio: %s|    [1] Alterar senha.                |\t\t\tTipo: %c\n|    [2] Sair.                         |\nO======================================O\n\nR: ", atualUser->nome, atualUser->usuario, atualUser->tipo);
                                scanf("%d", &rOpc);

                                switch (rOpc){
                                    case 1:
                                        system("cls");
                                        printf("O====================================O\n| Voc� escolheu [1] alretar a senha. |\nO====================================O\n");
                                        char newPassword[64];

                                        printf("Digite a nova senha.\nR:");
                                        setbuf(stdin, NULL);
                                        fgets(newPassword, 64, stdin);

                                        if(changePassword(Lista, usersDataBase, atualUser, newPassword)==0){
                                            printf("\nN�o foi poss�vel alterar a senha.");
                                        }else{
                                            printf("\nSenha alterada com sucesso.");
                                        }
                                        proxTela();
                                    break;
                                    
                                    case 2:
                                        system("cls");
                                        printf("O=============================O\n| Voc� escolheu [2] deslogar. |\nO=============================O");
                                        
                                        proxTela();
                                    break;

                                    case 3: // Debug.
                                        system("cls");
                                        printf("Os usu�rios que est�o na lista:\t\t\tQuantidade de usu�rios: %d\n\n", Lista->cont);
                                        
                                        printList(Lista);
                                        proxTela();
                                    break;
                                    default:
                                        printf("Op��o inv�lida. escolha apenas entre 1 e 2");
                                        proxTela();
                                    break;
                                }
                            }while(rOpc!=2);
                            break;
                    
                            default: // Caso o usu�rio seja o *SUPERUSU�RIO* ou um Administrador.
                            do
                            {
                                system("cls");
                                printf("O======================================O\t\t\tInforma��es do usu�rio logado:\n|     Sistema de usu�rios - Logado     |\t\t\tNome: %sO======================================O\t\t\tUsu�rio: %s|    [1] Cadastrar usu�rio.            |\t\t\tTipo: %c\n|    [2] Remover usu�rio.              |\n|    [3] Pesquisar usu�rio por nome.   |\n|    [4] Alterar senha.                |\n|    [5] Sair.                         |\nO======================================O\n\nR: ", atualUser->nome, atualUser->usuario, atualUser->tipo);
                                scanf("%d", &rOpc);

                                switch (rOpc){
                                    case 1:
                                        system("cls");
                                        printf("O======================================O\n| Voc� escolheu [1] Cadastrar usu�rio. |\nO======================================O\n");

                                        node *novoUsuario = (node*)malloc(sizeof(node));
                                        int verif2;
                                        
                                        // pegar ID do novo usu�rio.
                                        novoUsuario->UserNode.id = idGenerator(Lista);

                                        // Pegar nome do novo usu�rio.
                                        printf("\nDigite o nome do usu�rio que voc� quer adicionar.\nR: ");
                                        setbuf(stdin, NULL);
                                        fgets(novoUsuario->UserNode.nome, 128, stdin);

                                        // Pegar dados de ENDERE�O.
                                        // Rua do novo usu�rio.
                                        printf("\nDigite a rua do usu�rio que voc� quer adicionar.\nR: ");
                                        setbuf(stdin, NULL);
                                        fgets(novoUsuario->UserNode.endereco.rua, 128, stdin);

                                        // Bairro do novo usu�rio.
                                        printf("\nDigite o bairro no usu�rio que voc� quer adicionar.\nR: ");
                                        setbuf(stdin, NULL);
                                        fgets(novoUsuario->UserNode.endereco.bairro, 128, stdin);

                                        // N�mero do novo usu�rio.
                                        printf("\nDigite o n�mero do usu�rio que voc� quer adicionar.\nR: ");
                                        scanf("%d", &novoUsuario->UserNode.endereco.numero);

                                        // Cep do novo usu�rio.
                                        printf("\nDigite o CEP do usu�rio que voc� quer adicionar.\nR: ");
                                        scanf("%d", &novoUsuario->UserNode.endereco.cep);

                                        // Nick de usu�rio.
                                        do{
                                            printf("\nDigite o nick do usu�rio (par�metro de login).\nR: ");
                                            setbuf(stdin, NULL);
                                            fgets(novoUsuario->UserNode.usuario, 64, stdin);

                                            verif2 = verifLogin(Lista, novoUsuario);
                                            if(verif2 == 1 || verif2 == 0){
                                                printf("\nUsu�rio j� cadastrado. tente outro nick.\n");
                                            }
                                        }while(verif2!=-1);
                                        
                                        // Senha do usu�rio.
                                        printf("\nDigite a senha do usu�rio que voc� quer adicionar.\nR: ");
                                        setbuf(stdin, NULL);
                                        fgets(novoUsuario->UserNode.senha, 64, stdin);

                                        // Tipo de usu�rio.
                                        do{
                                            printf("\nDigite o tipo de usu�rio.\nR: ");
                                            setbuf(stdin, NULL);
                                            scanf("%c", &novoUsuario->UserNode.tipo);

                                            if(atualUser->tipo == 'S'){
                                                if(novoUsuario->UserNode.tipo == 'A' || novoUsuario->UserNode.tipo == 'a'){
                                                    printf("\nUsu�rio ser� do tipo Administrador.\n");
                                                    verif2 = 0;
                                                }else if(novoUsuario->UserNode.tipo == 'C' || novoUsuario->UserNode.tipo == 'c'){
                                                    printf("\nUsu�rio ser� do tipo Cliente.\n");
                                                    verif2 = 0;
                                                }else if(novoUsuario->UserNode.tipo == 'S' || novoUsuario->UserNode.tipo == 's'){
                                                    printf("\nEste programa � pequeno demais para 2 *SUPERUSU�RIOS*, parceiro. op��o inv�lida.\n");
                                                    verif2 = 1;
                                                }else{
                                                    printf("\nOp��o inv�lida. escolha [A/C].\n");
                                                    verif2 = 1;
                                                }
                                            }else if(atualUser->tipo == 'A'){
                                                if(novoUsuario->UserNode.tipo == 'A' || novoUsuario->UserNode.tipo == 'a'){
                                                    printf("\nVoc� n�o tem permiss�o para adicionar um Administrador.\n");
                                                    verif2 = 1;
                                                }else if(novoUsuario->UserNode.tipo == 'C' || novoUsuario->UserNode.tipo == 'c'){
                                                    printf("\nUsu�rio ser� do tipo Cliente.\n");
                                                    verif2 = 0;
                                                }else if(novoUsuario->UserNode.tipo == 'S' || novoUsuario->UserNode.tipo == 's'){
                                                    printf("\nVoc� n�o tem permiss�o para adicionar um Superusu�rio.\n");
                                                    verif2 = 1;
                                                }else{
                                                    printf("\nOp��o inv�lida. voc� s� pode escolher [C].\n");
                                                    verif2 = 1;
                                                }
                                            }
                                        } while(verif2);

                                        // Saber se o usu�rio estar� no arquivo.
                                        do{
                                            char r;
                                            printf("\nDeseja salvar o novo usu�rio no arquivo? [S/N].\nR: ");
                                            setbuf(stdin, NULL);
                                            scanf("%c", &r);
                                            if(r=='s' || r=='S'){
                                                printf("\nO usu�rio ser� salvo no arquivo.");
                                                novoUsuario->isOnFile = 1;
                                                addUserToFile(usersDataBase, novoUsuario);
                                            }else if(r=='n' || r=='N'){
                                                printf("\nO usu�rio n�o ser� salvo no arquivo.");
                                                novoUsuario->isOnFile = 0;
                                            }else{
                                                printf("\nOp��o inv�lida. escolha [S/N].\n");
                                                novoUsuario->isOnFile = -1;
                                            }
                                        }while(novoUsuario->isOnFile == -1);
                                        addUser(Lista, novoUsuario);
                                        proxTela();
                                    break;

                                    case 2:
                                        system("cls");
                                        printf("O=======================================O\n| Voc� escolheu [2] Remover um usu�rio. |\nO=======================================O\n");
                                        int Id, check;

                                        printf("Digite a Id do usu�rio que voc� quer remover.\nR: ");
                                        scanf("%d", &Id);
                                        
                                        check = removeUser(Lista, usersDataBase, atualUser, Id);
                                        if(check == -2){
                                            printf("\nVoc� n�o tem permiss�o para remover este usu�rio.");
                                        }else if(check == -1){
                                            printf("\nN�o � poss�vel remover o SuperUsu�rio.");
                                        }else if(check==0){
                                            printf("\nN�o foi poss�vel encontrar o usu�rio.");
                                        }else{
                                            printf("\nUsu�ruio removido com sucesso.");
                                        }

                                        proxTela();
                                    break;

                                    case 3:
                                        system("cls");
                                        printf("O===============================================O\n| Voc� escolheu [3] Pesquisar usu�rio por nome. |\nO===============================================O\n");
                                        char rNameUser[128];

                                        // Pegar o nome a ser pesquisado.
                                        printf("Digite o nome do usu�rio que voc� quer pesquisar.\nR: ");
                                        setbuf(stdin, NULL);
                                        fgets(rNameUser, 128, stdin);
                                        printf("\n");

                                        if(searchForName(Lista, rNameUser)==0){
                                            printf("N�o foi poss�vel encontrar o usu�rio.\n");
                                        }else{
                                            printf("Usu�rio(s) encontrado(s) com sucesso.\n");
                                        }
                                         // OBS: n�o ficou totalmente claro no enunciado se � para pesquisar o nome do usu�rio (Lista->inicio->UserNode.nome) ou o nome de usu�rio (Lista->inicio->UserNode.usuario), sendo assim, foi pesquisado o par�metro nome (Lista->inicio->UserNode.nome).
                                        
                                        proxTela();
                                    break;

                                    case 4:
                                        system("cls");
                                        printf("O==================================O\n| Voc� escolheu [4] Alterar senha. |\nO==================================O\n");
                                        char newPassword[64];

                                        printf("Digite a nova senha.\nR:");
                                        setbuf(stdin, NULL);
                                        fgets(newPassword, 64, stdin);

                                        if(changePassword(Lista, usersDataBase, atualUser, newPassword)==0){
                                            printf("\nN�o foi poss�vel alterar a senha.");
                                        }else{
                                            printf("\nSenha alterada com sucesso.");
                                        }

                                        proxTela();
                                    break;
                                    
                                    case 5:
                                        system("cls");
                                        printf("O=============================O\n| Voc� escolheu [5] deslogar. |\nO=============================O");

                                        proxTela();
                                    break;

                                    case 6: // Debug.
                                        system("cls");
                                        printf("Os usu�rios que est�o na lista:\t\t\tQuantidade de usu�rios: %d\n\n", Lista->cont);
                                        
                                        printList(Lista);
                                        proxTela();
                                    break;
                                    default:
                                        printf("\nOp��o inv�lida. escolha apenas entre 1 e 5.");
                                        proxTela();
                                    break;
                                }
                            } while(rOpc!=5);
                            break;
                        }
                        

                    }else if(verif == 1){
                        printf("\nUsu�rio correto, senha inv�lida.");
                        proxTela();
                    }else{
                        printf("\nN�o foi poss�vel encontrar o usu�rio.");
                        proxTela();
                    }
                break;

                case 2:
                    printf("\nVoc� escolheu sair do programa. tenha um bom dia!");

                    clearList(Lista); // � necess�rio apenas apagar a lista da mem�ria, j� que o que devia ser salvo j� foi salvo :D.

                    proxTela();
                break;

                case 3: // Debug.
                    system("cls");
                    printf("Os usu�rios que est�o na lista:\t\t\tQuantidade de usu�rios: %d\n\n", Lista->cont);
                    
                    printList(Lista);
                    proxTela();
                break;
            
                default:
                printf("\nOp��o inv�lida. tente escolher entre 1 e 2.");
                                        
                proxTela();
                break;
            }
        } while (rLog!=2);
    }
    return 0;
}