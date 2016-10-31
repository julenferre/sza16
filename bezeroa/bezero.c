#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/stat.h>
#include <unistd.h>

#include "bezero.h"

int main(int argc, char *argv[])
{
  char buf[MAX_BUF], erabiltzailea[MAX_BUF], param[MAX_BUF], param2[MAX_BUF], lag[MAX_BUF], tm[2];
  char zerbitzaria[MAX_BUF];
  int portua = PORT;
  socklen_t helb_tam;

  int sock, n, status, aukera, i;
  struct sockaddr_in zerb_helb;
  struct hostent *hp;


  // Parametroak prozesatu.
  switch(argc)
  {
    case 1:
      strcpy(zerbitzaria, SERVER);
          break;
    case 3:
      portua = atoi(argv[2]);
    case 2:
      strcpy(zerbitzaria, argv[1]);
          break;
    default:
      printf("Erabilera: %s <zerbitzaria> <portua>\n", argv[0]);
          exit(1);
  }

  // Socketa sortu.
  if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror("Errorea socketa sortzean");
    exit(1);
  }

  // Zerbitzariko socketaren helbidea sortu.
  memset(&zerb_helb, 0, sizeof(zerb_helb));
  zerb_helb.sin_family = AF_INET;
  zerb_helb.sin_port = htons(portua);
  if((hp = gethostbyname(zerbitzaria)) == NULL)
  {
    herror("Errorea zerbitzariaren izena ebaztean");
    exit(1);
  }
  memcpy(&zerb_helb.sin_addr, hp->h_addr, hp->h_length);

  // Konexioa egin baino lehen mezu huts bat bidali zerbitzarira
  if(sendto(sock, NULL, 0, 0, (struct sockaddr *) &zerb_helb, sizeof(zerb_helb)) < 0)
  {
    perror("Errorea lehenengo mezua bidaltzean");
    exit(1);
  }
  // Mezu hutsaren erantzuna jaso (OK)
  helb_tam = sizeof(zerb_helb);
  if((n = recvfrom(sock, buf, MAX_BUF,0, (struct sockaddr *) &zerb_helb, &helb_tam)) < 0)
  {
    perror("Errorea lehenengo erantzuna jasotzean");
    exit(1);
  }
  // Konektatu zerbitzariarekin.
  if(connect(sock, (struct sockaddr *) &zerb_helb, helb_tam) < 0)
  {
    perror("Errorea zerbitzariarekin konektatzean");
    exit(1);
  }

  do
  { /* Hasierako menuan hiru aukera daude:
    *   1: Erabiltzaile berri bat sortu (erregistratu).
    *   2: Existitzen den erabiltzaile batekin identifikatu, eta jokora sartu.
    *   3: Identifikatu gabe jokora sartu (anonimoan).
    */
    aukera = hasierakoMenua();
    switch(aukera)
    {
      case 0: jokuaItxi(sock); //Jokua itxi - irten
      case 1: //Erregistratu
        do
        { // Erroreren bat balego 'continue' erabiliz hurrengo begiztara pasako litzateke datuak berriro sartzen hasteko.
          printf("Erabiltzaile izena: ");
          fgets(param,MAX_BUF,stdin);
          param[strlen(param)-1] = 0;


          printf("Pasahitza: ");
          fgets(param2,MAX_BUF,stdin);
          param[strlen(param2)-1] = 0;

          sprintf(buf,"%s,%s,%s",KOMANDOAK[COM_ERRG],param,param2);
          if(write(sock, buf, strlen(buf)) < 0)
          {
            perror("Errorea datuak bidaltzean.");
            continue;
          }
          if(read(sock,buf,MAX_BUF) < 0)
          {
            perror("Errorea datuak jasotzean.");
            continue;
          }
          buf[2]=0;
          if(strcmp(buf,"OK") != 0)
          {
            fprintf(stderr, "Errorea: %s",ER_MEZUAK[8]);
            continue;
          }
          break;
        } while(1);
            printf("%s erabiltzailea ondo erregistratu da.",param);
            continue;
      case 2: //Sesioa hasi
        do
        { // Erroreren bat balego 'continue' erabiliz hurrengo begiztara pasako litzateke datuak berriro sartzen hasteko.
          // Erabiltzaile izena USER komandoaren bidez bidali.
          printf("Erabiltzaile izena: ");
          fgets(param,MAX_BUF,stdin);
          param[strlen(param)-1] = 0;
          strcpy(erabiltzailea,buf); // Erabiltzailea aldagaian uneko erabiltzailea gorde gerorako.
          sprintf(buf,"%s,%s",KOMANDOAK[COM_USER],param);
          if(write(sock, buf, strlen(buf)) < 0)
          {
            perror("Errorea datuak bidaltzean.");
            continue;
          }
          if(read(sock,buf,MAX_BUF) < 0)
          {
            perror("Errorea datuak jasotzean.");
            continue;
          }
          buf[2]=0;
          if(strcmp(buf,"OK") != 0)
          {
            fprintf(stderr, "Errorea: %s",ER_MEZUAK[2]);
            continue;
          }

          // Pasahitza PASS komandoaren bidez bidali.
          printf("Pasahitza: ");
          fgets(param,MAX_BUF,stdin);
          param[strlen(param)-1] = 0;
          sprintf(buf,"%s,%s",KOMANDOAK[COM_PASS],param);
          if(write(sock, buf, strlen(buf)) < 0)
          {
            perror("Errorea datuak bidaltzean.");
            continue;
          }
          if(read(sock,buf,MAX_BUF) < 0)
          {
            perror("Errorea datuak jasotzean.");
            continue;
          }
          buf[2]=0;
          if(strcmp(buf,"OK") != 0)
          {
            fprintf(stderr, "Errorea: %s",ER_MEZUAK[3]);
            continue;
          }
          break;
        } while(1);
      case 3: // Sesioa hasi gabe jokora sartu.
        // Gainera, aurreko atala exekutatzean (sesioa hastean) atal hau ere exekutatuko da jokoan sartzeko.
        while (1) {
          printf("\nJokura sartu nahi zara?(B/E)");
          fgets(param,MAX_BUF,stdin);
          param[1] = 0;
          if(strcmp(param,"B") == 0){
            if(write(sock, KOMANDOAK[COM_ENTR], 4) < 0)
            {
              perror("Errorea datuak bidaltzean.");
              continue;
            }
            if(read(sock,buf,MAX_BUF) < 0)
            {
              perror("Errorea datuak jasotzean.");
              continue;
            }
            buf[2]=0;
            if(strcmp(buf,"OK") != 0)
            {
              fprintf(stderr, "Errorea: Ezin izan da sartu.");
              continue;
            }
            break;
          }
          else if(strcmp(param,"E") != 0)
          {
            continue;
          }
          else{
            break;
          }
        } if(strcmp(param,"E") == 0){
      continue;
    } else{
      break;
    }
    }
    if(strcmp(param,"B") == 0){
      break;
    }
  } while(1);

  do
  { /* Menu nagusian hiru aukera daude:
    *   1: Jokatu: hemen sartzean kategoria aukeratu behar duzu eta hitzak asmatzen hasi.
    *   2: Kategoria-listak eskatu.
    *   3: Puntuazio-listak eskatu.
    */
    aukera = menuNagusia();
    switch(aukera)
    {
      case 0: jokuaItxi(sock); //Jokua itxi
      case 1: //Jokatu
        // Kategoria bat bidali PLAY komandoaren bidez jokuaren hitzen multzoa aukeratzeko.
        printf("Sartu hitzen kategoria: ");
            fgets(param,MAX_BUF,stdin);
            param[strlen(param)-1] = 0;
            sprintf(buf,"%s,%s",KOMANDOAK[COM_PLAY],param);
            do { // Lehenengo iterazioan kategoria idatziko da socketan. Hurrengoetan, berriz, jokaldiaren erantzuna.
              if(write(sock, buf, strlen(buf)) < 0)
              {
                perror("Errorea datuak bidaltzean.");
                break;
              }
              memset(buf, 0, sizeof(buf)); //buf string-a hustu
              if((n = read(sock,buf,MAX_BUF)) < 0)
              {
                perror("Errorea datuak jasotzean.");
                break;
              }

              buf[n]='\0';
              // buf-aren lehen bi letrak hartu eta TM komandoa diren begiratu, denbora bukatu den ala ez egiaztatzeko.
              tm[0]=buf[0];
              tm[1]=buf[1];
              if(strcmp(tm,"TM")==0) break; //denbora bukatu denean
              // lag aldagari OK komandoarekin jasotako asmatu beharreko hitza esleitu.
              memset(lag, 0, sizeof(lag)); //lag string-a hustu
              for(i=2;i<n;i++)
              {
                lag[i-2]=buf[i];
              }
              buf[2]=0;
              if(strcmp(buf,"OK") != 0 && strcmp(buf,"WR") != 0)
              {
                fprintf(stderr, "Errorea: %s",ER_MEZUAK[5]);
                break;
              }
              // WR komandoa jasotzen bada, bidalitako hitza ez da erantzun zuzena izan. Berriro asmatzen saiatu behar da.
              // (lehen iterazioan hemen sartzea ezinezkoa da, oraindik hitzarik bidali ez delako.)
              if(strcmp(buf,"WR") == 0) printf("-- Kale egin duzu, berriro saiatu. --\n");
              else{ //asmatu behar den hitza (desordenaturik) pantailaratu
                printf("-------- Puntuak jokoan: %d --------\n",n-2);
                printf("%s\n",lag);
              }
              // Hitzaren erantzun zuzena idatzi (hurrengo iterazioaren hasieran soketean idatziko da).
              fgets(param,MAX_BUF,stdin);
              param[strlen(param)-1] = 0;
              sprintf(buf,"%s,%s",KOMANDOAK[COM_SOLV],param);
              // Sarrera estandarretik 'STOP' hitza bidali bada, jokoa amaitu.
              if(strcmp(param,"STOP") == 0){
                if(write(sock, KOMANDOAK[COM_STOP], 4) < 0)
                {
                  perror("Errorea datuak bidaltzean.");
                  break;
                }
                memset(buf, 0, sizeof(buf));
                if((n = read(sock,buf,MAX_BUF)) < 0)
                {
                  perror("Errorea datuak jasotzean.");
                  break;
                }
                buf[2]=0;
                if(strcmp(buf,"OK") != 0)
                {
                  fprintf(stderr, "Errorea: %s",ER_MEZUAK[6]);
                  break;
                }
                fprintf(stderr,"Jokaldia amaituta.\n");
                break;
              }
            } while(1);
            // Denbora bukatzen denean zerbitzariak lortutako puntuazioa bidaliko digu.
            // Erabiltzailea identifikaturik baldin badago, puntuazioa zerbitzarien listetan gordetzeko aukera izango du.
            if(strcmp(tm,"TM") == 0)
            {
              memset(lag, 0, sizeof(lag));
              for(i=2;i<n;i++)
              {
                lag[i-2]=buf[i];
              }
              printf("---------\nDenbora bukatu da. Hau da zure puntuazioa: %s\n",lag);
              if(strlen(erabiltzailea)>1) // Erabiltzailea identifikaturik baldin badago soilik
              {
                while(1)
                {
                  printf("Puntuazioa gorde nahi duzu?(B/E)");
                  fgets(param,MAX_BUF,stdin);
                  param[1] = 0;
                  if(strcmp(param,"B") == 0)
                  {
                    if(write(sock, KOMANDOAK[COM_RANK], 4) < 0)
                    {
                      perror("Errorea datuak bidaltzean.");
                      continue;
                    }
                    if(read(sock,buf,MAX_BUF) < 0)
                    {
                      perror("Errorea datuak jasotzean.");
                      continue;
                    }
                    buf[2]=0;
                    if(strcmp(buf,"OK") != 0)
                    {
                      fprintf(stderr, "Errorea: Ezin izan da puntuazioa gorde.");
                      continue;
                    }
                    break;
                  }
                  else if(strcmp(param,"E") == 0)
                  {
                    break;
                  }
                  else
                  {
                    continue;
                  }
                }
              }
            }
            break;
      case 2: //Kategoria-listak
        if(write(sock, KOMANDOAK[COM_LIST], 4) < 0)
        {
          perror("Errorea datuak bidaltzean.");
          continue;
        }
            if((n = read(sock,buf,MAX_BUF)) < 0)
            {
              perror("Errorea datuak jasotzean.");
              continue;
            }
            memset(lag, 0, sizeof(lag));
            for(i=2;i<n;i++)
            {
              lag[i-2]=buf[i];
            }
            buf[2]=0;
            if(strcmp(buf,"OK") != 0)
            {
              fprintf(stderr, "Errorea: %s",ER_MEZUAK[4]);
              continue;
            }
            lag[n-2]=0;
            printf("%s\n",lag);
            break;
      case 3: //Puntuazio-listak
        // Kategoria bat sartu eta RNKN komandoaren bidez bidali, kategoria horren puntuazio-listak pantailaratzeko.
        printf("Sartu kategoria bat, haren puntuazio-listak erakusteko: ");
            fgets(param,MAX_BUF,stdin);
            param[strlen(param)-1] = 0;
            sprintf(buf,"%s,%s",KOMANDOAK[COM_RNKG],param);
            if(write(sock, buf, strlen(buf)) < 0)
            {
              perror("Errorea datuak bidaltzean.");
              continue;
            }
            if((n = read(sock,buf,MAX_BUF)) < 0)
            {
              perror("Errorea datuak jasotzean.");
              continue;
            }
            memset(lag, 0, sizeof(lag));
            for(i=2;i<n;i++)
            {
              lag[i-2]=buf[i];
            }
            buf[2]=0;
            if(strcmp(buf,"OK") != 0)
            {
              fprintf(stderr, "Errorea: %s",ER_MEZUAK[7]);
              continue;
            }
            lag[n-2]=0;
            printf("\n%s\n",lag);
            break;
    }
  }while(1);

}

void jokuaItxi(int sock){
  char buf[MAX_BUF];
  if(write(sock, KOMANDOAK[COM_EXIT], 4) < 0)
  {
    perror("Errorea datuak bidaltzean.");
    exit(1);
  }
  if(read(sock,buf,MAX_BUF) < 0)
  {
    perror("Errorea datuak jasotzean.");
    exit(1);
  }
  close(sock);
  exit(0);
}

int hasierakoMenua()
{
  char katea[MAX_BUF];
  int aukera;

  printf("\n");
  printf("\t\t\t\t*********************************************\n");
  printf("\t\t\t\t**              IDENTIFIKATU               **\n");
  printf("\t\t\t\t*********************************************\n");
  printf("\t\t\t\t**                                         **\n");
  printf("\t\t\t\t**       1. Erregistratu                   **\n");
  printf("\t\t\t\t**       2. Saioa hasi                     **\n");
  printf("\t\t\t\t**       3. Saiorik hasi gabe jokatu       **\n");
  printf("\t\t\t\t**                                         **\n");
  printf("\t\t\t\t**       0. Irten                          **\n");
  printf("\t\t\t\t*********************************************\n");
  printf("\t\t\t\t*********************************************\n");

  printf("\t\t\t\tEgin zure aukera: ");
  while(1)
  {
    fgets(katea,MAX_BUF,stdin);
    aukera = atoi(katea);
    if(aukera >= 0 && aukera < 4)
      break;
    printf("\t\t\t\tAukera okerra, saiatu berriro: ");
  }
  printf("\n");
  return aukera;
}

int menuNagusia()
{
  char katea[MAX_BUF];
  int aukera;

  printf("\n");
  printf("\t\t\t\t*********************************************\n");
  printf("\t\t\t\t**           ONGI ETORRI JOKURA            **\n");
  printf("\t\t\t\t*********************************************\n");
  printf("\t\t\t\t**                                         **\n");
  printf("\t\t\t\t**       1. Jokatu                         **\n");
  printf("\t\t\t\t**       2. Kategoria-listak bistaratu     **\n");
  printf("\t\t\t\t**       3. Puntuazio-listak bistaratu     **\n");
  printf("\t\t\t\t**                                         **\n");
  printf("\t\t\t\t**       0. Jokua itxi                     **\n");
  printf("\t\t\t\t*********************************************\n");
  printf("\t\t\t\t*********************************************\n");

  printf("\t\t\t\tEgin zure aukera: ");
  while(1)
  {
    fgets(katea,MAX_BUF,stdin);
    aukera = atoi(katea);
    if(aukera >= 0 && aukera < 4)
      break;
    printf("\t\t\t\tAukera okerra, saiatu berriro: ");
  }
  printf("\n");
  return aukera;
}
