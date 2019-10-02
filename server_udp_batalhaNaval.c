/************* UDP SERVER CODE *******************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

//TABULEIRO DO JOGADOR
char myTable[10][10] = {
		{'#',' ',' ',' ',' ',' ',' ',' ',' ',' '},
		{'#',' ',' ',' ',' ',' ',' ',' ',' ',' '},
		{'#',' ',' ',' ',' ',' ',' ',' ',' ',' '},
		{'#',' ',' ',' ',' ',' ',' ',' ',' ',' '},
		{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
		{'#',' ','#',' ','#',' ',' ','#',' ','#'},
		{'#',' ','#',' ','#',' ',' ',' ',' ',' '},
		{'#',' ','#',' ','#',' ',' ','#',' ','#'},
		{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
		{'#','#',' ','#','#',' ','#','#',' ',' '}
};
//TABULEIRO INIMIGO
char enemyTable[10][10] = {
		{'#',' ',' ',' ',' ',' ',' ',' ',' ',' '},
		{'#',' ',' ',' ',' ',' ',' ',' ',' ',' '},
		{'#',' ',' ',' ',' ',' ',' ',' ',' ',' '},
		{'#',' ',' ',' ',' ',' ',' ',' ',' ',' '},
		{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
		{'#',' ','#',' ','#',' ',' ','#',' ','#'},
		{'#',' ','#',' ','#',' ',' ',' ',' ',' '},
		{'#',' ','#',' ','#',' ',' ','#',' ','#'},
		{' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
		{'#','#',' ','#','#',' ','#','#',' ',' '}
};

int ganhou = 0;//VARIAVEL PARA TESTAR SE A CONDIÇAO DE VITORIA OU DERROTA FOI ALCANÇADA

int contarNavios()
/*ESSE METODO SERVE PARA CONFERIR SE AINDA EXISTE ALGUM NAVIO DO JOGADOR OU DO INIMIGO*/
{
	int i,j;
	ganhou = 1;

	for(i = 0;i<10;i++)
	{
		for(j = 0;j<10;j++)
		{
			if(enemyTable[i][j] == '#')
			{
				ganhou = 0;
				break;
			}
		}
	}

	if(ganhou == 1)
	/*SE TODOS OS NAVIOS INIMIGOS FORAM DESTRUIDOS, ESSE JOGADOR VENCEU*/
	{
		printf("\nVOCE GANHOU!!");
		return 0;
	}
	/*CASO ISSO NAO OCORRA.. SEGUE O CODIGO ABAIXO*/
	for(i = 0;i<10;i++)
		{
			for(j = 0;j<10;j++)
			{
				if(myTable[i][j] == '#')
				{
					ganhou = 0;
					break;
				}
			}
		}
	if(ganhou == 1)
	/*APOS CONFERIR SE O JOGADOR VENCEU, CONFERE SE ELE PERDEU OU SEJA
	SE ELE AINDA POSSUI ALGUM NAVIO*/
	{
		printf("\nVOCE PERDEU!!");
		return 0;
	}
}

int Attack(int x, int y)
/*ESSE METODO SERVE PARA ATACAR O TABULEIRO IMIMIGO COM AS COORDENADAS DIGITADAS*/
{
	printf("attacado em: %c %c",x,y);
	printf("attacado em: %c",enemyTable[x][y]);
	switch(enemyTable[x][y])
	{
		case' ':
		/*CASO A CASA ATACADA FOR VAZIA PREENCHE COM UM SIMBOLO QUE SIGNIFICA QUE O TIRO
		 * FOI ERRADO OU SEJA (O);
		 */
		{
			enemyTable[x][y] = 'O';
			break;
		}
		case'#':
		/*CASO A CASA ATACADA POSSUIR UM NAVIO PREENCHE COM UM SIMBOLO QUE SIGNIFICA QUE O
		* TIRO FOI CERTEIRO OU SEJA (X);
		*/
		{
			enemyTable[x][y] = 'X';
			break;
		}
		case'X':
		/*CASO A CASA ATACADA JA ESTIVER SIDO ATACADA, PROBLEMA DO JOGADOR, NADA É ACERTADO
		 * E A CASA CONTINUA COM O SIMBOLO DE ACERTO (X)
		 */
		{
			enemyTable[x][y] = 'X';
			break;
		}
		case'O':
		/*CASO A CASA ATACADA JA ESTIVER SIDO ATACADA E ERRRADA, PROBLEMA DO JOGADOR,
		* NADA É ACERTADO E A CASA CONTINUA COM O SIMBOLO DE ACERTO (O)
		*/
		{
			enemyTable[x][y] = 'O';
			break;
		}
	}
	return 0;
}

int PrintTable()
/*ESSE METODO IMPRIMI O TABULEIRO DO JOGADOR E DO INIMIGO NA TELA*/
{
	int i,j;
	printf("\nMEU TABULEIRO\n");
	printf("  0123456789\n");
	printf("------------\n");

	for(i = 0;i<10;i++)	//linha
	{
		printf("%d|",i);
		for(j = 0;j<10;j++)	//coluna
		{
			printf("%c",myTable[i][j]);
		}

		printf("\n");
	}
		printf("TABULEIRO INIMIGO\n");
		printf("  0123456789\n");
		printf("------------\n");

		for(i = 0;i<10;i++)	//linha
		{

			printf("%d|",i);
			for(j = 0;j<10;j++)	//coluna
			{
				char temp = enemyTable[i][j];
				if (temp == '#')
				/*POR SE TRATAR DO TABULEIRO INIMIGO, NÃO SE PODE MOSTRAR AS CASAS QUE
				 * POSSUEM NAVIO, LOGO O SIMBOLO DO NAVIO É MASCARADO POR UM VAZIO
				 */
				{
					temp = ' ';
				}
				printf("%c",temp);
			}
			printf("\n");
		}
}

int main(){
  int udpSocket, nBytes;
  char buffer[1024];
  struct sockaddr_in serverAddr, clientAddr;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size, client_addr_size;
  int i;

  /*Create UDP socket*/
  udpSocket = socket(PF_INET, SOCK_DGRAM, 0);

  /*Configure settings in address struct*/
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(7891);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

  /*Bind socket with address struct*/
  bind(udpSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

  /*Initialize size variable to be used later on*/
  addr_size = sizeof serverStorage;

  printf("VOCE EH O SERVIDOR, JOGA DEPOIS PORQUE SIM");
  PrintTable();

  while(1){

	  printf("\nAGUARGANDO ADVERSARIO...\n");
	  recvfrom(udpSocket,myTable,sizeof(myTable),0,(struct sockaddr *)&serverStorage, &addr_size);
	  printf("\n----------------------------\nTURNO INIMIGO:\n");
	  PrintTable();

	  contarNavios();
	  /*CHECA SE ESSE JOGADOR PERDEU ANTES DE PERMITIR QUE ELE JOGUE
	   *
	   */
	  if(ganhou == 1)
	  {
		  break;
	  }

	  int x, y;

	  printf("seu turno:\n");
	  printf("digite posicao x:\n");
	  scanf("%d",&x);
	  printf("digite posicao y:\n");
	  scanf("%d",&y);

	  printf("\n----------------------------\nSEU TURNO:\n");
	  Attack(x,y);
	  PrintTable();
	  sendto(udpSocket,enemyTable,sizeof(enemyTable),0,(struct sockaddr *)&serverStorage,addr_size);

	  contarNavios();
	  /*CHECA SE ESSE JOGADOR VENCEU ANTES DE PERMITIR O MOVIMENTO ADVERSARIO
	  *
	  */
	  if(ganhou == 1)
	  {
		  break;
	  }
  }

  return 0;
}
