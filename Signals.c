#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "Parent_Child_Processes.c" 

char nome_ficheiro[125]; //Variável global para o ficheiro a analisar
char cabecalho[45];      //Variável global para a existência de cabecalho do ficheiro
char coluna_dados[35];   //Variável global para a coluna de dados a analisar
pid_t pid_1, pid_2;

/*Inicialização das seguintes funções devido a ser invocadas depois da
*sua declaração.*/
int menu_sinais();
void executa_menu_sinais();
int menu_ficha3();
void executa_menu_ficha3();
void calcula_media3();

/*Ao processo pai receber o sinal SIGUSR2 dos processos filhos deve reencaminhar o utilizador para o
*submenu da ficha 3. Para tal serve presente a função seguinte que será invocada com o processo pai
*receber o sinal SIGUSR2 dos filhos.*/
void handler(int sign)
{
    int opt;
    char str[1];
    if (sign == SIGUSR2){
        printf("\nSinal SIGUSR2 recebido do filho com PID %ld, retornando para o submenu da Ficha 3\n", getpid());
        printf("Introduza uma tecla para continuar...");
        scanf("%s", str);

        do{

            opt = menu_sinais();
            executa_menu_sinais(opt);

        } while ((opt != 1) || (opt != 3));
    }
}

/*Aos processos filhos receberem o sinal SIGUSR1 do processo pai questionaram o utilizador sobre
*o nome do ficheiro que contém os dados a analisar, a existência de cabeçalho no mesmo e o nome da
*coluna de dados a analisar, de maneira a calcular a média. E para tal serve a seguinte função que
*será invocado quando os filhos receberem o sinal do pai.*/
void questiona_utilizador(int sign)
{
    if (sign == SIGUSR1){
        printf("\nQual o ficheiro a ler: ");
        scanf("%s", nome_ficheiro);

        printf("\nExistência de cabeçalho: ");
        scanf("%s", cabecalho);

        printf("\nQual a coluna de dados a analisar: ");
        scanf("%s", coluna_dados);

        calcula_media3(nome_ficheiro, cabecalho, coluna_dados);
    }
}

/*A função seguinte é responsável por apresentar o gráfico através do comando gnuplot
*que recurrerá ao ficheiro ficha03_histograma.dat, que por sua vez con tém os dados
*da coluna em análise*/
void mostra_grafico3()
{
    system("gnuplot -e \"plot 'ficha03_histograma.dat' using 1:2 with lines title 'idades';pause mouse;\"");
}

/*A função seguinte quando invocado no programa, o que vai fazer é receber como parâmetro o nome do ficheiro
*a analisar, a existência de um cabeçalho e a coluna de dados a analisar. Logo, de seguida com recurso ao
*comando awk procede ao cálculo da média dessa mesma coluna de dados.*/
void calcula_media3(char nomeFicheiro[125], char cabecalho[45], char coluna[35])
{
  char comando [100]="awk -F ',' 'NR > 1 && NF $";

  strcat(comando,coluna);
  strcat(comando,"~/^[0-9]+$/{sum+= $");
  strcat(comando, coluna);
  strcat(comando,";count++} END{if(count > 0) print sum/count;}' ");
  strcat(comando,nomeFicheiro);

  system(comando);

   printf("Média calculada por o filho 1 com o PID: %ld\n", getpid());
}

/*Ao o processo filho receber o sinal SIGUSR1 do processo pai questionará o utilizador sobre
*o nome do ficheiro que contém os dados a analisar, a existência de cabeçalho no mesmo e o nome da
*coluna de dados a analisar, de maneira a criar o ficheiro ficha03_histograma.dat e posteriomente
*apresentar o gráfico. E para tal serve a seguinte função que será invocado quando os filhos receberem
*o sinal do pai.*/
void questiona_utilizador2(int sign)
{
    char str[1];
    if (sign == SIGUSR1){

        printf("\nQual o ficheiro a ler: ");
        scanf("%s", nome_ficheiro);

        printf("\nExistência de cabeçalho: ");
        scanf("%s", cabecalho);

        printf("\nQual a coluna de dados a analisar: ");
        scanf("%s", coluna_dados);

        char comando[100] = "awk -F ',' 'NR > 1 && NF {a[$";
        strcat(coluna_dados, "]++} END{for (i in a) print i,a[i];}' ");
        strcat(coluna_dados, nome_ficheiro);
        strcat(coluna_dados, " | sort -k1 > ficha03_histograma.dat");
        strcat(comando, coluna_dados);
        system(comando);

        printf("Geração do ficheiro histograma efetuada com sucesso por o PID: %ld\n", getpid());

        printf("Introduza uma tecla para continuar...");
        scanf("%s", str);

        mostra_grafico3();
    }
}

/*A função seguinte recebe como parâmetro o inteiro devolvido por a função menu_sinais() e irá realizar tarefas
*consoante a opcção introduzida por o utilizador. Se a opcção for igual a 1 é calculada a média por o processo
*filho 1. Se for igual 2 é realizado o histograma por o processo filho 2. E se for igual a 3, o processo pai envia
*o sinal SGKILL aos dois procesos filhos e ambos terminam.*/
void executa_menu_sinais(int opcao)
{
    int opc, estado, i;
    char str[1];

    switch (opcao){
    case 1:
        pid_1 = fork();

        if (pid_1 < 0){
            fprintf(stderr, "O processo filho 1 falhou!!");
            exit(-1);
        }else if (pid_1 == 0){

            signal(SIGUSR1, questiona_utilizador);

            sleep(2);
            kill(getppid(), SIGUSR2); //Processo filho 1 envia o sinal SIGUSR2 ao pai
            printf("\nSinal SIGUSR2 enviado pelo processo %ld ao processo pai %ld", getpid(), getppid());

            for (i = 0; i < 1; i++)
                pause();
        }else{
            sleep(2);
            kill(pid_1, SIGUSR1);
            waitpid(pid_1, &estado, 0);
            signal(SIGUSR2, handler);
        }

        break;
    case 2:
        pid_2 = fork();

        if (pid_2 < 0){

            fprintf(stderr, "O processo filho 2 falhou!!");

            exit(-1);
        }else if (pid_2 == 0){

            signal(SIGUSR1, questiona_utilizador2); //Processo filho 2 recebe o sinal SIGUSR1 do processo pai
            sleep(2);
            kill(getppid(), SIGUSR2); //O processo filho 2 envia o sinal SIGUSR2 ao processo pai
            printf("\nSinal SIGUSR2 enviado pelo processo %ld ao processo pai %ld", getpid(), getppid());

            for (i = 0; i < 1; i++)
                pause();
        }else{
            sleep(2);
            kill(pid_2, SIGUSR1);
            waitpid(pid_2, &estado, 0);
            signal(SIGUSR2, handler); //Processo pai recebe o sinal SIGUSR2 do processo filho 2
        }

        break;
    case 3:

        sleep(2);
        kill(pid_1, SIGKILL);
        sleep(2);
        kill(pid_2, SIGKILL);

        printf("\nSinal SIGKILL recebido do pai com o PID %ld e enviado aos filhos, retornando para o menu principal.\n", getppid());
        printf("Introduza uma tecla para continuar...");
        scanf("%s", str);

        do{

            opc = menu_ficha3();
            executa_menu_ficha3(opc);

        } while ((opc != 1) || (opc != 4));
        exit(0);
        break;
    default:
        printf("\nLamentamos, mas a opcção introduzida não está correcta.\n");
        break;
    }
}

/*A função seguinte é responsável por apresentar o menu da ficha 3 visualmente ao utilizador
*e fica aguardar que o utilizador introduza uma opcção do menu para que sejam realizadas tarefas
*no programa.
*Após o utilizador ter introduzido uma opcção, a mesma é armazenada numa variável do tipo inteiros
*e devolvida para ser recebida como parâmetro por outra função.*/
int menu_sinais()
{
    int opt;

    printf("\n+--------------------------------------------------+");
    printf("\n+1 - Calculo da média usando sinais                +");
    printf("\n+2 - Realização de histograma usando sinais        +");
    printf("\n+3 - Sair                                          +");
    printf("\n+--------------------------------------------------+");
    printf("\nSeleccione uma opcção: ");
    scanf("%d", &opt);

    return opt;
}

/*A função seguinte fica responsável por receber por parâmetro o inteiro devolvido por a função menu()
*Consoante a opcção escolhida por o utilizador é executada uma tarefa diferente. Se a opcção for igual
*a 1 é apresentado o menu da ficha 1. Se a opcção for igual a 2 é apresentado o menu da ficha 2. Se a
*a opcção for igual a 3 é apresentado o menu da ficha 3. E se for igual a 4 o programa termina.*/
void executa_menu_ficha3(int Opccao)
{
    int opt;

    switch (Opccao){
    case 1:

        do{
            opt = menu();      //Aqui é chamada a função que irá mostrar o menu da ficha1
            executa_menu(opt); //Aqui é chamada a função que permite executar o menu.
        } while ((opt != 1) || (opt != 4));

        break;
    case 2:

        do{

            opt = menu_ficha2();      //Aqui é chamada a função que irá mostrar o menu da ficha2
            executa_menu_ficha2(opt); //Aqui é chamada a função que permite executar o menu.

        } while ((opt != 1) || (opt != 3));
        break;
    case 3:

        do{

            opt = menu_sinais();
            executa_menu_sinais(opt);

        } while ((opt != 1) || (opt != 3));

        break;
    case 4:
        printf("\n Até à próxima!\n");
        exit(0);
        break;
    default:

        printf("\nLamentamos, mas a opcao escolhida esta errada!\n");
        break;
    }
}

/*A função menu() é responsável por apresentar visualmente o menu ao utlizador e esperar que o mesmo
*introduza uma opcção.
*Ao o utilizador introduzir uma opção a mesma é guardada numa variável e retornada para que o programa
*comece a executar tarefas.*/
int menu_ficha3()
{
    int option;

    printf("\n+-----------------------------------------+");
    printf("\n+1 - Menu da Ficha 1                      +");
    printf("\n+2 - Menu da Ficha 2                      +");
    printf("\n+3 - Menu da Ficha 3                      +");
    printf("\n+4 - Sair                                 +");
    printf("\n+-----------------------------------------+");
    printf("\nSeleccione uma opcção: ");
    scanf("%d", &option);

    return option;
}
