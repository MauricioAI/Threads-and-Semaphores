#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

#include "Signals.c"

char file[150];
int linha1, linha2, repeticoes;

int menu_ficha4();
void executa_menu_ficha4();
int submenu_ficha4();
void executa_submenu_ficha4();

/*A função criar_ficheiro() vai receber como parâmetro a variável file do tipo
string, que será o ficheiro introduzido po o utilizador na thread criada na opcção
1 do submenu da ficha 4. Consequentemente irá retornar o nome do ficheiro criado.*/
const char *criar_ficheiro(char file[150])
{
    char *nome_file, *extensao;
    int i = 0;
    char *tmp = NULL; //Ponteiro inicializado
    char *array[2];

    /*Partir o nome do ficheiro introduzido por o utilizador em dois*/
    tmp = strtok(file, ".");

    /*Percorrer o ponteiro e guardar os valores num array*/
    while (tmp != NULL){
        array[i++] = tmp;
        tmp = strtok(NULL, " ");
    }

    for (i = 0; i < 2; i++){

        nome_file = array[0]; //Vai guardar o nome do ficheiro
        extensao = array[1];  //Vai guardar a extenção do ficheiro
    }

    char delim[] = ".";

    /*Concatenação das palavras de modo a criar o nome do novo ficheiro*/
    strcat(delim, extensao);
    strcat(nome_file, "_inv");
    strcat(nome_file, delim);

    return nome_file;
}

/*A função primeira_parte() será invocada quando a primeira thread for criada na
opcção 2 do submenu da ficha 4. Colocará num ficheiro a primeira linha escolhida
por o utilizador do ficheiro escolhido também por ele, repetida o número de vezes
que o mesmo indicou.*/
void *primeira_parte(void *arg)
{
    int line1 = *(int *)arg;

    int i, aux, count = 0, esc;
    char str[512];
    FILE *fd = fopen(file, "r");
    FILE *fp = fopen("PL_Programacao_04_Threads.csv", "w");
    if (fd == NULL){
        printf("\nOcorreu um erro a abrir o ficheiro.\n");
    }

    for (aux = i = 0; i < line1; ++i){
        if (fgets(str, sizeof(str), fd) == NULL){
            aux = 1;
            break;
        }
    }
    if (!aux){
        while (count != repeticoes){
            esc = fprintf(fp, "%s", str);

            count++;
        }
    }
    if (esc != EOF)
        printf("\nGeração do ficheiro com linhas repetidas por a thread %ld efetuada com sucesso.\n", pthread_self());
    else
        printf("\nOcorreu um erro a escrever no ficheiro, por a thread %ld\n", pthread_self());

    fclose(fd);
    fclose(fp);

    pthread_exit(NULL); //A thread2 termina a sua sessão
}

/*A função segunda_parte() será invocada quando a segunda thread for criada na
opcção 2 do submenu da ficha 4. Colocará num ficheiro a segunda linha escolhida
 por o utilizador do ficheiro escolhido também por ele, repetida o número de
 vezes que o mesmo indicou.*/
void *segunda_parte(void *arg)
{
    int line2 = *(int *)arg;

    int i, aux, count = 0, esc;
    char str[512];
    FILE *fd = fopen(file, "r");
    FILE *fp = fopen("PL_Programacao_04_Threads.csv", "a");
    if (fd == NULL){
        printf("\nOcorreu um erro a abrir o ficheiro.\n");
    }

    for (aux = i = 0; i < line2; ++i){
        if (fgets(str,sizeof(str),fd) == NULL){
            aux = 1;
            break;
        }
    }
    if (!aux){
        while (count != repeticoes){
            esc = fprintf(fp, "%s", str);
            count++;
        }
    }

    if (esc != EOF)
        printf("\nGeração do ficheiro com linhas repetidas por a thread %ld efetuada com sucesso.\n", pthread_self());
    else
        printf("\nOcorreu um erro a escrever no ficheiro, por a thread %ld\n", pthread_self());

    fclose(fd);
    fclose(fp);

    pthread_exit(NULL); //A thread3 termina a sua sessão
}

/*Na opcção 1 do submenu da ficha 4, depois de ter siso criada uma thread. A mesma irá
questionar o utilizador sobre o ficheiro de dados a analisar e vai trocar as linhas
por colunas, e transferir os dados já trocados para um outro ficheiro criado
a partir do ficheiro introduzido por o utilizador.*/
void *transformar_ficheiro()
{

    int l = 0, k = 0;

    char ch[l][k];
    printf("Introduza o nome do ficheiro para transformar: ");
    scanf("%s", file);

    FILE *fp = fopen(file, "r");
    FILE *fq = fopen(criar_ficheiro(file), "w");

    if (fp == NULL){
        printf("\nOcorreu um erro a abrir o ficheiro.\n");
    }else{
        while (fscanf(fp, "%s", ch[l][k]) != EOF){

            l++;
            k++;
        }
    }

    for (int i = 0; i < l; i++){
        for (int j = 0; j < k; j++){
            ch[i][j] = ch[j][i];
        }
    }

    for (int m = 0; m < l; m++){
        for (int n = 0; n < k; n++){
            fprintf(fq, "%s", ch[m][n]);
        }
    }

    printf("\nGeração do ficheiro com troca de linhas com colunas efectuada comando sucesso.\n");
    fclose(fp);
    fclose(fq);
    pthread_exit(NULL); //A thread em execução é terminada
}

/*Após a função submenu_ficha4() ter retornado a variável opccao, a função
executa_submenu_ficha4 vai receber como paraâmetro esse mesmo valor por via da
variável opt do tipo inteiro. Logo, de seguida vai executar a tarefa que corres-
ponda à opcção escolhida por o utilizador.*/
void executa_submenu_ficha4(int opt)
{
    int opcao;
    char c[2];
    pthread_t thread1, thread2, thread3;

    switch (opt){
    case 1:
        pthread_create(&thread1, NULL, transformar_ficheiro, NULL);
        pthread_join(thread1, NULL);

        printf("\nPressione uma tecla para continuar...");
        scanf("%s", c);

        do{
            opcao = submenu_ficha4();
            executa_submenu_ficha4(opcao);

        } while ((opcao != 1) || (opcao != 3));

        break;
    case 2:

        printf("\nIntroduza o nome do ficheiro: ");
        scanf("%s", file);

        printf("\nIntroduza as linhas que pretende copiar para outro ficheiro: ");
        scanf("%d %d", &linha1, &linha2);

        printf("\nIntroduza o número de repetições: ");
        scanf("%d", &repeticoes);

        pthread_create(&thread2, NULL, primeira_parte, &linha1);
        pthread_join(thread2, NULL);

        pthread_create(&thread3, NULL, segunda_parte, &linha2);
        pthread_join(thread3, NULL);

        printf("\nPressione uma tecla para continuar...");
        scanf("%s", c);

        do{
            opcao = submenu_ficha4();
            executa_submenu_ficha4(opcao);

        } while ((opcao != 1) || (opcao != 3));

        break;

    case 3:
        do{

            opcao = menu_ficha4();
            executa_menu_ficha4(opcao);

        } while ((opcao != 1) || (opcao != 5));
        break;

    default:
        printf("Verifique a opcção escolhida.");
        break;
    }
}

/*A função submenu_ficha4() fica responsável por apresentar o submenu da ficha 4
ao utilizador e recolher a opcção escolhida por o mesmo. Que será armazenada na
variável opccao, para proventura ser retornada.*/
int submenu_ficha4()
{
    int opccao;

    printf("\n+-----------------------------------------------+\n");
    printf("+1 - Transformação de linhas em colunas         +\n");
    printf("+2 - Extração e gravação de linhas              +\n");
    printf("+3 - Sair                                       +\n");
    printf("+-----------------------------------------------+\n");
    printf("Seleccione uma opcção: ");
    scanf("%d", &opccao);

    return opccao;
}

/*A função executa_menu_ficha4() ficará encarregue de esecutar o menu principal,
ou seja, após receber a variável aOpcao do tipo interios por parâmetro com a
opcção escolhida por o utilizador irá proceder a execução da respectiva tarefa.*/
void executa_menu_ficha4(int aOpcao)
{
    int opc;

    switch (aOpcao){
    case 1:
        do{
            /*Menu principal da Ficha 1*/
            opc = menu();
            executa_menu(opc);

        } while ((opc != 1) || (opc != 4));
        break;
    case 2:

        do{
            /*Menu principal da Ficha 2*/
            opc = menu_ficha2();
            executa_menu_ficha2(opc);

        } while ((opc != 1) || (opc != 3));
        break;
    case 3:
        do{
            /*Menu principal da Ficha 3*/
            opc = menu_ficha3();
            executa_menu_ficha3(opc);

        } while ((opc != 1) || (opc != 4));
        break;
    case 4:
        do{
            /*Submenu da Ficha 4*/
            opc = submenu_ficha4();
            executa_submenu_ficha4(opc);

        } while ((opc != 1) || (opc != 3));

        break;
    case 5:
        printf("\nAté à próxima!\n");
        exit(0);
        break;
    default:
        printf("\nLamentamos,mas a opcção escolhida não é válida.");
        break;
    }
}

/*A função menu_ficha4() irá apresentar o menu principal visualmente ao utilizador
de modo a ler do teclado a opcção escolhida por o mesmo, que imediatamente a seguir
será retornado esse valor através da variável option;*/
int menu_ficha4()
{
    int option;

    printf("\n+-------------------------------+\n");
    printf("+1 - Ficha Laboratorial 1       +\n");
    printf("+2 - Ficha Laboratorial 2       +\n");
    printf("+3 - Ficha Laboratorial 3       +\n");
    printf("+4 - Ficha Laboratorial 4       +\n");
    printf("+5 - Sair                       +\n");
    printf("+-------------------------------+\n");
    printf("Seleccione uma opcção: ");
    scanf("%d", &option);

    return option;
}
