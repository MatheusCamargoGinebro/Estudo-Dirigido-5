# Estudo-dirigido-5
Repositório do estudo dirigido 5 (atividade da aula de ILG).


  Este repositório serve para guardar o estudo dirigido 5, que é uma atividade da disciplina de ILG - Lógica e Técnicas de Programação, 
na qual os alunos do Instituto Federal de Educação, Ciência e Tecnologia de São Paulo - IFSP Câmpus Campinas precisam fazer um determinado programa,
ou determinada pesquisa.

  As "especificações" da atividade estão listadas abaixo.


Programa 1.
(colocar depois).
  

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


  > Logo após criar a lista como mostrado acima, o sistema parte para um menu inicial (menu de login). 
 
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
          
          
      3. Pesquisar Usuário por nome:
          >>> O nome do usuário deve ser digitado;
          
          >>> Caso exista, os dados do usuário devem ser apresentados. Caso contrário, 
            apresenta a mensagem “usuário não encontrado.”. 



      
  > Informações sobre a senha

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
