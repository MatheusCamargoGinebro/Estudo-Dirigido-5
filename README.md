# Estudo dirigido 5
Repositório do estudo dirigido 5 - Matéria de Lógica e técnicas de programação.


  Este repositório serve para guardar o estudo dirigido 5, que é uma atividade da disciplina de ILG - Lógica e Técnicas de Programação, 
na qual os alunos do Instituto Federal de Educação, Ciência e Tecnologia de São Paulo - IFSP Câmpus Campinas precisam fazer um determinado programa,
ou determinada pesquisa.

  As "especificações" da atividade estão listadas abaixo.


Programa 1.
  > O objetivo da primeira parte deste estudo dirigido é garantir a persistência dos dados que
foram produzidos durante o estudo dirigido 4. Para isso, é importante que o usuário possa
gravar os dados gerados em um arquivo binário sempre que necessário.


    Sistema de Cadastro Acadêmico Utilizando Lista Ligada Simples:

    Faça um programa de sistema acadêmico que gerencie o desempenho dos alunos da disciplina de 
    Lógica e Técnicas de Programação.
    
    O programa deve conter um registro (estrutura/struct) que consiga armazenar os seguintes 
    dados de um aluno:
    
    nome;
    matrícula;
    nota;
    frequência; 
    turma;
    
   > O programa deve permitir o recebimento desses dados para vários alunos. Seu programa deve armazenar os alunos em uma Lista Ligada
    Simples, como visto na aula. O programa deverá conter um menu com as seguintes opções:
    
        1. Cadastrar aluno no início da lista;
        2. Remover aluno;
        3. Pesquisar aluno por nome;
        4. Pesquisar aluno por matrícula;
        5. Mostrar todos os alunos de uma turma;
        6. Ordenar toda a lista de alunos pelo nome;
        7. Mostrar os alunos com a maior e menor nota;
        8. Gravar dados;
        9. Sair;
    
  > A seguir são dados mais detalhes sobre o funcionamento de cada opção do menu:


      Opção 1 - Cadastrar aluno:
        >-> O usuário cadastra todos os dados de um aluno (nome, matrícula, nota, frequência e turma)
        em uma nova struct aluno (que será um novo nó da lista).

        >-> A inserção do aluno deve acontecer sempre no início da Lista Ligada Simples.

        >-> O programa não deve aceitar alunos com o mesmo número de matrícula, ou seja, é preciso 
        enviar uma mensagem ao usuário (ESSE NÚMERO DE MATRÍCULA JÁ FOI CADASTRADO) caso já exista 
        um aluno cadastrado com o mesmo número de matrícula.

        >-> Sobre os valores aceitos:
            --> Para a “frequência” deverão ser aceitos valores entre 0 (0% de frequência) e 100 
            (100% de frequência).
            
            --> Para a “nota” deverão ser aceitos valores entre 0 e 10.
            --> Para a “matrícula” deverão ser aceitos valores inteiros entre 0 e 99999.
            --> Para a “turma” deverão ser aceitos apenas dois valores: “Turma A” ou “Turma B”.

            ***Qualquer valor fora das faixas especificadas anteriormente deve ser tratado
            adequadamente. Sendo que em nenhuma hipótese o programa deve ser encerrado.***

        >-> Ao final do cadastro do novo aluno (novo nó na lista) é preciso perguntar ao usuário 
        se ele deseja salvar o aluno no disco com a seguinte mensagem: “Você deseja salvar o aluno
        no disco ? (S)im ou (N)ão ?”. Caso o usuário pressione S o novo aluno (nó da lista) é 
        inserido no arquivo. Caso o usuário pressione N, o novo aluno é mantido na lista, porém 
        não é salvo no arquivo.

  >  

     Opção 2 - Remover aluno:
        >-> O usuário deverá excluir determinado aluno utilizando para isso o seu número de 
        matrícula e exibir a mensagem: “Aluno removido com sucesso!”.
        
        >-> Todas as operações necessárias para a realização da remoção devem ser realizadas,
        buscando garantir a integridade da Lista Ligada.
        
        >-> Caso não exista um aluno com a matrícula informada, o programa deve exibir a 
        mensagem: "Não foi possível fazer a remoção, aluno não encontrado!".
        
        >-> Caso o aluno seja removido da lista (remoção com sucesso), é necessário verificar
        se o mesmo também existe no arquivo (porque foi inserido anteriormente). 
            Assim, caso o aluno removido da lista esteja salvo também no arquivo, é preciso
            que a remoção garanta que o aluno também seja excluído do arquivo.

  >  

     Opção 3 - Pesquisar aluno por nome:
        >-> O usuário deve digitar o nome de um aluno e o programa deve mostrar todas as 
        informações daquele aluno ou dos alunos que possuam o nome informado.

        >-> Caso esse aluno não esteja cadastrado (não exista um aluno com o nome informado
        o programa deve exibir a mensagem: "Aluno não encontrado!".

  >  

     Opção 4 - Pesquisar aluno por matrícula:

        >-> O usuário deve digitar a matrícula de um aluno e o programa deve mostrar todas
        as informações do aluno que possua a matrícula informada.

        >-> Caso esse aluno não esteja cadastrado (não exista um aluno com a matrículainformada)
        o programa deve exibir a mensagem: "Aluno não encontrado!".

  >  

     Opção 5 - Mostrar todos os alunos de uma turma:

        >-> O usuário deve digitar uma turma e o programa deve mostrar as informações de todosos
        alunos que pertençam à turma informada.

        >-> Caso não exista pelo menos um aluno na turma informada, o programa deve exibir a
        mensagem: "A turma não possui alunos matriculados!".

  >  

     Opção 6 - Ordenar toda a lista de alunos pelo nome:

        >-> O programa deve mostrar as informações de todos os alunos cadastrados logo após a 
        ordenação.

        >-> Os nós da lista devem ser alterados para que fiquem em ordem alfabética.

  >  

     Opção 7 - Mostrar os alunos com a maior e menor nota:

        >-> O programa deve mostrar quais alunos tiveram a maior e a menor nota;

        >-> Caso haja empate (dois ou mais alunos com a maior ou menor nota) devem ser mostrados
        os dados de todos os alunos queempataram, ou seja, todos os alunos que tiraram a menor 
        ou maior nota.

  >  

     Opção 8 - Gravar dados:

        >-> Nesta opção o programa deve gravar no arquivo todos os dados que foram inseridos na lista
        ligada e que ainda não estão gravados no arquivo.

        >-> Lembre-se: não é nada bom regravar todos os dados da lista ligada. Para fazer isso, será
        necessário apagar todo o arquivo e reescrever todos os dados. Isso não é eficiente 
        (imagine um arquivo com 10 milhões de alunos cadastrados ?) e não será permitido neste estudo
        dirigido!
        
        Desta forma, apenas os novos alunos (aqueles que estão na lista e não estão no arquivo)
        devem ser inseridosno arquivo.
        
        
   > OBS 1: Ao iniciar o seu programa (executá-lo) é preciso criar a lista ligada (iniciar a lista). Para isso, você deve primeiramente verificar se já existem nós cadastrados no arquivo. Caso o arquivo já exista (porque foi gerado em uma execução anterior), você deve criar a lista ligada inserindo os nós que já pertencem ao arquivo. Em outras palavras, é preciso carregar na lista ligada os dados que já foram gerados em uma execução anterior e que, portanto, já estão salvos no arquivo. Você deve criar uma lista vazia apenas se o arquivo não existir (ou estiver vazio - sem nenhum nó cadastrado).


  > OBS 2: É obrigatório fazer uma função específica chamada inserirAluno que adiciona um novo aluno (nó da lista) na lista ligada, recebendo como parâmetro de entrada os atributos do aluno (nome, matrícula, nota, frequência e turma) que foram previamente digitados pelo usuário na função main e um ponteiro para o início da lista. 


  > Essa função deve utilizar uma outra função chamada verificarAluno que, por sua vez, percorre toda a lista ligada retornando 1, caso o aluno já exista e 0 caso o aluno não exista na lista. 


  > O novo aluno deve ser inserido apenas se a função verificarAluno retornar 0.
  

Programa 2.
  > Você deve criar uma estrutura do tipo cadastro que seja capaz de armazenar as pessoas cadastradas no sistema. Abaixo os campos obrigatórios:

        ID (int);
        Nome (char[100]);
        Endereço (struct endereco (rua, bairro, número e cep));
        Tipo (C - Cliente, A - Administrador e S - Superusuário);
        usuário (char[50]);
        senha (char[50]);
      
  > Seu sistema deve seguir o mesmo esquema do PROGRAMA 1, ou seja, armazenar os dados em memória principal por meio de uma lista ligada e gravá-los permanentemente em um arquivo. 

  > Existe apenas um superusuário no sistema e ele deve ser inserido no início do arquivo.
  
    >>> Ao iniciar o programa, o sistema cria a lista (com os dados do arquivo) e dependendo dos casos:
    
      >-> caso o arquivo exista (caso o arquivo exista o superusuário será um entre os outros
      nós cadastrados).
      
      >-> Caso o arquivo não exista, o sistema pede que o usuário cadastre primeiramente os dados do
      superusuário.
            O superusuário é então colocado na lista e também gravado no arquivo. Esse usuário terá
      o privilégio de poder cadastrar Administradores para o sistema, além de fazer tudo o que um
      administrador faz.


  > Logo após criar a lista como mostrado acima, o sistema parte para um menu inicial (menu de login):
 
    >>> Todo usuário deve estar logado no sistema para que novos menus sejam exibidos.
      Para isso, primeiramente crie um menu com apenas duas opções:
        1- Login;
        2- Sair.

    >>> Abaixo os detalhes deste menu:
      1- Login:
              Ao escolher a opção login, o sistema verifica se o usuário e a senha estão corretos:
              >-> Caso estejam incorretos, o sistema deve mostrar a mensagem 
                “usuário e/ou senha inválidos." e logo depois retornar para o menu anterior.
              
              >-> Caso o usuário digite suas credenciais (usuário e senha) corretamente, 
                um novo menu deve ser exibido de acordo com o tipo do usuário:
                   --> Se o usuário for o superusuário ou Administrador, deve-se apresentar o seguinte
                   menu:
                          1- Cadastrar Usuário;
                          2- Remover Usuário;
                          3- Pesquisar Usuário por nome;
                          4- Alterar senha.
                          
                   --> Caso o usuário seja um Cliente, o menu a ser apresentado é o seguinte:
                          1- Alterar senha.
                          2- Sair.
                          
    **(Deve sair do sistema, gravando todos os dados não salvos no arquivo).
                          
                          
  > Abaixo temos a descrição dos itens dos submenus que devem ser apresentados apenas depois que o usuário digitar suas credenciais:
      
      1. Cadastrar Usuário:
          >>> Insere um novo nó na lista do tipo Cliente ou Administrador;
          
          >>> Caso o usuário já exista na lista deve-se apresentar uma mensagem 
            “Usuário já cadastrado, por favor, escolha outro.”;
          
          >>> Logo após inserir um novo usuário, o sistema deve apresentar a mensagem:
            “Cadastro realizado com sucesso!. Deseja gravá-lo no disco? (S)im ou (N)ão ?”.
              >-> Caso seja escolhido S, os dados são gravados no arquivo.
              >-> Caso seja escolhido N, os dados são mantidos apenas na lista ligada.
            
            
          >>> A senha não deve ser armazenada como um texto em claro (texto sem criptografia)
          por medidas de segurança. 
    **Mais informações sobre como trabalhar com a senha são dadas mais a frente;
          
          >>> O ID é sempre único e, portanto, ao cadastrar um usuário ele deve ser o próximo
            disponível (o próximo que não foi utilizado por nenhum nó da lista).
          
          >>> Apenas o superusuário pode inserir Administradores;
          
          >>> O superusuário e os Administradores podem inserir clientes.
          
  >       
      
      2. Remover Usuário:
          >>> O usuário deve digitar o ID que deseja remover.
          
          >>> Se o ID existir e o perfil do usuário permitir que ele faça tal remoção, o
            usuário deve ser removido da lista e também do arquivo e uma mensagem 
            “usuário removido com sucesso" deve aparecer.
          
          >>> Se o ID não existir o sistema deve apresentar a mensagem “usuário inexistente.”;.
          
          >>> Se o perfil do usuário não permitir a remoção deve-se apresentar a mensagem
            “você não pode remover esse usuário.”.
          
          >>> O superusuário não pode ser removido.
          
          >>> Apenas Administradores e o superusuário podem remover clientes.
          
          >>> Apenas o superusuário pode remover administradores.
          
  >       
      
      3. Pesquisar Usuário por nome:
          >>> O nome do usuário deve ser digitado;
          
          >>> Caso exista, os dados do usuário devem ser apresentados. Caso contrário, 
            apresenta a mensagem “usuário não encontrado.”. 



      
  > Informações sobre a senha:

        Em sistemas seguros, a senha dos usuários não é diretamente armazenada. Ao invés disso,
    os sistemas costumam armazenar o hash da senha digitada pelo usuário. O hash são funções
    de espalhamento, onde a entrada destas funções pode ser de qualquer
    tamanho, porém a saída é sempre de tamanho fixo.
    
        Como exemplo, temos o SHA256 que é um hash bastante utilizado capaz de receber uma entrada
    qualquer e gerar um código pseudo aleatório (resumo ou hash calculado) com 256 bits de tamanho.
    
        Assim, imagine que a senha digitada pelo usuário seja a entrada da função (E). Primeiramente, 
    aplica-se a função hash H produzindoa saída S (H(E) = S). O valor S é então armazenado como 
    sendo o resumo criptográfico da senha que é digitada como entrada pelo usuário. 
   
  > Uma boa função hash tem algumas propriedades interessantes:

        1- É muito difícil (probabilisticamente improvável) descobrir E apenas conhecendo a 
        função H e o código S;
        2- É muito difícil encontrar E1 e E2 tais que H(E1) = H(E2) = S.

  > Ao tentar logar no sistema, o usuário irá digitar a sua senha E. Neste momento, o sistema calcula H(E) e compara o resultado com aquele H(E) que foi registrado no sistema no momento do cadastro do usuário. Caso sejam iguais, a senha é considerada correta e o usuário é autenticado, passando para o seu submenu.
      
  > (Faça uma pesquisa básica para entender um pouco mais sobre funções hash. Logo depois, descubra como utilizar o SHA256 em linguagem C e mantenha as senhas dos usuários protegidas, como mostrado acima).
