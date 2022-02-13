#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "Processes_Creation.c" 

char nome_ficheiro[125];
char cabecalho[45];
char coluna_dados[35];

int menu_criacao_processos();
void executa_menu_criacao_processos();

/*A função mostra_grafico() fica encarregue de quando invocado no programa de mostrar o gráfico dos
dados existentes no ficheiro ficha02_histograma.dat que terá sido previamente criado por o processo
filho 1.
*/
void mostra_grafico()
{

  system("gnuplot -e \"plot 'ficha02_histograma.dat' using 1:2 with lines title 'idades';pause mouse;\"");
}

/*A função calcula_media() que cuando invocada por o processo filho será lhe passado como parâmetro o nome
do ficheiro a analisar, a existência do cabeçalho e qual a coluna de dados que será analisada. De seguida,
com recurso ao comando awk calculará a média de todos os dados contidos na coluna.*/
void calcula_media(char nomeFicheiro[125], char cabecalho[45], char coluna[35])
{
  char comando [100]="awk -F ',' 'NR > 1 && NF $";

  strcat(comando,coluna);
  strcat(comando,"~/^[0-9]+$/{sum+= $");
  strcat(comando, coluna);
  strcat(comando,";count++} END{if(count > 0) print sum/count;}' ");
  strcat(comando,nomeFicheiro);

  system(comando);

  printf("Média calculada por o filho 1 com o PID: %ld\n",getpid());
}

/*Ao utilizador escolher a opcção do submenu da ficha 2, o processo cria um filho. Esse mesmo filho ficará
encarregue de perguntar ao utilizador o nome do ficheiro, a existência de um cabeçalho e a coluna de dados
a analisar. Imediatamente calculará a média dos dados contidos na coluna do ficheiro indicao por o utilizador*/
void criacao_filho()
{
  pid_t pid_filho;
  int estado;

  pid_filho = fork();

  if (pid_filho < 0){

    fprintf(stderr, "O processo falhou!!");

    exit(-1);
  }else if (pid_filho == 0){

    printf("\nQual o ficheiro a ler: ");
    scanf("%s", nome_ficheiro);

    printf("\nExistência de cabeçalho: ");
    scanf("%s", cabecalho);

    printf("\nQual a coluna de dados a analisar: ");
    scanf("%s", coluna_dados);

    calcula_media(nome_ficheiro, cabecalho, coluna_dados);

    exit(0);
  }else{
    pid_filho = wait(&estado);
  }
}

/*Após o utilizador clicar na opcção 2 do submenu da Ficha 2, o processo pai cria dois filhos.
O primeiro filho ficará encarregue criar o ficheiro ficha02_histograma.dat e o segundo filho
de mostrar o gráfico dos enviados para o ficheiro criado por o filho 1.*/
void criacao_dois_filhos()
{
  pid_t pid_filho1, pid_filho2;
  int estado, opt;

  pid_filho1 = fork();

  //Primeiro processo filho a ser iniciado
  if (pid_filho1 < 0){

    fprintf(stderr, "O processo filho 1 falhou!!");

    exit(-1);
  }else if (pid_filho1 == 0){

    printf("\nQual o ficheiro a ler: ");
    scanf("%s", nome_ficheiro); //Vai ler o nome do ficheiro a analisar.

    printf("\nExistência de cabeçalho: ");
    scanf("%s", cabecalho); //Vai ler a existência de cabeçalhos

    printf("\nQual a coluna de dados a analisar: ");
    scanf("%s", coluna_dados); //Indicar a coluna de dados do ficheiro a analisar.

    char comando[100] = "awk -F ',' 'NR > 1 && NF {a[$";
    strcat(coluna_dados, "]++} END{for (i in a) print i,a[i];}' ");
    strcat(coluna_dados, nome_ficheiro);
    strcat(coluna_dados, " | sort -k1 > ficha02_histograma.dat");
    strcat(comando, coluna_dados);

    system(comando);
    printf("Geração do ficheiro histograma efetuada com sucesso por o PID: %ld\n", getpid());

    exit(0);
  }else{
      pid_filho2 = wait(&estado);
  }

  pid_filho2 = fork();

  //Segundo processo filho a ser iniciado
  if (pid_filho2 < 0){

    fprintf(stderr, "O processo filho 2 falhou!!");

    exit(-1);
  }else if (pid_filho2 == 0){

    mostra_grafico();
    exit(0);
  }else{
    pid_filho2 = wait(&estado); //Aguarda que os filhos terminem

    printf("\nO processo pai com PID: %ld depois de os processos filhos terminarem.\n", getppid());
    printf("\nA reencaminhar para o submenu da ficha 2 por o processo com o PID: %ld", getppid());

    //Encaminha o utilizador para o submenu do programa
    do{

      opt = menu_criacao_processos();
      executa_menu_criacao_processos(opt);

    } while ((opt != 1) || (opt != 3));

    exit(0);
  }
}

/*
*Esta função recebe como parâmetro o valor inteiro que vai ser devolvido por a
*função menu_criacao_processos(). Ou seja, vai receber a opcção escolhida por o
*utilizador e consequência disso, vai executar a acção referente à opcção escolhida.
*/
void executa_menu_criacao_processos(int aOpcao)
{
  int opt;

  switch (aOpcao){
  case 1:

    criacao_filho();
    break;
  case 2:

    criacao_dois_filhos();
    break;
  case 3:
    printf("\nAté à próxima!!\n");
    exit(0);
    break;
  default:
    printf("\nLamentamos, mas a opcção introduzida não está correcta.\n");
    break;
  }
}

/*
*Esta função tem como objectivo mostrar o menu referente à criação de processos.
*E após o utilizador escolher a opcção do menu, vai ler esse valor
*para uma variável, do tipo inteiro. Posto isto, é retornado esse mesmo valor.
*/
int menu_criacao_processos()
{
  int opcao;

  printf("\n+-----------------------------------------+");
  printf("\n+1 - Cálculo da média                     +");
  printf("\n+2 - Realização de Histograma             +");
  printf("\n+3 - Sair                                 +");
  printf("\n+-----------------------------------------+");
  printf("\nSeleccione uma opcção: ");
  scanf("%d", &opcao);

  return opcao;
}

/*
*Esta função recebe como parâmetro o valor inteiro que vai ser devolvido por a
*função menu_ficha2(). Ou seja, vai receber a opcção escolhida por o utilizador
*e consequência disso, vai executar a acção referente à opcção escolhida.
*/
void executa_menu_ficha2(int aOpcao)
{
  int option;

  switch (aOpcao){
  case 1:

    do{
      option = menu();      //Aqui é chamada a função que irá mostrar o menu da ficha1
      executa_menu(option); //Aqui é chamada a função que permite executar o menu.
    } while ((option != 1) || (option != 4));

    break;
  case 2:

    do{
      option = menu_criacao_processos();
      executa_menu_criacao_processos(option);

    } while ((option != 1) || (option != 3));

    break;
  case 3:
    printf("\nAté à próxima!!\n");
    exit(0);
    break;
  default:
    printf("\nLamentamos, mas a opcção introduzida não está correcta.\n");
    break;
  }
}

/*
*Menu principal do programa. Após, o programa ser executado o utilizador vai ter
*contacto com este menu, que de seguida lhe permitirá ir para outros submenus.
*Esta função vai ler a opcção escolhida por o utilizador para uma variável do
*tipo inteiros.E por fim, vai retornar esse valor.
*/
int menu_ficha2()
{
  int opcao;

  printf("\n+-----------------------------------------+");
  printf("\n+1 - Menu da Ficha 1                      +");
  printf("\n+2 - Criação de Processos                 +");
  printf("\n+3 - Sair                                 +");
  printf("\n+-----------------------------------------+");
  printf("\nSeleccione uma opcção: ");
  scanf("%d", &opcao);

  return opcao;
}
