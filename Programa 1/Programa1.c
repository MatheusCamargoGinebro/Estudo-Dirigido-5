#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

/*
O==================================================================================O 
| Estudo dirigido 5 - Sistema de Cadastro Acadêmico com Lista Encadeada e Arquivos |
|                                   Programa 1                                     |
|    Nomes:                                                                        |
|        Matheus Camargo Ginebro CP3016153                                         |   
|        Lucas Haiter Leoni CP3017362                                              |
O==================================================================================O
*/


//+---------------------------------</> DEFININDO ESTRUTURAS </>---------------------------------+

//Estrutura de aluno.
typedef struct nodeData{
    char nome[128];
    char turma;
    float nota;
    float frequencia;
    int matricula;
}nodeData;

//Estrutura de nó (representa item da lista).
typedef struct node{
    // Struct que guarda os dados do aluno.
    nodeData aluno;

    // Variável para vefiricar se o node está salvo no arquivo.
    int isOnFile;

    // Ponteiro para o próximo nó.
    struct node *proximo; 
}node;


//Estrutura de lista encadeada.
typedef struct list{
    // Ponteiro para o primeiro node da lista.
    node *inicio;

    // Ponteiro para o último nó da lista
    node *fim;

    // Variável que guarda o tamanho da lista. (útil para debug).
    int cont;
}list;



//+---------------------------------</> DEFININDO FUNÇÕES </>---------------------------------+
// Funções de exibição e facilidades:

// Função para limpar a tela.
void proxTela(){
    printf("\n");
    system("pause");
    system("cls");
}

// Função para verificar se há lista ou se ela está vazia.
int checklist(list *lista){
    if(lista == NULL){ // Testa se a lista foi criada.
        return 0;
    }else if(lista->inicio == NULL){ // Testa se a lista está vazia.
        return 0;
    } else{
        return 1; // Indica lista já criada e com itens dentro.
    }
}
/* Gabarito do return:
    return 0 == Lista vazia ou não criada.
    return 1 == Lista já criada e com elementos.
*/

//Função que verifica se uma matrícula já foic cadastrada. (função obrigatória da atividade).
int verificarAluno(list *lista, nodeData aluno){
    for(node *Node = lista->inicio; Node != NULL; Node = Node->proximo){
        if(aluno.matricula == Node->aluno.matricula){
            return 1;
        }
    }
    return 0;
}
/* Gabarito do return:
    return 0 == matrícula já cadastrada.
    return 1 == matrícula disponível.
*/

// Função para imprimir um único node.
void printStudent(node *student){
    printf("Nome: %s", student->aluno.nome);
    printf("Matrícula: %d\n", student->aluno.matricula);
    printf("Turma: %c\n", student->aluno.turma);
    printf("Frequência: %.1f\n", student->aluno.frequencia);
    printf("Nota: %.1f\n", student->aluno.nota);
    printf("Salvo: %d\n\n", student->isOnFile);
}

// Função para imprimir uma lista inteira.
int printList(list *lista){
    int check = checklist(lista);

    switch (check){
        case 0:
            printf(" "); // Apenas para validar o case.
        return 0;
        
        default:
            for (node *I = lista->inicio; I != NULL; I = I->proximo){
                printStudent(I);
            }
        return 1;
    }
}
/* Gabarito do return:
    return 0 == Lista vazia.
    return 1 == Lista impressa.
*/

// Função para procurar aluno com determinada característica.
int findStudent(list *lista, node *student, int mode){
    int verif = 0;
    switch (mode)
    {
    case 1: // Modo nome.
        for(node *Node = lista->inicio; Node != NULL; Node = Node->proximo){
            if(strcmp(Node->aluno.nome, student->aluno.nome)==0){
                printStudent(Node);
                verif = 1;
            }
        }
    break;

    case 2: // Modo Turma.
        for(node *Node = lista->inicio; Node != NULL; Node = Node->proximo){
            if(Node->aluno.turma == student->aluno.turma){
                printStudent(Node);
                verif = 1;
            }
        }
    break;

    case 3: // Modo Matrícula.
        for(node *Node = lista->inicio; Node != NULL; Node = Node->proximo){
            if(Node->aluno.matricula == student->aluno.matricula){
                printStudent(Node);
                return 1; // Não há razão para continuar procurando por mais alunos com tal matrícula, já que não há matrículas repetidas.
            }
        }
    break;
    
    default: // Modo ERRO :D (foi por erro do programador que chegamos aqui).
        printf("ERROR: modo da função findStudent indefinido. adios.\n");
        proxTela();
        exit(1);
    break;
    }
    return verif;
}
/* Gabarito do return:
    return 0 == Aluno não encontrado.
    return 1 == Aluno encontrado.
*/



// Funções de modificação de lista/arquivo:

// Função para Criar a lista.
list *CreateList(){
    // Declara uma lista vazia dinâmicamente.
    list *lista = (list *)malloc(sizeof(list));

    // Verifica se a lista foi realmente criada.
    if (lista == NULL){ // Caso haja erro, filaizará o programa.
        printf("\nErro ao alocar memória para a lista.\n");
        exit(1);
    }
    else{ // Caso não haja erro, o programa inicializa os elementos da lista como NULL (indica lista vazia).
        lista->inicio = NULL;
        lista->fim = NULL;
        lista->cont = 0;

        return lista;
    }
}

// Função para Limpar a lista.
int clearList(list *lista){
    node *student;
    int check = checklist(lista);

    switch (check){
        case 0:
            printf(" "); // Apenas para validar o case.
        return 0;

        default:
            // Enquanto o inicio da lista apontar para um nó.
            while (lista->inicio != NULL){
                // Recupera o primeiro elemento da lista.
                student = lista->inicio;

                // Faz com que o próximo elemento seja o primeiro elemento da lista.
                lista->inicio = student->proximo;

                // Remove o nodo da lista.
                free(student);
            }
            // Libera a memória da variável que representa a lista.
            free(lista);
        return 1;
    }
}
/* Gabarito do return:
    return 0 == Lista já vazia.
    return 1 == Lista limpa.
*/

//Função para adicionar aluno à lista.
int addStudent(list *lista, node *newNode){
    // cria o node da lista dinâmincamente.
    node *NewUser = (node *)malloc(sizeof(node));

    // Verifica se ele foi criado com êxito.
    if(NewUser == NULL){
        printf("\nErro ao adicionar novo aluno.\n");
        proxTela();
        exit(1); // finaliza o programa retornando código de erro 1.
    }
    else{
        if(verificarAluno(lista, newNode->aluno) == 0){
            // Inserindo elemento no inicio da lista.
            NewUser = newNode;
            NewUser->proximo = lista->inicio;

            // Verificando se a lista está vazia.
            if (lista->inicio == NULL)
            { // Caso esteja vazia, o primeiro elemento também será o último.
                lista->fim = NewUser;
            }
            lista->inicio = NewUser;
            lista->cont++;
            return 1;
        }else{
            return 0;
        }
    }
}
/* Gabarito do return:
    retturn 0 == matrícula já cadastrada.
    return 1 == Aluno adicionado.
*/

int removeStudent(list *lista, FILE *fileToEdit, int auxMat){
    node *prev;
    int check = checklist(lista);

    switch (check){
        case 0:
            printf(" "); // Apenas para validar o case.
        return 0;
        
        default:
            // Percorrer a lista, enquando procura alguma matrícula igual à digitada pelo usuário.
            for(node *I = lista->inicio; I != NULL; prev=I, I=I->proximo){

                if(I->aluno.matricula == auxMat){ // Encontrou algum nó com a matrícula igual à digitada pelo usuário.
                    if(lista->inicio == lista->fim){ // Verifica se a lista possui apenas um Aluno (Nó).
                        // Inicializa a lista como vazia.
                        lista->inicio = NULL;
                        lista->fim = NULL;

                    }else{ // se a lista possuir mais de um aluno (Nó).

                        if(I == lista->inicio){ // Se o elemento está no início da lista
                            // O segundo elemento passa a ser o início da lista
                            lista->inicio = I->proximo;

                        }else if(I == lista->fim){ // Se o elemento está no fim da lista.
                            lista->fim = prev; // O final da lista passa apontar para o penúltimo nó.
                            prev->proximo = NULL;  // O penúltimo node passa a ser o último nó.

                        } else{// O Elemento está no meio da lista.
                            // O proximo do nó anterior passa a ser o proximo do nó encontrado.
                            prev->proximo = I->proximo;
                        }
                    }

                    //Verifica se o aluno está na lista.
                    if (I->isOnFile == 1){
                        //Cria uma struct auxíliar para ler os nós do arquivo e procurar o node a ser excluído.
                        node *student = (node *)malloc(sizeof(node));

                        fileToEdit = fopen("studentDatabase.bin", "r+b");

                        while (1){
                            // Salva a posição do node lido no arquivo, para caso seja o que estamos procurando.
                            int posicao = ftell(fileToEdit);
                            // Lê o node no arquivo.
                            fread(student, sizeof(node), 1, fileToEdit);

                            // Testa se já chegou ao final do arquivo.
                            if (feof(fileToEdit)){
                                break;
                            }
                            // Se o aluno lido no arquivo tiver a mesma matrícula digitada, então achamos o node a ser "removido" (pelo o que entendemos, não é possível remover algo da memória, apenas sobrescrever).
                            if (I->aluno.matricula == student->aluno.matricula){
                                // Define que o espaço ocupado no arquivo pelo node está disponível.
                                student->isOnFile = -1;
                                
                                // Volta para a posição onde o node estava.
                                
                                fseek(fileToEdit, posicao, SEEK_SET);

                                // Sobrescreve o node.
                                fwrite(student, sizeof(node), 1, fileToEdit);
                                // Termina operação de remover aluno do arquivo.
                                break;
                            }
                        }
                        fclose(fileToEdit);
                    }

                    //libera a memória ocupada pelo nó.
                    free(I);
                    lista->cont--;
                    //retorna 1 (true) indicando que encontrou e removeu o elemento da lista.
                    return 1;
                } // fim do if que encontrou o elemento.

            } //fim do for.

        //se chegou nesse ponto quer dizer que a lista foi inteiramente percorrida
        //e não foi encotrado o valor que queremos remover da lista (retorna 0 - false).
        return 0;  
    }
}
/* Gabarito do return:
    return 0 == Não encontrou o aluno.
    return 1 == Aluno removido com sucesso.
*/


//Função para Ordenar alunos.
int OrdenaAluno(list *lista){
    node *pi, *pj, *pend = NULL;
    int check = checklist(lista);

    switch (check){
    case 0:
    return 0;
    
    default:
        for(pi = lista->inicio; pi->proximo!=NULL; pi = pi->proximo){
            for(pj = lista->inicio; pj->proximo!= pend; pj = pj->proximo){
                if(strcmp(pj->aluno.nome, pj->proximo->aluno.nome)>0){
                    nodeData aux = pj->aluno;
                    pj->aluno = pj->proximo->aluno;
                    pj->proximo->aluno = aux;
                }
            }
        }
    }
    return 1;
}
/* Gabarito do return:
    return 0 == Lista vazia.
    return 1 == Lista ordenada.
*/

// Funções de modificação de arquivo:
int loadfile(list *lista, FILE *fileToLoad)
{
    while (1)
    {
        node *student = (node *)malloc(sizeof(node));
        fread(student, sizeof(node), 1, fileToLoad);

        if (feof(fileToLoad))
        {
            break;
        }
        if (student->isOnFile != -1)
        {
            addStudent(lista, student);
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
/* Gabarito do return:
    return 0 == Carregado com sucesso.
    return 1 == problema ao carregar a lista.
*/

// Função para adicionar um node ao arquivo.
void addStudenttoFile(FILE *fileToAdd, node *NewStudent){
fileToAdd = fopen("studentDatabase.bin", "r+b");
    while (1){
        node *aux = (node *)malloc(sizeof(node));
        int posicao = ftell(fileToAdd);
        fread(aux, sizeof(node), 1, fileToAdd);

        if (feof(fileToAdd) || aux->isOnFile == -1){
            if (aux->isOnFile == -1){
                fseek(fileToAdd, posicao, SEEK_SET);
            }

            fwrite(NewStudent, sizeof(node), 1, fileToAdd);
            break;
        }
    }
    fclose(fileToAdd);
}


//+---------------------------------</> FUNÇÃO MAIN </>---------------------------------+
int main()
{
    setlocale(LC_ALL, "portuguese");

    //Variáveis auxiliares.
    int verif, R;
    float HighNote, LowNote;
    char R2;
    node *rFind = (node*)malloc(sizeof(node)); // Ter ou não ter, não muda o funcionamento do programa, mas remove um waring, e é tudo que queremos.


    //Criando uma lista.
    list *lista = CreateList();

    // Tentando abrir o banco de dados de alunos.
    FILE *studentDatabase = fopen("studentDatabase.bin", "rb");

    // Caso não consiga, cria um novo banco de dados.
    if(studentDatabase == NULL){

        fclose(studentDatabase);
        studentDatabase = fopen("studentDatabase.bin", "wb");
        
        if(studentDatabase != NULL){
            printf("Nova lista criada com sucesso.\n");
            proxTela();
        }else{
            printf("Não foi possível criar lista.\n\nFinalizando o programa.\n");
            proxTela();
            exit(1);
        }

        fclose(studentDatabase);
    }else{ // Arquivo aberto com sucesso.
        verif = loadfile(lista, studentDatabase);
        if (verif==0){
            printf("Lista carregada com sucesso. pronto para começar.\n");
            proxTela();
        }else{
            printf("O arquivo estava vazio. (lista limpa). pronto para começar.\n");
            proxTela();
        }
    }

    do
    {
        //menu com as opções para que o usuário possa escolher.
        printf("O=================================================O\n|          SISTEMA DE CADASTRO ACADEMICO          |\nO=================================================O\n| [1] Cadastrar aluno no inicio da lista.         |\n| [2] Remover aluno.                              |\n| [3] Pesquisar aluno por nome.                   |\n| [4] Pesquisar aluno por matricula.              |\n| [5] Mostrar todos os alunos de uma turma.       |\n| [6] Ordenar toda a lista de alunos pelo nome.   |\n| [7] Mostrar os alunos com a maior e menor nota. |\n| [8] Gravar dados.                               |\n| [9] Sair.                                       |\nO=================================================O\n\nDigite o que quer fazer.\nR: ");
        scanf("%d", &R);

        switch (R){
        case 1:
            system("cls");
            printf("O=======================================================O\n| Você escolheu [1] Cadastrar aluno no inicio da lista. |\nO=======================================================O\n");
            node *aux = (node *)malloc(sizeof(node));

            // Pegar nome do aluno.
            printf("\nDigite o nome do novo aluno.\nR: ");
            setbuf(stdin, NULL);
            fgets(aux->aluno.nome, 128, stdin);

            // Pegar matrícula do aluno.
            do{
                printf("\nDigite a matrícula.\nR: ");
                scanf("%d", &aux->aluno.matricula);

                if(aux->aluno.matricula >= 0 && aux->aluno.matricula <= 99999){
                    verif = 0;
                    printf("\nApós a inserção de todos os dados, será verificado se a matrícula já foi cadastrada.\n");
                }else{
                    verif = 1;
                    printf("\nMatrícula inválida. escola um valor do intervalo 0 <= Matrícula <= 99999.\n");
                }
            }while(verif);


            // Pegar a nota do aluno.
            do{
                printf("\nDigite a nota do aluno.\nR: ");
                scanf("%f", &aux->aluno.nota);
                if(aux->aluno.nota>=0.0 && aux->aluno.nota<=10.0){
                    verif=0;
                }else{
                    printf("\nValor invalido. digite um valor do intervalo 0 <= Nota <= 10.\n");
                    verif=1;
                }
            }while(verif);

            // Pegar a frequência do aluno.
            do{
               printf("\nDigite a frequencia do aluno.\nR: ");
                    scanf("%f", &aux->aluno.frequencia);
                    if(aux->aluno.frequencia>=0.0 && aux->aluno.frequencia<=100.0){
                    verif=0;
                }else{
                    printf("\nValor invalido. digite um valor que esteja no intervalo 0 <= X <= 100.\n");
                    verif=1;
                }
            }while(verif);

            // Pegar turma do aluno.
            do{
                printf("\nDigite a turma do aluno [A/B].\nR: ");
                setbuf(stdin, NULL);
                scanf("%c", &aux->aluno.turma);

                if(aux->aluno.turma!= 'A' && aux->aluno.turma!='B'){
                    printf("\nTurma invalida. Escolha entre [A/B].\n");
                    verif=1;
                }else{
                    verif=0;
                }
            }while(verif);
            
            // Verificar se é para salvar no arquivo.
            do
            {
                printf("\nDeseja salvar o aluno no disco? [S/N].\nR: ");
                setbuf(stdin, NULL);
                scanf("%c", &R2);

                if(R2 == 's' || R2 == 'S'){
                    printf("\nO aluno será salvo no arquivo.\n");
                    aux->isOnFile = 1;
                    verif = 0;
                }else if( R2 == 'n' || R2 == 'N'){
                    printf("\nO aluno não será salvo no arquivo.\n");
                    aux->isOnFile = 0;
                     verif = 0;
                }else{
                    printf("\nOpção inválida. escolha [S/N].\n");
                    verif = 1;
                }
            } while (verif);
            
            //Adicionando o novo aluno à lista.
            verif = addStudent(lista, aux);
            if(verif==1){
                printf("\nUsuário adicionado à lista");
                if(aux->isOnFile==1){
                    addStudenttoFile(studentDatabase, aux);
                    printf(" e ao arquivo");
                }
                printf(".\n");
            }else{
                printf("\nNão foi possível adicionar o usuário à lista, pois a matrícula digitada já foi cadastrada.\n");
            }

            proxTela();
        break;
        
        case 2:
            system("cls");
            printf("O==================================O\n| Você escolheu [2] Remover aluno. |\nO==================================O\n");
            int rMat;

            printf("\nDigite a matrícula do aluno a ser excluído.\nR: ");
            scanf("%d", &rMat);

            verif = removeStudent(lista, studentDatabase, rMat);
            if(verif==1){
                printf("\nAluno removido com sucesso.\n");
            }else{
                printf("\nAluno não encontrado.\n");
            }

            proxTela();
        break;
        
        case 3:
            system("cls");
            printf("O=============================================O\n| Você escolheu [3] Pesquisar aluno por nome. |\nO=============================================O\n");

            printf("\nDigite o nome do aluno que você quer pesquisar.\nR: ");
            setbuf(stdin, NULL);
            fgets(rFind->aluno.nome, 128, stdin);

            printf("\nOs alunos encontrados:\n\n");
            if(findStudent(lista, rFind, 1) == 1){
                printf("Todos os alunos foram exibidos.\n");
            }else{
                printf("Não foi encontrado nenhum aluno com o nome %s", rFind->aluno.nome);
            }

            proxTela();
        break;
        
        case 4:
            system("cls");
            printf("O==================================================O\n| Você escolheu [4] Pesquisar aluno por matricula. |\nO==================================================O\n\n");

            printf("\nDigite a matrícula do aluno que você quer procurar.\nR: ");
            scanf("%d", &rFind->aluno.matricula);

            printf("\nO aluno encontrado:\n\n");
            if(findStudent(lista, rFind, 3) == 1){
                printf("Aluno dono da matrícula %d exibido.\n", rFind->aluno.matricula);
            }else{
                printf("Não foi encontrado nenhum aluno com a matrícula %d.\n", rFind->aluno.matricula);
            }

            proxTela();
        break;
        
        case 5:
            system("cls");
            printf("O=========================================================O\n| Você escolheu [5] Mostrar todos os alunos de uma turma. |\nO=========================================================O\n");
            
            do{
                printf("\nDigite a turma do aluno que você quer procurar.\nR: ");
                setbuf(stdin, NULL);
                scanf("%c", &rFind->aluno.turma);
                if(rFind->aluno.turma == 'A' || rFind->aluno.turma == 'B'){
                    verif = 0;
                }else if(rFind->aluno.turma == 'a'){
                    rFind->aluno.turma = 'A';
                    verif = 0;
                }else if(rFind->aluno.turma == 'b'){
                    rFind->aluno.turma = 'B';
                    verif = 0;
                }else{
                    printf("\nOpção inválida. escolha entre [A/B].\n");
                    verif = 1;
                }
            }while (verif);
            
            printf("\nOs alunos encontrados:\n\n");
            if(findStudent(lista, rFind, 2) == 1){
                printf("Todos os alunos da turma %c foram exibidos.\n", rFind->aluno.turma);
            }else{
                printf("Não foi encontrado nenhum aluno da turma %c.\n", rFind->aluno.turma);
            }
            
            proxTela();
        break;
        
        case 6:
            system("cls");
            printf("O=============================================================O\n| Você escolheu [6] Ordenar toda a lista de alunos pelo nome. |\nO=============================================================O\n");

            if(OrdenaAluno(lista)==0){
                printf("\nNao ha alunos cadastrados.\n");
            }else{
                printList(lista);
            }

            proxTela();
        break;
        
        case 7:
            system("cls");
            printf("O===============================================================O\n| Você escolheu [7] Mostrar os alunos com a maior e menor nota. |\nO===============================================================O\n");

            // Pegar a maior e a menor nota.
            for(node *Node = lista->inicio; Node != NULL; Node = Node->proximo){
                if (Node == lista->inicio){
                    LowNote = Node->aluno.nota;
                    HighNote = Node->aluno.nota;
                }else{
                    if(LowNote > Node->aluno.nota){
                        LowNote = Node->aluno.nota;
                    }
                    if(HighNote < Node->aluno.nota){
                        HighNote = Node->aluno.nota;
                    }
                }
            }

            //Mostrar os alunos com a menor nota.
            printf("A menor nota foi %.2f. Tirada por:\n\n", LowNote);
            for(node *Node= lista->inicio; Node != NULL; Node = Node->proximo){
                if(Node->aluno.nota == LowNote){
                    printStudent(Node);
                }
            }

            //Mostrar os alunos com a maior nota.
            printf("\n\nA maior nota foi %.2f. Tirada por:\n\n", HighNote);
            for(node *Node= lista->inicio; Node != NULL; Node = Node->proximo){
                if(Node->aluno.nota == HighNote){
                    printStudent(Node);
                }
            }

            proxTela();
        break;
        
        case 8:
            system("cls");
            printf("O=================================O\n| Você escolheu [8] Gravar dados. |\nO=================================O\n");

            studentDatabase = fopen("studentDatabase.bin", "r+b");

            // Salvando os usuários ainda não salvos.
            for(node *Node= lista->inicio; Node != NULL; Node = Node->proximo){
                if(Node->isOnFile == 0){
                    Node->isOnFile = 1;
                    addStudenttoFile(studentDatabase, Node);
                }
            }

            proxTela(); 
        break;
        
        case 9:
            printf("\nVocê escolheu sair do programa.\n");
            if(clearList(lista)==1){
                printf("\nLista limpa.\n");
            }else{
                printf("\nNão foi necessário limpar a lista, pois ela já estava vazia.\n");
            }
            proxTela();
        break;

        case 3016153: // Debug.
        system("cls");
            printf("A lista de alunos:\t\t\tQuantidade de alunos na lista: %d.\n\n", lista->cont);
            printList(lista);

            proxTela();
        break;
        
        default:
            printf("\nValor inválido. tente algo entre 1 e 9.\n");
            proxTela();
        break;
        }
    }while(R!=9);
    return 0;
}