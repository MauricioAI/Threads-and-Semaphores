#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#include "Synchronization.c"

#define MAX_TEXT 512

int menu_ficha6();
void executa_menu_ficha6();
int submenu_ficha6();
void executa_submenu_ficha6();

/*Estrutura*/
struct my_msg_st
{
	long int my_msg_type;
	char some_text[MAX_TEXT];
};

void executa_submenu_ficha6(int opct)
{
	pid_t proc;
	int opcao, estado;
	char ch[250];

	int running = 1;
	int msgid;
	struct my_msg_st some_data;
	long int msg_to_receive = 0;
	char buffer[BUFSIZ];

	switch (opct)
	{

	case 1:

		proc = fork();

		if (proc < 0)
		{
			printf("Erro!\n");
			exit(0);
		}
		else if (proc == 0)
		{
			FILE *fp;
			fp = fopen("PL_Programacao_06_IPC.dat", "wb");
			if (fp == NULL)
			{
				printf("Não foi possivel abrir o ficheiro.");
			}

			msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
			if (msgid == -1)
			{
				fprintf(stderr, "msgget failed with error: %d\n", errno);
				exit(EXIT_FAILURE);
			}

			while (running)
			{
				if (msgrcv(msgid, (void *)&some_data, BUFSIZ, msg_to_receive, 0)== -1)
				{
					fprintf(stderr, "msgrcv failed with error: %d\n", errno);
					exit(EXIT_FAILURE);
				}

				fprintf(fp,"%s", some_data.some_text);

				if (strncmp(some_data.some_text, "terminar", 8) == 0)
				{
					running = 0;

					fclose(fp);

				}
			}

			fp = fopen("PL_Programacao_06_IPC.dat", "rb");

			if (fp == NULL)
			{
				printf("Não foi possivel abrir o ficheiro.");
			}

			while (fscanf(fp,"%s",ch) != EOF)
			{
				puts(ch);
			}

			fclose(fp);

			if (msgctl(msgid, IPC_RMID, 0) == -1)
			{
				fprintf(stderr, "msgctl(IPC_RMID) failed\n");
				exit(EXIT_FAILURE);
			}

			exit(EXIT_SUCCESS);
		}
		else
		{

			msgid = msgget((key_t)1234, 0666 | IPC_CREAT);

			if (msgid == -1)
			{
				fprintf(stderr, "msgget failed with error: %d\n", errno);
				exit(EXIT_FAILURE);
			}

			while (running)
			{

				printf("Introduza texto: ");
				fgets(buffer, BUFSIZ, stdin);

				some_data.my_msg_type = 1;
				strcpy(some_data.some_text, buffer);

				if (msgsnd(msgid, (void *)&some_data, MAX_TEXT, 0) == -1)
				{
					fprintf(stderr, "msgsnd failed\n");
					exit(EXIT_FAILURE);
				}

				if (strncmp(buffer, "terminar", 8) == 0)
				{
					running = 0;

					proc = wait(&estado);

					do
					{
						opcao = submenu_ficha6();
						executa_submenu_ficha6(opcao);

					} while ((opcao != 1) || (opcao != 7));
				}
			}
			exit(EXIT_SUCCESS);

		}
		break;
	case 2:
		do
		{
			opcao = menu_ficha6();
			executa_menu_ficha6(opcao);

		} while ((opcao != 1) || (opcao != 7));
		break;
	default:
		printf("\nLamentamos,mas a opcção escolhida não é válida.");
		break;
	}
}

int submenu_ficha6()
{
	int opt;

	printf("\n+---------------------------------------------------+\n");
	printf("+1 - Envio de mensagens entre processos             +\n");
	printf("+2 - Sair                                           +\n");
	printf("+---------------------------------------------------+\n");
	printf("Seleccione uma opcção: ");
	scanf("%d", &opt);

	return opt;
}

void executa_menu_ficha6(int aOpcao)
{
	int opc;

	switch (aOpcao)
	{
	case 1:
		/*Menu da ficha 1*/
		do
		{
			opc = menu();
			executa_menu(opc);

		} while ((opc != 1) || (opc != 4));
		break;
	case 2:
		/*Menu da ficha 2*/
		do
		{
			opc = menu_ficha2();
			executa_menu_ficha2(opc);

		} while ((opc != 1) || (opc != 3));
		break;
	case 3:
		/*Menu da ficha 3*/
		do
		{

			opc = menu_ficha3();
			executa_menu_ficha3(opc);

		} while ((opc != 1) || (opc != 4));
		break;
	case 4:
		/*Menu da ficha 4*/
		do
		{

			opc = submenu_ficha4();
			executa_submenu_ficha4(opc);

		} while ((opc != 1) || (opc != 3));

		break;
	case 5:
		/*Menu da ficha 5*/
		do
		{

			opc = menu_ficha5();
			executa_menu_ficha5(opc);

		} while ((opc != 1) || (opc != 6));
		break;
	case 6:
		do
		{
			opc = submenu_ficha6();
			executa_submenu_ficha6(opc);

		} while ((opc != 1) || (opc != 2));
		break;
	case 7:
		printf("\nAté à próxima!\n");
		exit(0);
		break;
	default:
		printf("\nLamentamos,mas a opcção escolhida não é válida.");
		break;
	}
}

int menu_ficha6()
{
	int option;

	printf("\n+-------------------------------+\n");
	printf("+1 - Ficha Laboratorial 1       +\n");
	printf("+2 - Ficha Laboratorial 2       +\n");
	printf("+3 - Ficha Laboratorial 3       +\n");
	printf("+4 - Ficha Laboratorial 4       +\n");
	printf("+5 - Ficha Laboratorial 5       +\n");
	printf("+6 - Ficha Laboratorial 6       +\n");
	printf("+7 - Sair                       +\n");
	printf("+-------------------------------+\n");
	printf("Seleccione uma opcção: ");
	scanf("%d", &option);

	return option;
}

int main()
{
	int opcao;

	do
	{
		opcao = menu_ficha6();
		executa_menu_ficha6(opcao);

	} while ((opcao != 1) || (opcao != 7));
}
