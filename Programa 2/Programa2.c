#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
// Puxando o arquivo com as fun��es de HASH (Nota ao professor: caso queira ver como estava antes, � s� ir no commit n* 34);
#include "hash_function.c" /* puxa um arquivo em C, � �til para fazer uma "biblioteca pr�pria", pois as fun��es e vari�veis.
que est�o naquele arquivo podem ser usadas na fun��o main.*/

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

/*------------------------------< Structs >------------------------------*/

// Estrutura de endere�o.
typedef struct
{
    char rua[128];
    char bairro[128];
    int numero;
    int cep;
} address;

// Estrutura que guarda os dados do usu�rio (incluindo uma estrutura de endere�o).
typedef struct
{
    // Dados obrigat�rios da atividade
    int id;
    char nome[128];
    char usuario[64];
    char senha[65]; // Se fosse 50, como pedido no enunciado, ou at� mesmo 64 n�o haveria espa�o para por toda a chave e mais o \0.
    char tipo;
    address endereco;
} user;

// Estrutura de n� (node).
typedef struct NODE
{
    // Estrutura do usu�rio
    user UserNode;

    // vari�vel para verificar se o n� est� ou n�o no arquivo criado pelo programa.
    int isOnFile;

    // Ponteiro que aponta para o endere�o do pr�ximo n�.
    struct NODE *proximo;
} node;

// Estrutura de lista.
typedef struct
{
    node *inicio;
    node *fim;
    int cont;
} list;

/*------------------------------< Functions >------------------------------*/
/*-------------< Fun��es de exibi��o >-------------*/
// Fun��o de limpar a tela (� um system("cls"); s� que aprimorado).
void proxTela()
{
    printf("\n\n");
    system("pause");
    system("cls");
}

// Imprime um �nico node.
void printuser(node *userToPrint)
{
    printf("Usu�rio: %s", userToPrint->UserNode.usuario);
    printf("Nome: %s", userToPrint->UserNode.nome);
    printf("Tipo: %c\n", userToPrint->UserNode.tipo);
    printf("Id: %d\n", userToPrint->UserNode.id);
    printf("Rua: %s", userToPrint->UserNode.endereco.rua);
    printf("Bairro: %s", userToPrint->UserNode.endereco.bairro);
    printf("N�mero: %d\n", userToPrint->UserNode.endereco.numero);
    printf("CEP: %d\n", userToPrint->UserNode.endereco.cep);
    printf("Salvo no arquivo == %d.\n\n", userToPrint->isOnFile);
}

// Imprime uma lista inteira.
void printList(list *lista)
{
    for (node *I = lista->inicio; I != NULL; I = I->proximo)
    {
        printuser(I);
    }
}

/*-------------< Fun��es de modifica��o de lista/arquivo >-------------*/
// Fun��o para Criar a lista.
list *CreateList()
{
    // Declara uma lista vazia din�micamente.
    list *lista = (list *)malloc(sizeof(list));

    // Verifica se a lista foi realmente criada.
    if (lista == NULL)
    { // Caso haja erro, filaizar� o programa.
        printf("\nErro ao alocar mem�ria para a lista.\n");
        exit(1);
    }
    else
    { // Caso n�o haja erro, o programa inicializa os elementos da lista como NULL (indica lista vazia).
        lista->inicio = NULL;
        lista->fim = NULL;
        lista->cont = 0;

        return lista;
    }
}

// Fun��o para Limpar a lista.
void clearList(list *Lista)
{
    node *user;

    // Enquanto o inicio da lista apontar para um n�.
    while (Lista->inicio != NULL)
    {
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

// Fun��o para gerar um Id para um novo usu�rio.
int idGenerator(list *lista)
{
    // Declara uma vari�vel de verifica��o e um cont.
    int verif, cont = 0;
    // A fun��o testa para achar um valor que seja igual � algum Id.
    do
    {
        verif = 0;
        for (node *I = lista->inicio; I != NULL; I = I->proximo)
        {
            if (cont == I->UserNode.id)
            {
                verif = 1;
                break;
            }
        }
        if (verif == 1)
        { // Quando a fun��o encontra um Id igual ao cont, o cont � incrementado.
            cont++;
        }
    } while (verif != 0); // Enquanto a fun��o n�o encontrar um valor para cont, que seja diferente de todos os IDs, ela n�o para.

    return cont; // Quando a fun��o acha um valor para o novo Id, esse valor � retornado par a main.
}

// Fun��o para adicionar um usu�rio � lista. *(tamb�m serve para carregar usu�rios de um arquivo, trabalhando em conjunto com a fun��o loadToMemo)
void addUser(list *lista, node *usuarioNode)
{
    // cria o node da lista din�mincamente.
    node *NewUser = (node *)malloc(sizeof(node));

    // Verifica se o n� foi criado com �xito.
    if (NewUser == NULL)
    {
        printf("\nErro ao adicionar novo usu�rio.\n");
        proxTela();
        exit(1); // finaliza o programa retornando c�digo de erro 1.
    }
    else
    {
        // Inserindo elemento no inicio da lista.
        NewUser = usuarioNode;
        NewUser->proximo = lista->inicio;

        // Verificando se a lista est� vazia.
        if (lista->inicio == NULL)
        { // Caso esteja vazia, o primeiro elemento tamb�m ser� o �ltimo.
            lista->fim = NewUser;
        }
        lista->inicio = NewUser;
        lista->cont++;
    }
}

// Fun��o para remover da lista. (e dependendo do caso, do arquivo tamb�m).
int removeUser(list *Lista, FILE *fileToEdit, user *Removedor, int Id)
{
    node *anterior;
    for (node *I = Lista->inicio; I != NULL; anterior = I, I = I->proximo)
    { // N�o h� necessidade de testar se a lista est� vazia, j� que sempre haver� ao menos um elemento dentro dela.
        if (Id == I->UserNode.id)
        { // Achou um usu�rio com o ID parecido.
            if (I->UserNode.tipo == 'S')
            { // Se esse usu�rio for o SuperUsu�rio, a fun��o retorna -1, n�o permitindo a remo��o.
                return -1;
            }
            else
            {
                if (Removedor->tipo == 'S' || (Removedor->tipo == 'A' && I->UserNode.tipo == 'C'))
                { // Indica permiss�o para remover.
                    if (I == Lista->inicio)
                    { // Se o elemento est� no in�cio da lista.
                        // O segundo elemento passa a ser o in�cio da lista.
                        Lista->inicio = I->proximo;
                    }
                    else
                    { // O Elemento est� no meio da lista.
                        // O proximo do n� anterior passa a ser o proximo do n� encontrado;
                        anterior->proximo = I->proximo;
                    }

                    // Testando se o elemento est� salvo no arquivo.
                    if (I->isOnFile == 1)
                    {
                        node *uRemFil = (node *)malloc(sizeof(node));

                        fileToEdit = fopen("userDataBase.bin", "r+b");

                        while (1)
                        {
                            int posicao = ftell(fileToEdit);
                            fread(uRemFil, sizeof(node), 1, fileToEdit);
                            if (feof(fileToEdit))
                            {
                                break;
                            }
                            if (I->UserNode.id == uRemFil->UserNode.id)
                            {
                                uRemFil->isOnFile = -1; // O item IsOnfile define se o arquivo est� ou n�o no arquivo.
                                // Caso ele esteja, seu valor � 1. ------ Quando removido, seu valor passa a ser -1, indicando que o espa�o que ele ocupa pode ser reaproveitado. Foi feito desta maneira, pois aparentemente voc� n�o pode simplesmente "remover" algo da mem�ria, mas pode "sobrescrever".

                                fseek(fileToEdit, posicao, SEEK_SET);
                                fwrite(uRemFil, sizeof(node), 1, fileToEdit);
                                break;
                            }
                        }
                        fclose(fileToEdit);
                    }
                    // libera a mem�ria ocupada pelo n�.
                    free(I);
                    Lista->cont--;
                    // retorna 1 (true) indicando que encontrou e removeu o elemento da lista
                    return 1;
                }
                else if (Removedor->tipo == 'A' && I->UserNode.tipo == 'A')
                { // N�o tem permiss�o para remover.
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

// Fun��o para carregar os dados de um arquivo para a mem�ria.
int loadListToMemo(list *lista, FILE *fileToLoad)
{
    while (1)
    {
        node *getUser = (node *)malloc(sizeof(node));
        fread(getUser, sizeof(node), 1, fileToLoad);

        if (feof(fileToLoad))
        {
            break;
        }
        if (getUser->isOnFile != -1)
        {

            addUser(lista, getUser);
        }
    }
    if (lista->inicio != NULL)
    {
        return 0; // Carregado com sucesso.
    }
    else
    {
        return 1; // Deu algum problema.
    }
}

// Fun��o para salvar um usu�rio no arquivo.
void addUserToFile(FILE *fileToAdd, node *NewUser)
{
    fileToAdd = fopen("userDataBase.bin", "r+b");
    while (1)
    {
        node *getUser = (node *)malloc(sizeof(node));
        int posicao = ftell(fileToAdd);
        fread(getUser, sizeof(node), 1, fileToAdd);

        if (feof(fileToAdd) || getUser->isOnFile == -1)
        {
            if (getUser->isOnFile == -1)
            {
                fseek(fileToAdd, posicao, SEEK_SET);
            }
            fwrite(NewUser, sizeof(node), 1, fileToAdd);
            break;
        }
    }
    fclose(fileToAdd);
}

/*-------------< Fun��es de verifica��o e etc >-------------*/
// Fun��o para verificar os par�metros de senha e de node de usu�rio. (dependendo da situa��o, pode-se testar apenas um dos itens (se quiser)).
int verifLogin(list *Lista, user *atualUser)
{
    int verif = -1;
    for (node *I = Lista->inicio; I != NULL; I = I->proximo)
    {
        if (strcmp(atualUser->usuario, I->UserNode.usuario) == 0)
        {
            verif = 1; // Usu�rio verificado
            if (strcmp(atualUser->senha, I->UserNode.senha) == 0)
            {
                verif = 0; // Senha verificada
                *atualUser = I->UserNode;
            }
        }
    }
    return verif;
}
/*
    Gabarito do return (verif):
    return 1 == Usu�rio encontrado, senha inv�lida.
    return 0 == usu�rio e senha corretos.
    return -1 == n�o foi encontrado usu�rio.
*/

// Fun��o para procurar um usu�rio pelo nome.
int searchForName(list *Lista, char nameToCheck[])
{
    int verif = 0;
    for (node *I = Lista->inicio; I != NULL; I = I->proximo)
    {
        if (strcmp(I->UserNode.nome, nameToCheck) == 0)
        {
            printuser(I);
            verif = 1;
        }
    }
    return verif;
}

// Fun��o para trocar a senha.
int changePassword(list *Lista, FILE *fileToEdit, user *atualUser, char newPassword[])
{
    int verif = 0;
    for (node *I = Lista->inicio; I != NULL; I = I->proximo)
    {
        if (strcmp(atualUser->usuario, I->UserNode.usuario) == 0)
        {
            strcpy(I->UserNode.senha, newPassword);
            strcpy(atualUser->senha, newPassword);

            if (I->isOnFile == 1)
            {
                fileToEdit = fopen("userDataBase.bin", "r+b");
                while (1)
                {
                    int posicao = ftell(fileToEdit);
                    node *userToEdit = (node *)malloc(sizeof(node));
                    fread(userToEdit, sizeof(node), 1, fileToEdit);

                    if (feof(fileToEdit))
                    {
                        break;
                    }
                    if (strcmp(atualUser->usuario, I->UserNode.usuario) == 0)
                    {
                        strcpy(userToEdit->UserNode.senha, newPassword);
                        fseek(fileToEdit, posicao, SEEK_SET);
                        fwrite(userToEdit, sizeof(node), 1, fileToEdit);
                        break;
                    }
                }
                fclose(fileToEdit);
            }
            verif = 1;
            break;
        }
    }
    return verif;
}

/*------------------------------< Main >------------------------------*/
int main()
{
    setlocale(LC_ALL, "portuguese");

    // Iniciando arquvo e lista.
    list *Lista = CreateList();

    FILE *usersDataBase = fopen("userDataBase.bin", "rb");
    int verif2;

    if (usersDataBase == NULL)
    { // N�o foi poss�vel abrir o arquivo. (tentar� criar outro arquivo).
        fclose(usersDataBase);
        usersDataBase = fopen("userDataBase.bin", "wb");

        // Salva a lista.
        fclose(usersDataBase);
        if (usersDataBase != NULL)
        { // Arquivo criado com sucesso.
            node *superUser = (node *)malloc(sizeof(node));
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

            // Dar a senha (criptografada) para o login do *SUPERUSU�RIO*.
            // Definindo uma vari�vel tempor�ria para pegar a senha.
            char *TempPassW = (char *)malloc(64 * sizeof(char));
            // Pegando a senha
            printf("\nInsira a Senha do *SUPERUSU�RIO* (par�metro para fazer o login).\nR: ");
            setbuf(stdin, NULL);
            fgets(TempPassW, 64, stdin);

            // Fazendo o hashing da senha.
            TempPassW = H(TempPassW);

            // Passando a senha (com o HASH j� executado) para o node.
            strcpy(superUser->UserNode.senha, TempPassW);

            /* Limpando a vari�vel tempor�ria para garantir mais seguran�a.
            (n�o somos peritos em seguran�a da computa��o/hacking, ent�o n�o sabemos se realmente
            funcionaria isso... Mas t� a�, por desencargo de conci�ncia).*/
            free(TempPassW);

            // Definir o tipo de usu�rio (no caso, � fixo em *SUPERUSU�RIO*).
            printf("\nO *SUPERUSU�RIO* recebe o tipo S (*SUPERUSU�RIO*).");
            superUser->UserNode.tipo = 'S';

            // Definindo que o *SUPERUSU�RIO* estar� no arquivo.
            superUser->isOnFile = 1;

            proxTela();
            // Insere o *SUPERUSU�RIO* na lista.
            addUserToFile(usersDataBase, superUser);
            // Libera a mem�ria usada para guardar os dados do *SUPERUSU�RIO*.
            free(superUser);
            // Tenta novamente abrir o arquivo em modo leitura, para que seja poss�vel carregar o superusu�rio na lista.
            usersDataBase = fopen("userDataBase.bin", "rb");

            if (usersDataBase == NULL)
            {
                printf("O Super usu�rio foi salvo, por�m n�o foi poss�vel abrir o arquivo em modo leitura.\n");
                proxTela();
                exit(1);
            }
        }
        else
        {
            printf("\nERRO: n�o foi poss�vel criar/carregar o arquivo. adios.\n");
            proxTela();
            exit(1);
        }
    }

    // Carregando dados dos usu�rios para a lista.
    if (loadListToMemo(Lista, usersDataBase) != 0)
    {
        printf("ERROR: N�o foi poss�vel carregar os n�s do arquivo para a mem�ria. O programa ser� finalizado.\n");
        proxTela();
        exit(1);
    }
    else
    {
        printf("Os n�s salvos no arquivo foram carregados com sucesso!\n");
        fclose(usersDataBase); // Fechando/salvando o arquivo ap�s carreg�-lo para a mem�ria.
        proxTela();

        int rLog; // Vari�vel de resposta para login.

        do
        {
            printf("O=============================O\n| Sistema de Login e arquivos |\nO=============================O\n|     O que deseja fazer?     |\n|          [1] Login.         |\n|          [2] Sair.          |\nO=============================O\n\nR: ");
            scanf("%d", &rLog);

            switch (rLog)
            {
            case 1:
                system("cls");
                // Reservando um espa�o na mem�ria para o usu�rio atual, de forma din�mica.
                user *atualUser = (user *)malloc(1 * sizeof(user));
                int verif;

                printf("O===========================O\n| Voc� escolheu fazer login |\nO===========================O\n\n");

                // Pegando o nome do usu�rio.
                printf("Digite seu nome de usu�rio.\nR: ");
                setbuf(stdin, NULL);
                fgets(atualUser->usuario, 64, stdin);

                // Pegando a senha do usu�rio.
                // Definindo uma string tempor�ria para a pegar a senha do usu�rio.
                char *TempPassW = (char *)malloc(64 * sizeof(char));

                printf("\nDigite sua senha.\nR: ");
                setbuf(stdin, NULL);
                fgets(TempPassW, 64, stdin);

                // Fazendo o HASH dessa senha.
                TempPassW = H(TempPassW);

                // Passando a senha com HASH para a estrutura que ajuda na verifica��o de login.
                strcpy(atualUser->senha, TempPassW);

                // Limpando a string tempor�ria.
                free(TempPassW);

                // Passando o resultado da fun��o para uma vari�vel de verifica��o.
                verif = verifLogin(Lista, atualUser);

                if (verif == 0)
                {
                    printf("\nUsu�rio e senha corretos.\n\nlogado com sucesso!\n\nVoc� logou como %s", atualUser->usuario);
                    proxTela();
                    int rOpc;

                    switch (atualUser->tipo)
                    {
                    case 'C': // Caso o usu�rio seja um cliente.
                        do
                        {
                            system("cls");
                            printf("O======================================O\t\t\tInforma��es do usu�rio logado:\n|     Sistema de usu�rios - Logado     |\t\t\tNome: %sO======================================O\t\t\tUsu�rio: %s|    [1] Alterar senha.                |\t\t\tTipo: %c\n|    [2] Sair.                         |\nO======================================O\n\nR: ", atualUser->nome, atualUser->usuario, atualUser->tipo);
                            scanf("%d", &rOpc);

                            switch (rOpc)
                            {
                            case 1:
                                system("cls");
                                printf("O====================================O\n| Voc� escolheu [1] alretar a senha. |\nO====================================O\n");

                                // Declarando uma string tempor�ria para pegar a nova senha digitada pelo usu�rio.
                                char *TempNewPassW = (char *)malloc(64 * sizeof(char));

                                // Pegando a nova senha
                                printf("\nDigite a sua nova senha.\nR: ");
                                setbuf(stdin, NULL);
                                fgets(TempNewPassW, 64, stdin);

                                // Fazendo o HASH dessa senha.
                                TempNewPassW = H(TempNewPassW);

                                // Passando a senha com HASH para a estrutura que ajuda na verifica��o de login.
                                strcpy(atualUser->senha, TempNewPassW);

                                if (changePassword(Lista, usersDataBase, atualUser, TempNewPassW) == 0)
                                {
                                    printf("\nN�o foi poss�vel alterar a senha.");
                                }
                                else
                                {
                                    printf("\nSenha alterada com sucesso.");
                                }
                                free(TempNewPassW);
                                proxTela();
                                break;

                            case 2:
                                system("cls");
                                printf("O=============================O\n| Voc� escolheu [2] deslogar. |\nO=============================O");

                                // N�o � necess�rio salvar nada, j� que o que deveria ser salvo j� foi salvo.

                                proxTela();
                                break;

                            case 598758: // Debug.
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
                        } while (rOpc != 2);
                        break;

                    default: // Caso o usu�rio seja o *SUPERUSU�RIO* ou um Administrador.
                        do
                        {
                            system("cls");
                            printf("O======================================O\t\t\tInforma��es do usu�rio logado:\n|     Sistema de usu�rios - Logado     |\t\t\tNome: %sO======================================O\t\t\tUsu�rio: %s|    [1] Cadastrar usu�rio.            |\t\t\tTipo: %c\n|    [2] Remover usu�rio.              |\n|    [3] Pesquisar usu�rio por nome.   |\n|    [4] Alterar senha.                |\n|    [5] Sair.                         |\nO======================================O\n\nR: ", atualUser->nome, atualUser->usuario, atualUser->tipo);
                            scanf("%d", &rOpc);

                            switch (rOpc)
                            {
                            case 1:
                                system("cls");
                                printf("O======================================O\n| Voc� escolheu [1] Cadastrar usu�rio. |\nO======================================O\n");

                                node *novoUsuario = (node *)malloc(sizeof(node));

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
                                do
                                {
                                    printf("\nDigite o nick do usu�rio (par�metro de login).\nR: ");
                                    setbuf(stdin, NULL);
                                    fgets(novoUsuario->UserNode.usuario, 64, stdin);

                                    verif2 = verifLogin(Lista, novoUsuario);
                                    if (verif2 == 1 || verif2 == 0)
                                    {
                                        printf("\nUsu�rio j� cadastrado. tente outro nick.\n");
                                    }
                                } while (verif2 != -1);

                                // Senha do usu�rio.
                                // String de senha tempor�ria.
                                char *TempPassW = (char *)malloc(64 * sizeof(char));
                                printf("\nDigite a senha do usu�rio que voc� quer adicionar.\nR: ");
                                setbuf(stdin, NULL);
                                fgets(TempPassW, 64, stdin);

                                // Fazendo o HASH da senha.
                                TempPassW = H(TempPassW);

                                // Passando a senha j� criptografada para o novo node.
                                strcpy(novoUsuario->UserNode.senha, TempPassW);

                                // Limpando a String tempor�ria.
                                free(TempPassW);

                                // Tipo de usu�rio.
                                do
                                {
                                    printf("\nDigite o tipo de usu�rio.\nR: ");
                                    setbuf(stdin, NULL);
                                    scanf("%c", &novoUsuario->UserNode.tipo);

                                    if (atualUser->tipo == 'S')
                                    {
                                        if (novoUsuario->UserNode.tipo == 'A' || novoUsuario->UserNode.tipo == 'a')
                                        {
                                            printf("\nUsu�rio ser� do tipo Administrador.\n");
                                            verif2 = 0;
                                        }
                                        else if (novoUsuario->UserNode.tipo == 'C' || novoUsuario->UserNode.tipo == 'c')
                                        {
                                            printf("\nUsu�rio ser� do tipo Cliente.\n");
                                            verif2 = 0;
                                        }
                                        else if (novoUsuario->UserNode.tipo == 'S' || novoUsuario->UserNode.tipo == 's')
                                        {
                                            printf("\nEste programa � pequeno demais para 2 *SUPERUSU�RIOS*, parceiro. op��o inv�lida.\n");
                                            verif2 = 1;
                                        }
                                        else
                                        {
                                            printf("\nOp��o inv�lida. escolha [A/C].\n");
                                            verif2 = 1;
                                        }
                                    }
                                    else if (atualUser->tipo == 'A')
                                    {
                                        if (novoUsuario->UserNode.tipo == 'A' || novoUsuario->UserNode.tipo == 'a')
                                        {
                                            printf("\nVoc� n�o tem permiss�o para adicionar um Administrador.\n");
                                            verif2 = 1;
                                        }
                                        else if (novoUsuario->UserNode.tipo == 'C' || novoUsuario->UserNode.tipo == 'c')
                                        {
                                            printf("\nUsu�rio ser� do tipo Cliente.\n");
                                            verif2 = 0;
                                        }
                                        else if (novoUsuario->UserNode.tipo == 'S' || novoUsuario->UserNode.tipo == 's')
                                        {
                                            printf("\nVoc� n�o tem permiss�o para adicionar um Superusu�rio.\n");
                                            verif2 = 1;
                                        }
                                        else
                                        {
                                            printf("\nOp��o inv�lida. voc� s� pode escolher [C].\n");
                                            verif2 = 1;
                                        }
                                    }
                                } while (verif2);

                                // Saber se o usu�rio estar� no arquivo.
                                do
                                {
                                    char r;
                                    printf("\nDeseja salvar o novo usu�rio no arquivo? [S/N].\nR: ");
                                    setbuf(stdin, NULL);
                                    scanf("%c", &r);
                                    if (r == 's' || r == 'S')
                                    {
                                        printf("\nO usu�rio ser� salvo no arquivo.");
                                        novoUsuario->isOnFile = 1;
                                        addUserToFile(usersDataBase, novoUsuario);
                                        verif2 = 0;
                                    }
                                    else if (r == 'n' || r == 'N')
                                    {
                                        printf("\nO usu�rio n�o ser� salvo no arquivo.");
                                        novoUsuario->isOnFile = 0;
                                        verif2 = 0;
                                    }
                                    else
                                    {
                                        printf("\nOp��o inv�lida. escolha [S/N].\n");
                                        verif2 = 1;
                                    }
                                } while (verif2);
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
                                if (check == -2)
                                {
                                    printf("\nVoc� n�o tem permiss�o para remover este usu�rio.");
                                }
                                else if (check == -1)
                                {
                                    printf("\nN�o � poss�vel remover o SuperUsu�rio.");
                                }
                                else if (check == 0)
                                {
                                    printf("\nN�o foi poss�vel encontrar o usu�rio.");
                                }
                                else
                                {
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

                                if (searchForName(Lista, rNameUser) == 0)
                                {
                                    printf("N�o foi poss�vel encontrar o usu�rio.\n");
                                }
                                else
                                {
                                    printf("Usu�rio(s) encontrado(s) com sucesso.\n");
                                }
                                // OBS: n�o ficou totalmente claro no enunciado se � para pesquisar o nome do usu�rio (Lista->inicio->UserNode.nome) ou o nome de usu�rio (Lista->inicio->UserNode.usuario), sendo assim, foi pesquisado o par�metro nome (Lista->inicio->UserNode.nome).

                                proxTela();
                                break;

                            case 4:
                                system("cls");
                                printf("O==================================O\n| Voc� escolheu [4] Alterar senha. |\nO==================================O\n");
                                // Declarando uma string tempor�ria para pegar a nova senha digitada pelo usu�rio.
                                char *TempNewPassW = (char *)malloc(64 * sizeof(char));

                                // Pegando a nova senha
                                printf("\nDigite a sua nova senha.\nR: ");
                                setbuf(stdin, NULL);
                                fgets(TempNewPassW, 64, stdin);

                                // Fazendo o HASH dessa senha.
                                TempNewPassW = H(TempNewPassW);

                                // Passando a senha com HASH para a estrutura que ajuda na verifica��o de login.
                                strcpy(atualUser->senha, TempNewPassW);

                                if (changePassword(Lista, usersDataBase, atualUser, TempNewPassW) == 0)
                                {
                                    printf("\nN�o foi poss�vel alterar a senha.");
                                }
                                else
                                {
                                    printf("\nSenha alterada com sucesso.");
                                }
                                // Limpando a string tempor�ria.
                                free(TempNewPassW);
                                proxTela();
                                break;

                            case 5:
                                system("cls");
                                printf("O=========================O\n| Voc� escolheu [5] sair. |\nO=========================O");
                                verif2 = 0;
                                char R2;

                                for (node *Node = Lista->inicio; Node != NULL; Node = Node->proximo)
                                {
                                    if (Node->isOnFile == 0)
                                    {
                                        verif2 = 1;
                                        break;
                                    }
                                }
                                proxTela();

                                if (verif2 == 1)
                                {
                                    do
                                    {
                                        system("cls");
                                        printf("H� usu�rios na lista que ainda n�o foram salvos.\n\ndeseja fazer uma �ltima altera��o? [S/N] (os usu�rios n�o salvos ser�o perdidos).\nR: ");
                                        setbuf(stdin, NULL);
                                        scanf("%c", &R2);

                                        if (R2 == 'S' || R2 == 's')
                                        {
                                            printf("\nAgora, ser� exibido usu�rio por usu�rio, e voc� decidir� o que acontece com eles.");
                                            verif2 = 0;
                                        }
                                        else if (R2 == 'N' || R2 == 'n')
                                        {
                                            printf("\nOs usu�rios n�o salvos ser�o perdidos. fazendo logout.");
                                            verif2 = -1;
                                        }
                                        else
                                        {
                                            printf("\nOp��o inv�lida. Escolha entre [S/N].");
                                            verif2 = 1;
                                        }
                                        proxTela();
                                    } while (verif2 == 1);

                                    if (verif2 == 0)
                                    {
                                        for (node *Node = Lista->inicio; Node != NULL; Node = Node->proximo)
                                        {
                                            if (Node->isOnFile == 0)
                                            {
                                                printf("Informa��es do usu�rio:\n\n");
                                                printuser(Node);

                                                do
                                                {
                                                    printf("Deseja salvar Este usu�rio? S = Sim. N = N�o. E = Sair sem salvar nada.\nR: ");
                                                    setbuf(stdin, NULL);
                                                    scanf("%c", &R2);
                                                    if (R2 == 's' || R2 == 'S')
                                                    {
                                                        printf("\nO usu�rio ser� salvo no arquivo.");
                                                        Node->isOnFile = 1;
                                                        addUserToFile(usersDataBase, Node);
                                                        verif2 = 0;
                                                    }
                                                    else if (R2 == 'n' || R2 == 'N')
                                                    {
                                                        printf("\nO usu�rio n�o ser� salvo no arquivo.");
                                                        verif2 = 0;
                                                    }
                                                    else if (R2 == 'e' || R2 == 'E')
                                                    {
                                                        printf("\nOs usu�rios n�o salvos ser�o perdidos.\n\nfazendo logout.");
                                                        verif2 = 0;
                                                    }
                                                    else
                                                    {
                                                        printf("\nOp��o inv�lida. escolha entre [S/N/E].");
                                                        verif2 = 1;
                                                    }
                                                } while (verif2);

                                                if (R2 == 'e' || R2 == 'E')
                                                {
                                                    proxTela();
                                                    break;
                                                }
                                                proxTela();
                                            }
                                        }
                                    }
                                }
                                break;

                            case 598758: // Debug.
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
                        } while (rOpc != 5);
                        break;
                    }
                }
                else if (verif == 1)
                {
                    printf("\nUsu�rio correto, senha inv�lida.");
                    proxTela();
                }
                else
                {
                    printf("\nN�o foi poss�vel encontrar o usu�rio.");
                    proxTela();
                }
                break;

            case 2:
                printf("\nVoc� escolheu sair do programa. tenha um bom dia!");

                clearList(Lista); // � necess�rio apenas apagar a lista da mem�ria, j� que o que devia ser salvo j� foi salvo :D.

                proxTela();
                break;

            case 598758: // Debug.
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
        } while (rLog != 2);
    }
    return 0;
}