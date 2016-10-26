#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <signal.h>

#include "login.h"
#include "ausazkoHitza.h"

#define MAX_BUF 1024
#define PORT 50005
#define MAX_WAIT 120

int main(int argc, char *argv[])
{
	int sock, elkarrizketa, n;
	struct sockaddr_in zerb_helb, bez_helb;
	socklen_t helb_tam;
	char buf[MAX_BUF];
	char * komandoak[5];
	struct timeval timer;
	fd_set rset;

	char * user = malloc(sizeof(char)*MAX_BUF);

	int jokoarenEgoera = 0;
	/**
	 * 0 = erabiltzailea sartu behar da
	 * 1 = pasahitza sartu behar da
	 **/

	if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("Errorea socketa sortzean");
		exit(1);
	}

	memset(&zerb_helb, 0, sizeof(zerb_helb));
	zerb_helb.sin_family = AF_INET;
	zerb_helb.sin_addr.s_addr = htonl(INADDR_ANY);
	zerb_helb.sin_port = htons(PORT);

	if(bind(sock, (struct sockaddr *) &zerb_helb, sizeof(zerb_helb)) < 0)
	{
		perror("Errorea socketari helbidea esleitzean");
		exit(1);
	}

	signal(SIGCHLD, SIG_IGN);

	while(1)
	{
		helb_tam = sizeof(bez_helb);

		if((n=recvfrom(sock, buf, MAX_BUF, 0, (struct sockaddr *) &bez_helb, &helb_tam)) !=0)
		{
			perror("Errorea lehen mezua jasotzean");
			exit(1);
		}

		//if(n==0)continue;

		if(fork() == 0)	// Prozesu umeak egin beharrekoa.
		{
			close(sock);
			if((elkarrizketa = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
			{
				perror("Errorea prozesu umearen socketa sortzean");
				exit(1);
			}

			if(connect(elkarrizketa, (struct sockaddr *) &bez_helb, helb_tam) < 0)
			{
				perror("Errorea bezeroarekin konektatzean");
				exit(1);
			}

			do
			{

				if((n=recvfrom(elkarrizketa, buf, MAX_BUF, 0, (struct sockaddr *) &bez_helb, &helb_tam)) < 0)
				{
					perror("Errorea mezua jasotzean");
					exit(1);
				}

				char *token;
				int komkop=0;

				/* Komandoa hartu*/
				token = strtok(buf, ",");

				/* Parametroak hartu */
				while( token != NULL )
				{
					komandoak[komkop] = malloc(10);
					strcpy(komandoak[komkop],token);
					token = strtok(NULL, ",");
				}

				strtok(buf,",");

				/*USER komandoa*/
				if(strcmp(komandoak[0],"USER")==0) {
					if (jokoarenEgoera != 0) {        /*Komando okerra*/
						if (write(elkarrizketa, "EZ,1", 4) < 4) {
							perror("Errorea erantzuna bidaltzean");
							exit(1);
						}
					} else if (loginUser(komandoak[1]) == -1) {  /*Erabiltzaile okerra*/
						if (write(elkarrizketa, "EZ,2", 4) < 4) {
							perror("Errorea erantzuna bidaltzean");
							exit(1);
						}
					} else {
						strcpy(user, komandoak[1]); /*Erabitzaile zuzena*/
						if (write(elkarrizketa, "OK", 2) < 2) {
							perror("Errorea erantzuna bidaltzean");
							exit(1);
						}
						jokoarenEgoera = 1;
					}
				}

				/*PASS komandoa*/
				else if(strcmp(komandoak[0],"PASS")==0) {
					if (jokoarenEgoera != 1) {        /*Komando okerra*/
						if (write(elkarrizketa, "EZ,1", 4) < 4) {
							perror("Errorea erantzuna bidaltzean");
							exit(1);
						}
					} else if (loginPass(loginUser(user), komandoak[1]) == -1) {
						user = "";
						if (write(elkarrizketa, "EZ,3", 4) < 4) {
							perror("Errorea erantzuna bidaltzean");
							exit(1);
						}
					} else {
						if (write(elkarrizketa, "OK", 2) < 2) {
							perror("Errorea erantzuna bidaltzean");
							exit(1);
						}
						jokoarenEgoera=2;
					}
				}

				/*ENTR komandoa*/
				else if(strcmp(komandoak[0],"ENTR")==0) {
					printf("USEEER !!");
				}

				/*LIST komandoa*/
				else if(strcmp(komandoak[0],"LIST")==0) {
					printf("USEEER !!");
				}

				/*PLAY komandoa*/
				else if(strcmp(komandoak[0],"PLAY")==0) {
					printf("USEEER !!");
				}

				/*SOLV komandoa*/
				else if(strcmp(komandoak[0],"SOLV")==0) {
					printf("USEEER !!");
				}

				/*STOP komandoa*/
				else if(strcmp(komandoak[0],"STOP")==0) {
					printf("USEEER !!");
				}

				/*RANK komandoa*/
				else if(strcmp(komandoak[0],"RANK")==0) {
					printf("USEEER !!");
				}

				/*RNKG komandoa*/
				else if(strcmp(komandoak[0],"RNKG")==0) {
					printf("USEEER !!");
				}

				/*EXIT komandoa*/
				else if(strcmp(komandoak[0],"EXIT")==0) {
					printf("USEEER !!");
				}

				/**if(write(elkarrizketa, buf, n) < n)
				{
					perror("Errorea erantzuna bidaltzean");
					exit(1);
				}
			
				timer.tv_sec = MAX_WAIT;
				timer.tv_usec = 0;
				FD_ZERO(&rset);
				FD_SET(elkarrizketa, &rset);
				if((n=select(elkarrizketa+1,&rset,NULL,NULL,&timer)) < 0)
				{
					perror("Errorea mezuaren zain");
					exit(1);
				}
				else if(n == 0)
				{
					fprintf(stderr,"Itxoite denbora maximoa (%d s.) agortuta. Bezeroa bukatutzat jo da.",MAX_WAIT);
					exit(0);
				}*/

			} while((n=read(elkarrizketa, buf, MAX_BUF)) > 0);

			close(elkarrizketa);

			if(n < 0)
			{
				perror("Errorea mezua jasotzean");
				exit(1);
			}
			exit(0);
		}
	}
}
