#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <signal.h>

#include "erregistratu.h"
#include "login.h"
#include "ausazkoHitza.h"
#include "kronoa.h"

#define MAX_BUF 1024
#define PORT 50005
#define MAX_WAIT 120

#define EZ_BAT "EZ,1\n"
#define EZ_BI "EZ,2\n"
#define EZ_HIRU "EZ,3\n"
#define EZ_LAU "EZ,4\n"
#define EZ_BOST "EZ,5\n"
#define EZ_SEI "EZ,6\n"
#define EZ_ZAZPI "EZ,7\n"
#define EZ_ZORTZI "EZ,8\n"
#define EZ_BEDERATZI "EZ,9\n"

int main(int argc, char *argv[]) {
    int sock, elkarrizketa, n, puntuaketa;
    struct sockaddr_in zerb_helb, bez_helb;
    socklen_t helb_tam;
    char buf[MAX_BUF];

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Errorea socketa sortzean");
        exit(1);
    }
    //printf("Socket-a sortu da\n");

    memset(&zerb_helb, 0, sizeof(zerb_helb));
    zerb_helb.sin_family = AF_INET;
    zerb_helb.sin_addr.s_addr = htonl(INADDR_ANY);
    zerb_helb.sin_port = htons(PORT);

    if (bind(sock, (struct sockaddr *) &zerb_helb, sizeof(zerb_helb)) < 0) {
        perror("Errorea socketari helbidea esleitzean");
        exit(1);
    }
    //printf("Socket-ari helbidea esleitu zaio\n");

    signal(SIGCHLD, SIG_IGN);

    while (1) {
        helb_tam = sizeof(bez_helb);

        if ((n = recvfrom(sock, buf, MAX_BUF, 0, (struct sockaddr *) &bez_helb, &helb_tam)) < 0) {
            perror("Errorea lehen mezua jasotzean");
            exit(1);
        }
        //printf("Lehen mezua jaso da\n");

        if (fork() == 0) {   // Prozesu umeak egin beharrekoa
            char *user = malloc(sizeof(char) * MAX_BUF);
            int jokoarenEgoera = 1;
            char *token = malloc(sizeof(char) * MAX_BUF);
            int argkop = 0;
            char kom[4];
            char *args[5];
            char *hitzak[2];
            char *gaia;
            int denbora;
            int puntuaketa;
            char *hiscore = malloc(sizeof(char) * MAX_BUF);

            close(sock);
            if ((elkarrizketa = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
                perror("Errorea prozesu umearen socketa sortzean");
                exit(1);
            }
            //printf("Prozesu umearen socketa sortu da\n");

            if ((n = connect(elkarrizketa, (struct sockaddr *) &bez_helb, helb_tam)) < 0) {
                perror("Errorea bezeroarekin konektatzean");
                exit(1);
            }
            printf("Bezeroa ondo konektatu da\n");

            if(write(elkarrizketa, buf, 0) < n)
            {
                perror("Errorea erantzuna bidaltzean");
                exit(1);
            }

            do {
                //buf[n]=0;
                n = read(elkarrizketa, buf, MAX_BUF);

                strtok(buf, "\n");

                // Komandoa hartu
                //token = strtok(buf, ",");
                strncpy(kom, buf, 4);

                int c = 0;
                int length = strlen(buf) - 4;

                char sub[MAX_BUF];

                while (c < length) {
                    sub[c] = buf[5 + c -1];
                    c++;
                }
                sub[c] = '\0';

                printf("Komandoa: %s\n",kom);
                printf("sub: %s\n",sub);


                // Parametroak hartu
                argkop = 0;
                token = strtok(sub, ",");
                while (token != NULL) {
                    args[argkop] = malloc(MAX_BUF);
                    strcpy(args[argkop], token);
                    token = strtok(NULL, ",");
                    printf("%d. arg: %s\n", argkop, args[argkop]);
                    argkop++;
                }

                printf("Jasotakoa: %s\n",buf);

                /*ERRG komandoa*/
                if (strcmp(kom, "ERRG") == 0) {
                    if (jokoarenEgoera != 1) {        /*Komando okerra*/
                        printf("Komando okerra\n");
                        if (write(elkarrizketa, EZ_BAT, sizeof(EZ_BAT)) < sizeof(EZ_BAT)) {
                            perror("Errorea erantzuna bidaltzean\n");
                            exit(1);
                        }
                    } else if (argkop != 2) {  /*Argumento kopuru okerra*/
                        if (write(elkarrizketa, EZ_BAT, sizeof(EZ_BAT)) < sizeof(EZ_BAT)) {
                            perror("Errorea erantzuna bidaltzean\n");
                            exit(1);
                        }
                    } else {
                        if (sartuErab(args[0], args[1]) != 1) {
                            perror("Errorea erabiltzaile berria sortzean\n");
                            exit(1);
                        }
                        printf("Erabiltzailea ondo sortu da\n");
                        if (write(elkarrizketa, "OK\n", 3) < 3) {
                            perror("Errorea erantzuna bidaltzean\n");
                            exit(1);
                        }
                    }
                }

                /*USER komandoa*/
                else if (strcmp(kom, "USER") == 0) {
                    if (jokoarenEgoera != 1) {        /*Komando okerra*/
                        printf("Komando okerra\n");
                        if (write(elkarrizketa, EZ_BAT, sizeof(EZ_BAT)) < sizeof(EZ_BAT)) {
                            perror("Errorea erantzuna bidaltzean\n");
                            exit(1);
                        }
                    } else if (argkop != 1) {  /*Argumento kopuru okerra*/
                        if (write(elkarrizketa, EZ_BAT, sizeof(EZ_BAT)) < sizeof(EZ_BAT)) {
                            perror("Errorea erantzuna bidaltzean\n");
                            exit(1);
                        }
                    } else if (loginUser(args[0]) == -1) {  /*Erabiltzaile okerra*/
                        printf("Erab okerra: %s\n", args[1]);
                        if (write(elkarrizketa, EZ_BI, sizeof(EZ_BI)) < sizeof(EZ_BI)) {
                            perror("Errorea erantzuna bidaltzean\n");
                            exit(1);
                        }
                    } else { /*Erabitzaile zuzena*/
                        strcpy(user, args[0]);
                        printf("Erabiltzaile zuzena\n");
                        jokoarenEgoera = 2;
                        strcpy(args[0], "");
                        if (write(elkarrizketa, "OK\n", 3) < 3) {
                            perror("Errorea erantzuna bidaltzean\n");
                            exit(1);
                        }
                    }
                }

                    /*PASS komandoa*/
                else if (strcmp(kom, "PASS") == 0) {
                    if (jokoarenEgoera != 2) {        /*Komando okerra*/
                        printf("Komando okerra\n");
                        if (write(elkarrizketa, EZ_BAT, sizeof(EZ_BAT)) < sizeof(EZ_BAT)) {
                            perror("Errorea erantzuna bidaltzean\n");
                            exit(1);
                        }
                    } else if (argkop != 1) {  /*Argumento kopuru okerra*/
                        if (write(elkarrizketa, EZ_BAT, sizeof(EZ_BAT)) < sizeof(EZ_BAT)) {
                            perror("Errorea erantzuna bidaltzean\n");
                            exit(1);
                        }
                    } else if (loginPass(loginUser(user), args[0]) == -1) {
                        user = "";
                        printf("Pasahitza okerra\n");
                        jokoarenEgoera = 1;
                        if (write(elkarrizketa, EZ_HIRU, sizeof(EZ_HIRU)) < sizeof(EZ_HIRU)) {
                            perror("Errorea erantzuna bidaltzean\n");
                            exit(1);
                        }
                    } else {
                        printf("Pasahitza zuzena\n");
                        if (write(elkarrizketa, "OK\n", 3) < 3) {
                            perror("Errorea erantzuna bidaltzean\n");
                            exit(1);
                        }
                        jokoarenEgoera = 3;
                    }
                }

                    /*ENTR komandoa*/
                else if (strcmp(kom, "ENTR") == 0) {
                    if (jokoarenEgoera != 1 && jokoarenEgoera != 3 && jokoarenEgoera != 8 && jokoarenEgoera != 9 &&
                        jokoarenEgoera != 10) {        /*Komando okerra*/
                        printf("Komando okerra\n");
                        if (write(elkarrizketa, EZ_BAT, sizeof(EZ_BAT)) < sizeof(EZ_BAT)) {
                            perror("Errorea erantzuna bidaltzean\n");
                            exit(1);
                        }
                    } else if (argkop != 0) {  /*Argumento kopuru okerra*/
                        if (write(elkarrizketa, EZ_BAT, sizeof(EZ_BAT)) < sizeof(EZ_BAT)) {
                            perror("Errorea erantzuna bidaltzean\n");
                            exit(1);
                        }
                    } else {
                        printf("ENTR komandoa sartuta\n");
                        jokoarenEgoera = 4;
                    }
                }

                    /*LIST komandoa*/
                else if (strcmp(kom, "LIST") == 0) {
                    if (jokoarenEgoera != 4) {        /*Komando okerra*/
                        printf("Komando okerra\n");
                        if (write(elkarrizketa, EZ_BAT, sizeof(EZ_BAT)) < sizeof(EZ_BAT)) {
                            perror("Errorea erantzuna bidaltzean\n");
                            exit(1);
                        }
                    } else if (argkop != 0) {  /*Argumento kopuru okerra*/
                        if (write(elkarrizketa, EZ_BAT, sizeof(EZ_BAT)) < sizeof(EZ_BAT)) {
                            perror("Errorea erantzuna bidaltzean\n");
                            exit(1);
                        }
                    } else {
                        FILE *fp;
                        char buff[512];

                        fp = popen("ls ../zerrendak/", "r");
                        while (fgets(buff, MAX_BUF, fp) != NULL) {
                            strcat(buf, buff);
                        }
                        pclose(fp);

                        write(elkarrizketa, buf, strlen(buf));
                        printf("LIST\n");
                    }
                }

                    /*PLAY komandoa*/
                else if (strcmp(kom, "PLAY") == 0) {
                    if (jokoarenEgoera != 4) {        /*Komando okerra*/
                        printf("Komando okerra\n");
                        if (write(elkarrizketa, EZ_BAT, sizeof(EZ_BAT)) < sizeof(EZ_BAT)) {
                            perror("Errorea erantzuna bidaltzean\n");
                            exit(1);
                        }
                    } else if (argkop != 1) {  /*Argumento kopuru okerra*/
                        if (write(elkarrizketa, EZ_BAT, sizeof(EZ_BAT)) < sizeof(EZ_BAT)) {
                            perror("Errorea erantzuna bidaltzean\n");
                            exit(1);
                        }
                    } else if (strcmp(hitzaHartu(args[0]), "0") == 0) {  /*Gai okerra*/
                        if (write(elkarrizketa, EZ_BI, sizeof(EZ_BI)) < sizeof(EZ_BI)) {
                            perror("Errorea erantzuna bin = malloc(sizeof(char) * MAX_BUF);daltzean\n");
                            exit(1);
                        }
                    } else if ((strcmp(hitzaHartu(args[1]), "2") != 0) ||
                            (strcmp(hitzaHartu(args[1]), "5") != 0) ||
                            (strcmp(hitzaHartu(args[1]), "10") != 0)) {  /*Denbora okerra*/
                        if (write(elkarrizketa, EZ_BI, sizeof(EZ_BI)) < sizeof(EZ_BI)) {
                            perror("Errorea erantzuna bidaltzean\n");
                            exit(1);
                        }
                    }
                    else {
                        hitzak[0] = malloc(sizeof(char) * MAX_BUF);
                        strcpy(hitzak[0], hitzaHartu(args[0]));
                        gaia = malloc(sizeof(char) * MAX_BUF);
                        strcpy(gaia, args[0]);
                        hitzak[1] = malloc(sizeof(char) * MAX_BUF);
                        strcpy(hitzak[1], ausazkoa(hitzak[0]));
                        if ((write(elkarrizketa, hitzak[1], strlen(hitzak[1]))) < 0) {
                            perror("Errorea hitza bidaltzean");
                            exit(1);
                        }
                        printf("Hitza bidali da\n");
                        jokoarenEgoera = 6;

                        puntuaketa = 0;
                        denbora = atoi(args[1])*60;

                        //Kronoa hasiarazten da
                        start_timer(denbora);
                    }
                }

                    /*SOLV komandoa*/
                else if (strcmp(kom, "SOLV") == 0) {
                    if (jokoarenEgoera != 6) {        /*Komando okerra*/
                        printf("Komando okerra2\n");
                        if (write(elkarrizketa, EZ_BAT, sizeof(EZ_BAT)) < sizeof(EZ_BAT)) {
                            perror("Errorea erantzuna bidaltzean\n");
                            printf("popipopipopiipoi\n");
                            exit(1);
                        }
                    } else if (argkop != 1) {  /*Argumento kopuru okerra*/
                        if (write(elkarrizketa, EZ_BAT, sizeof(EZ_BAT)) < sizeof(EZ_BAT)) {
                            perror("Errorea erantzuna bidaltzean\n");
                            exit(1);
                        }
                    } else {
                        if ((strcmp(args[1], hitzak[0])) == 0) {
                            puntuaketa++;
                            write(elkarrizketa, "OK\n", strlen("OK\n"));

                            strcpy(hitzak[0], hitzaHartu(gaia));
                            strcpy(hitzak[1], ausazkoa(hitzak[0]));

                            //write(elkarrizketa,"popi\n",strlen("popi\n"));
                            //printf("%s\n", hitzak[1]);
                            if (write(elkarrizketa, hitzak[1], strlen(hitzak[1])) < strlen(hitzak[1])) {
                                perror("Errorea bidaltzean");
                                exit(1);
                            }
                            //write(elkarrizketa,"pipo\n",strlen("pipo\n"));
                        } else {
                            write(elkarrizketa, "WR\n", strlen("WR\n"));
                        }
                        // printf("Hitza asmatu\n");
                    }
                }

                    /*STOP komandoa*/
                else if (strcmp(kom, "STOP") == 0) {
                    if (jokoarenEgoera != 6) {        /*Komando okerra*/
                        printf("Komando okerra\n");
                        if (write(elkarrizketa, EZ_BAT, sizeof(EZ_BAT)) < sizeof(EZ_BAT)) {
                            perror("Errorea erantzuna bidaltzean");
                            exit(1);
                        }
                    } else if (argkop != 0) {  /*Argumento kopuru okerra*/
                        if (write(elkarrizketa, EZ_BAT, sizeof(EZ_BAT)) < sizeof(EZ_BAT)) {
                            perror("Errorea erantzuna bidaltzean\n");
                            exit(1);
                        }
                    } else {
                        printf("stop\n");
                        jokoarenEgoera = 4;
                    }
                }

                    /*RANK komandoa*/
                else if (strcmp(kom, "RANK") == 0) {
                    if (jokoarenEgoera != 9) {        /*Komando okerra*/
                        printf("Komando okerra2\n");
                        if (write(elkarrizketa, EZ_BAT, sizeof(EZ_BAT)) < sizeof(EZ_BAT)) {
                            perror("Errorea erantzuna bidaltzean");
                            exit(1);
                        }
                    } else if (argkop != 0) {  /*Argumento kopuru okerra*/
                        if (write(elkarrizketa, EZ_BAT, sizeof(EZ_BAT)) < sizeof(EZ_BAT)) {
                            perror("Errorea erantzuna bidaltzean\n");
                            exit(1);
                        }
                    } else {
                        sprintf(buf, "Zure puntuaketa: %d\n", puntuaketa*denbora);
                        write(elkarrizketa, buf, strlen(buf));

                        /*Fitxategia irekitzen da*/
                        int i, p, j;
                        FILE *fitx1, *fitx2;
                        char * erab[MAX_BUF];
                        char path[MAX_BUF];
                        char lerroa[MAX_BUF];
                        strcpy(path,"../ranking/");
                        strcat(path,gaia);

                        /*PUNTUAKETen zerrenda kargatzen da*/
                        fitx1= fopen(path, "a");
                        if (!fitx1)  {
                            printf("%s ez da existitzen\n", path);
                            return -1;
                        }

                        /*PUNTUAZIO berria sartzen da*/
                        fputs(hiscore,fitx1);

                        /*Fitxategia ixten da*/
                        fclose (fitx1);

                        jokoarenEgoera = 10;
                    }
                }

                    /*RNKG komandoa*/
                else if (strcmp(kom, "RNKG") == 0) {
                    if (jokoarenEgoera != 4) {        /*Komando okerra*/
                        printf("Komando okerra\n");
                        if (write(elkarrizketa, EZ_BAT, sizeof(EZ_BAT)) < sizeof(EZ_BAT)) {
                            perror("Errorea erantzuna bidaltzean");
                            exit(1);
                        }
                    } else if (argkop != 1) {  /*Argumento kopuru okerra*/
                        if (write(elkarrizketa, EZ_BAT, sizeof(EZ_BAT)) < sizeof(EZ_BAT)) {
                            perror("Errorea erantzuna bidaltzean\n");
                            exit(1);
                        }
                    } else {
                        FILE *fitx1;
                        char * puntuazioak[MAX_BUF];
                        char path[MAX_BUF];
                        char lerroa[MAX_BUF];
                        strcpy(path,"../ranking/");
                        strcat(path,args[1]);

                        /*ERABILTZAILEen zerrenda kargatzen da*/
                        fitx1= fopen(path, "r");
                        if (!fitx1)  {
                            printf("%s ez da existitzen\n", path);
                            return -1;
                        }

                        int i = 0;

                        char * hitza = malloc(sizeof(char)*MAX_BUF);
                        while (fgets(lerroa, sizeof(lerroa), fitx1)!=NULL) {
                            strcpy(hitza, lerroa);
                            strtok(hitza, "\n");
                            puntuazioak[i] = malloc(sizeof(char)*MAX_BUF);
                            strcpy(puntuazioak[i],hitza);
                            strcat(puntuazioak[i],"\n");
                            if (write(elkarrizketa, puntuazioak[i] , strlen(puntuazioak[i])) < strlen(puntuazioak[i])) {
                                perror("Errorea erantzuna bidaltzean\n");
                                exit(1);
                            }
                            i ++;
                        }

                        fclose (fitx1);
                    }
                }

                    /*EXIT komandoa*/
                else if (strcmp(kom, "EXIT") == 0) {
                    if (argkop != 0) {  /*Argumento kopuru okerra*/
                        if (write(elkarrizketa, EZ_BAT, sizeof(EZ_BAT)) < sizeof(EZ_BAT)) {
                            perror("Errorea erantzuna bidaltzean\n");
                            exit(1);
                        }
                    } else {
                        printf("EXIT\n");
                    }
                }

                    /*Komando ezezaguna*/
                else {
                    printf("Komando ezezaguna: %s\n", kom);
                    if (write(elkarrizketa, EZ_BAT, sizeof(EZ_BAT)) < sizeof(EZ_BAT)) {
                        perror("Errorea erantzuna bidaltzean");
                        exit(1);
                    }
                }

                strcpy(kom, "");

                //printf("Itzuli bat\n");

                //Kronoa begiratzen da
                if(current_time>=denbora){
                    jokoarenEgoera = 9;
                    strcpy(hiscore,"TM");
                    strcat(hiscore,atoi(denbora*puntuaketa));
                    if (write(elkarrizketa, hiscore, strlen(hiscore)) < strlen(hiscore)) {
                        perror("Errorea erantzuna bidaltzean");
                        exit(1);
                    }
                }


            } while (n > 0);

            close(elkarrizketa);
            if (n < 0) {
                perror("Errorea mezua jasotzean");
                exit(1);
            }
            exit(0);
        }//fork

    }//while(1)

}
