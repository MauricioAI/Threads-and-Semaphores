#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>

#include "Threads.c"

char file[150];
int linha1, linha2, repeticoes;

pthread_mutex_t lock; //Declaração do mutex para a sincronização das threads
sem_t bin_sem;        //Declaração do semáforo

int menu_ficha5();
int submenu_ficha5();
void executa_menu_ficha5();
void executa_submenu_ficha5();

void *thread_function1(void *arg)
{

    int line2 = *(int *)arg;
    int i, aux, count = 0, esc;
    char str[512];
    FILE *fd = fopen(file, "r");
    FILE *fp = fopen("PL_Programacao_05_Threads.csv", "a");

    sem_wait(&bin_sem);
    printf("\nEntrou na zona critica, thread %ld.\n", pthread_self());

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

    sem_post(&bin_sem); //Incrementa um valor no semáforo
    printf("\nSaiu da zona critica thread %ld.\n", pthread_self());
    fclose(fd);
    fclose(fp);

    pthread_exit(NULL);
}

void *thread_function(void *arg)
{

    int line1 = *(int *)arg;
    int i, aux, count = 0, esc;
    char str[512];
    FILE *fd = fopen(file, "r");
    FILE *fp = fopen("PL_Programacao_05_Threads.csv", "w");

    sem_wait(&bin_sem);
    printf("\nEntrou na zona critica, thread %ld.\n", pthread_self());

    if (fd == NULL){
        printf("\nOcorreu um erro a abrir o ficheiro.\n");
    }

    for (aux = i = 0; i < line1; ++i){
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

    sem_post(&bin_sem); //Incrementa um valor no semáforo

    printf("\nSaiu da zona critica thread %ld.\n", pthread_self());
    fclose(fd);
    fclose(fp);
    pthread_exit(NULL);
}

void *extracao_linhas(void *arg)
{
    int line1 = *(int *)arg;

    int i, aux, count = 0, esc;
    char str[512];
    FILE *fd = fopen(file, "r");
    FILE *fp = fopen("PL_Programacao_05_Threads.csv", "w");

    pthread_mutex_lock(&lock);

    printf("\nEntrou na zona critica,thread %ld.\n", pthread_self());

    if (fd == NULL){
        printf("\nOcorreu um erro a abrir o ficheiro.\n");
    }

    for (aux = i = 0; i < line1; ++i){
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

    pthread_mutex_unlock(&lock);
    printf("\nSaíu da zona critica,thread %ld.\n", pthread_self());

    fclose(fd);
    fclose(fp);

    pthread_exit(NULL); //A thread1 termina a sua sessão
}

void *extracao_linhas1(void *arg)
{
    int line2 = *(int *)arg;

    int i, aux, count = 0, esc;
    char str[512];
    FILE *fd = fopen(file, "r");
    FILE *fp = fopen("PL_Programacao_05_Threads.csv", "a");

    pthread_mutex_lock(&lock);

    printf("\nEntrou na zona critica,thread %ld.\n", pthread_self());

    if (fd == NULL){
        printf("Failed to open file\n");
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

    pthread_mutex_unlock(&lock);
    printf("\nSaíu da zona critica,thread %ld.\n", pthread_self());

    fclose(fd);
    fclose(fp);

    pthread_exit(NULL); //A thread2 termina a sua sessão
}

void executa_submenu_ficha5(int opt)
{
    int opcao;
    char c[2];
    pthread_t thread1, thread2;

    switch (opt){
    case 1:

        printf("\nIntroduza o nome do ficheiro: ");
        scanf("%s", file);

        printf("\nIntroduza as linhas que pretende copiar para outro ficheiro: ");
        scanf("%d %d", &linha1, &linha2);

        printf("\nIntroduza o número de repetições: ");
        scanf("%d", &repeticoes);

        pthread_mutex_init(&lock, NULL);

        pthread_create(&thread2, NULL, extracao_linhas1, &linha2);
        pthread_create(&thread1, NULL, extracao_linhas, &linha1);

        pthread_join(thread2, NULL);
        pthread_join(thread1, NULL);

        pthread_mutex_destroy(&lock);

        printf("\nPressione uma tecla para continuar...");
        scanf("%s", c);

        do{

            opcao = submenu_ficha5();
            executa_submenu_ficha5(opcao);

        } while ((opcao != 1) || (opcao != 3));

        break;
    case 2:

        printf("\nIntroduza o nome do ficheiro: ");
        scanf("%s", file);

        printf("\nIntroduza as linhas que pretende copiar para outro ficheiro: ");
        scanf("%d %d", &linha1, &linha2);

        printf("\nIntroduza o número de repetições: ");
        scanf("%d", &repeticoes);

        sem_init(&bin_sem, 0, 1);

        //Criar threads
        pthread_create(&thread2, NULL, thread_function1, &linha2);
        pthread_create(&thread1, NULL, thread_function, &linha1);

        /*Espera que a thread termine*/
        pthread_join(thread2, NULL);

        /*Espera que a thread termine*/
        pthread_join(thread1, NULL);

        sem_destroy(&bin_sem);

        printf("\nPressione uma tecla para continuar...");
        scanf("%s", c);

        do{

            opcao = submenu_ficha5();
            executa_submenu_ficha5(opcao);

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

int submenu_ficha5()
{
    int opccao;

    printf("\n+-------------------------------------------------+\n");
    printf("+1 - Extração e gravação de linhas (mutex)        +\n");
    printf("+2 - Extração e gravação de linhas (sem)          +\n");
    printf("+3 - Sair                                         +\n");
    printf("+-------------------------------------------------+\n");
    printf("Seleccione uma opcção: ");
    scanf("%d", &opccao);

    return opccao;
}

void executa_menu_ficha5(int aOpcao)
{
    int opc;

    switch (aOpcao){
    case 1:
        /*Menu da ficha 1*/
        do{
            opc = menu();
            executa_menu(opc);

        } while ((opc != 1) || (opc != 4));
        break;
    case 2:
        /*Menu da ficha 2*/
        do{
            opc = menu_ficha2();
            executa_menu_ficha2(opc);

        } while ((opc != 1) || (opc != 3));
        break;
    case 3:
        /*Menu da ficha 3*/
        do{

            opc = menu_ficha3();
            executa_menu_ficha3(opc);

        } while ((opc != 1) || (opc != 4));
        break;
    case 4:
        /*Menu da ficha 4*/
        do{

            opc = submenu_ficha4();
            executa_submenu_ficha4(opc);

        } while ((opc != 1) || (opc != 3));

        break;
    case 5:
        /*Submenu da ficha 5*/
        do{

            opc = submenu_ficha5();
            executa_submenu_ficha5(opc);

        } while ((opc != 1) || (opc != 3));
        break;
    case 6:
        printf("\nAté à próxima!\n");
        exit(0);
        break;
    default:
        printf("\nLamentamos,mas a opcção escolhida não é válida.");
        break;
    }
}

int menu_ficha5()
{
    int option;

    printf("\n+-------------------------------+\n");
    printf("+1 - Ficha Laboratorial 1       +\n");
    printf("+2 - Ficha Laboratorial 2       +\n");
    printf("+3 - Ficha Laboratorial 3       +\n");
    printf("+4 - Ficha Laboratorial 4       +\n");
    printf("+5 - Ficha Laboratorial 5       +\n");
    printf("+6 - Sair                       +\n");
    printf("+-------------------------------+\n");
    printf("Seleccione uma opcção: ");
    scanf("%d", &option);

    return option;
}
