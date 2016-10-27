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
#define PORT 50007
#define MAX_WAIT 120

#define EZ_BAT "EZ,1"
#define EZ_BI "EZ,2"
#define EZ_HIRU "EZ,3"
#define EZ_LAU "EZ,4"
#define EZ_BOST "EZ,5"
#define EZ_SEI "EZ,6"
#define EZ_ZAZPI "EZ,7"
#define EZ_ZORTZI "EZ,8"
#define EZ_BEDERATZI "EZ,9"

int main(int argc, char *argv[])
{
	int sock, elkarrizketa, n;
	struct sockaddr_in zerb_helb, bez_helb;
	socklen_t helb_tam;
	char buf[MAX_BUF];
	char * komandoak[5];
	struct timeval timer;
	fd_set rset;

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

	//signal(SIGCHLD, SIG_IGN);

	while(1)
	{
		helb_tam = sizeof(bez_helb);

		if((n=recvfrom(sock, buf, MAX_BUF, 0, (struct sockaddr *) &bez_helb, &helb_tam)) <0)
		{
			perror("Errorea lehen mezua jasotzean");
			exit(1);
		}
        //if(n==0)continue;

		//if(fork() == 0)	// Prozesu umeak egin beharrekoa.
		//{
            char * user = malloc(sizeof(char)*MAX_BUF);
            int jokoarenEgoera = 1;
            char *token;
            int komkop=0;

			//close(sock);
			/*if((elkarrizketa = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
			{
				perror("Errorea prozesu umearen socketa sortzean");
				exit(1);
			}*/
            elkarrizketa = sock;

			if((n=connect(elkarrizketa, (struct sockaddr *) &bez_helb, helb_tam)) < 0)
			{
				perror("Errorea bezeroarekin konektatzean");
				exit(1);
			}
            else{
                printf("Bezeroa ondo konektatu da (%d)\n", n);
            }

			do {
                //buf[n]=0;

                strtok(buf, "\n");

                /* Komandoa hartu*/
                token = strtok(buf, ",");

                /* Parametroak hartu */
                while (token != NULL) {
                    komandoak[komkop] = malloc(10);
                    strcpy(komandoak[komkop], token);
                    token = strtok(NULL, ",");
                    komkop++;
                }
                komkop=0;

                /*USER komandoa*/
                if (strcmp(komandoak[0], "USER") == 0) {
                    if (jokoarenEgoera != 1) {        /*Komando okerra*/
                        printf("Komando okerra1\n");
                        if (write(elkarrizketa, EZ_BAT, sizeof(EZ_BAT)) < sizeof(EZ_BAT)) {
                            perror("Errorea erantzuna bidaltzean");
                            exit(1);
                        }
                    } else if (loginUser(komandoak[1]) == -1) {  /*Erabiltzaile okerra*/
                        printf("Erab okerra: %s\n", komandoak[1]);
                        if (write(elkarrizketa, EZ_BI, sizeof(EZ_BI)) < sizeof(EZ_BI)) {
                            perror("Errorea erantzuna bidaltzean");
                            exit(1);
                        }
                    } else { /*Erabitzaile zuzena*/
                        strcpy(user, komandoak[1]);
                        printf("Erab zuzena\n");
                        jokoarenEgoera = 2;
                        strcpy(komandoak[0], "");
                        if (write(elkarrizketa, "OK\n", 3) < 3) {
                            perror("Errorea erantzuna bidaltzean");
                            exit(1);
                        }
                    }
                }

                /*PASS komandoa*/
                else if (strcmp(komandoak[0], "PASS") == 0) {
                    if (jokoarenEgoera != 2) {        /*Komando okerra*/
                        printf("Komando okerra2\n");
                        if (write(elkarrizketa, EZ_BAT, sizeof(EZ_BAT)) < sizeof(EZ_BAT)) {
                            perror("Errorea erantzuna bidaltzean");
                            exit(1);
                        }
                    } else if (loginPass(loginUser(user), komandoak[1]) == -1) {
                        user = "";
                        printf("Pasahitza okerra\n");
                        jokoarenEgoera = 1;
                        if (write(elkarrizketa, EZ_HIRU, sizeof(EZ_HIRU)) < sizeof(EZ_HIRU)) {
                            perror("Errorea erantzuna bidaltzean");
                            exit(1);
                        }
                    } else {
                        printf("Pasahitza zuzena\n");
                        if (write(elkarrizketa, "OK\n", 3) < 3) {
                            perror("Errorea erantzuna bidaltzean");
                            exit(1);
                        }
                        jokoarenEgoera = 3;
                    }
                }

                    /*ENTR komandoa*/
                else if (strcmp(komandoak[0], "ENTR") == 0) {
                    if (jokoarenEgoera != 1 || jokoarenEgoera != 3 || jokoarenEgoera != 8 || jokoarenEgoera != 9 || jokoarenEgoera != 10) {        /*Komando okerra*/
                        printf("Komando okerra2\n");
                        if (write(elkarrizketa, EZ_BAT, sizeof(EZ_BAT)) < sizeof(EZ_BAT)) {
                            perror("Errorea erantzuna bidaltzean");
                            exit(1);
                        }
                    }
                    else{
                        jokoarenEgoera = 4;
                    }
                }

                    /*LIST komandoa*/
                else if (strcmp(komandoak[0], "LIST") == 0) {
                    if (jokoarenEgoera != 4) {        /*Komando okerra*/
                        printf("Komando okerra2\n");
                        if (write(elkarrizketa, EZ_BAT, sizeof(EZ_BAT)) < sizeof(EZ_BAT)) {
                            perror("Errorea erantzuna bidaltzean");
                            exit(1);
                        }
                    }
                    else{
                        printf("Gaien zerrenda bueltatu\n");
                    }
                }

                    /*PLAY komandoa*/
                else if (strcmp(komandoak[0], "PLAY") == 0) {
                    if (jokoarenEgoera != 4) {        /*Komando okerra*/
                        printf("Komando okerra2\n");
                        if (write(elkarrizketa, EZ_BAT, sizeof(EZ_BAT)) < sizeof(EZ_BAT)) {
                            perror("Errorea erantzuna bidaltzean");
                            exit(1);
                        }
                    }
                    else{
                        printf("Jokatu\n");
                        jokoarenEgoera = 6;
                    }
                }

                    /*SOLV komandoa*/
                else if (strcmp(komandoak[0], "SOLV") == 0) {
                    if (jokoarenEgoera != 6) {        /*Komando okerra*/
                        printf("Komando okerra2\n");
                        if (write(elkarrizketa, EZ_BAT, sizeof(EZ_BAT)) < sizeof(EZ_BAT)) {
                            perror("Errorea erantzuna bidaltzean");
                            exit(1);
                        }
                    }
                    else{
                        printf("Hitza asmatu\n");
                    }
                }

                    /*STOP komandoa*/
                else if (strcmp(komandoak[0], "STOP") == 0) {
                    if (jokoarenEgoera != 6) {        /*Komando okerra*/
                        printf("Komando okerra2\n");
                        if (write(elkarrizketa, EZ_BAT, sizeof(EZ_BAT)) < sizeof(EZ_BAT)) {
                            perror("Errorea erantzuna bidaltzean");
                            exit(1);
                        }
                    }
                    else{
                        printf("stop\n");
                        jokoarenEgoera = 8;
                    }
                }

                    /*RANK komandoa*/
                else if (strcmp(komandoak[0], "RANK") == 0) {
                    if (jokoarenEgoera != 9) {        /*Komando okerra*/
                        printf("Komando okerra2\n");
                        if (write(elkarrizketa, EZ_BAT, sizeof(EZ_BAT)) < sizeof(EZ_BAT)) {
                            perror("Errorea erantzuna bidaltzean");
                            exit(1);
                        }
                    }
                    else{
                        printf("rank\n");
                        jokoarenEgoera = 4;
                    }
                }

                    /*RNKG komandoa*/
                else if (strcmp(komandoak[0], "RNKG") == 0) {
                    if (jokoarenEgoera != 4) {        /*Komando okerra*/
                        printf("Komando okerra2\n");
                        if (write(elkarrizketa, EZ_BAT, sizeof(EZ_BAT)) < sizeof(EZ_BAT)) {
                            perror("Errorea erantzuna bidaltzean");
                            exit(1);
                        }
                    }
                }

                    /*EXIT komandoa*/
                else if (strcmp(komandoak[0], "EXIT") == 0) {
                    printf("EXIT\n");
                }

                    /*Komando ezezaguna*/
                else {
                    printf("Komando ezezaguna\n");
                    if (write(elkarrizketa, "EZ,1", 4) < 4) {
                        perror("Errorea erantzuna bidaltzean");
                        exit(1);
                    }
                }

                strcpy(komandoak[0],"");

            }while((n=read(elkarrizketa, buf, MAX_BUF)) > 0);

			close(elkarrizketa);

			if(n < 0)
			{
				perror("Errorea mezua jasotzean");
				exit(1);
			}
			exit(0);
		//}/fork
	}
}
