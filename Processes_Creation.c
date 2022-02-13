#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int menu();          //Declaração da função menu()
void executa_menu(); //Declaração da função executaMenu()

/*
*Esta função recebe como parâmetro o valor inteiro que vai ser devolvido por a
*função menu_processo(). Ou seja, vai receber a opcção escolhida por o utilizador
*e consequência disso, vai executar a acção referente à opcção escolhida.
*/
void executa_menu_process(int aOpcao)
{
  int opt;

  switch (aOpcao){
  case 1:
    system("ps aux");
    break;
  case 2:
    system("pstree");
    break;
  case 3:
    execl("/usr/bin/top", "top", NULL);
    break;
  case 4:

    do{
      opt = menu();
      executa_menu(opt);

    } while ((opt != 1) || (opt != 4));
    break;
  default:
    printf("\nLamentamos, mas a opcção introduzida não está correcta.\n");
    break;
  }
}

/*
*Esta função tem como objectivo mostrar o menu referente à visualização dos
*processos, e após o utilizador escolher a opcção do menu, vai ler esse valor
*para uma variável, do tipo inteiro. Posto isto, é retornado esse mesmo valor.
*/
int menu_process()
{

  int opcao;

  printf("\n+--------------------------------+");
  printf("\n+1 - ps aux                      +");
  printf("\n+2 - pstree                      +");
  printf("\n+3 - top                         +");
  printf("\n+4 - Voltar ao menu principal    +");
  printf("\n+--------------------------------+");
  printf("\nSeleccione uma opcção: ");
  scanf("%d", &opcao);

  return opcao;
}

/*
*Esta função recebe como parâmetro o valor inteiro que vai ser devolvido por a
*função menu_pids(). Ou seja, vai receber a opcção escolhida por o utilizador
*e consequência disso, vai executar a acção referente à opcção escolhida.
*/
void executa_menu_pid(int aOpcao)
{
  int opc;

  switch (aOpcao){
  case 1:
    printf("\nO PID do processo actual é: %ld\n", getpid());
    break;
  case 2:
    printf("\nO PID do proceso pai é: %ld\n", getppid());
    break;
  case 3:

    do{

      opc = menu();
      executa_menu(opc);

    } while ((opc != 1) || (opc != 4));
    break;
  default:
    printf("\nLamentamos, mas a opcção introduzida não está correcta.\n");
    break;
  }
}

/*
*Esta função tem como objectivo mostrar o menu referente à visualização dos
*PID's.E após o utilizador escolher a opcção do menu, vai ler esse valor
*para uma variável, do tipo inteiro. Posto isto, é retornado esse mesmo valor.
*/
int menu_pids()
{
  int opcao;

  printf("\n+--------------------------------+");
  printf("\n+1 - PID do processo actual      +");
  printf("\n+2 - PID do processo pai         +");
  printf("\n+3 - Voltar ao menu principal    +");
  printf("\n+--------------------------------+");
  printf("\nSeleccione uma opcção: ");
  scanf("%d", &opcao);

  return opcao;
}

/*
*Esta função recebe como parâmetro o valor inteiro que vai ser devolvido por a
*função menu(). Ou seja, vai receber a opcção escolhida por o utilizador
*e consequência disso, vai executar a acção referente à opcção escolhida.
*/
void executa_menu(int aOpcao)
{
  int option;

  switch (aOpcao){
  case 1:

    do{
      option = menu_pids();
      executa_menu_pid(option);

    } while ((option != 1) || (option != 3));
    break;
  case 2:

    do{
      option = menu_process();
      executa_menu_process(option);

    } while ((option != 1) || (option != 4));
    break;
  case 3:
    system("awk -F ',' 'NR > 1 && NF {sum += $1;count++} END{ if(count > 0) printf sum/count;}' PL_Programacao_00_Database.csv");
    break;
  case 4:
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
int menu()
{
  int opcao;

  printf("\n+-----------------------------------------+");
  printf("\n+1 - Consulatar PID's                     +");
  printf("\n+2 - Visualização de processos            +");
  printf("\n+3 - Estatistica Descritiva (Média)       +");
  printf("\n+4 - Sair                                 +");
  printf("\n+-----------------------------------------+");
  printf("\nSeleccione uma opcção: ");
  scanf("%d", &opcao);

  return opcao;
}
