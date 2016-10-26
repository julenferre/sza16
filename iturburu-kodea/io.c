#include <stdio.h>//fopen
#include <string.h> //strcpy
#include <math.h> //round-erako
#include <stdlib.h> //rand

#define MAX_BUF 1024

/*
 * @brief Gai bat emanda, gai horretako hitz bat bueltatzen dituen metodoa
 * @param Parametro gisa hautatutako gaia hartzen du
 * @return Zerrendako hitz bat bueltatuko du
 */
char * hitzaHartu(char gaia[]) {
    
    /*Fitxategia irakurtzen da*/
    int i, ausazkoa; 
    FILE * fitxategia = NULL;
    char * hitzak[MAX_BUF];
    char lerroa[MAX_BUF];
    char pathosoa[MAX_BUF];
    size_t len = 0;
    ssize_t read;
    
    strcpy(pathosoa,"/home/julen/Dropbox/EHU3/SZA/PO2/zerrendak/");
    strcat(pathosoa,gaia);
    strtok(pathosoa,"\n");
        
    fitxategia=fopen(pathosoa, "r");
    
    if (!fitxategia)  {
        printf("%s gaia ez da existitzen\n", gaia);
        return "0";
    }
    
    i = 0;
    
    char * hitza = malloc(sizeof(char)*MAX_BUF);
    while (fgets(lerroa, sizeof(lerroa), fitxategia)!=NULL) {
        strcpy(hitza, lerroa);
        strtok(hitza, "\n");
        hitzak[i] = malloc(sizeof(char)*MAX_BUF);
        strcpy(hitzak[i],hitza);
        //printf("%d. hitza: %s\n", i, hitza);
        i ++;
    }
    free(hitza);
    //printf("\n");
    
    fclose (fitxategia);
    
    ausazkoa = rand() % i;
    
    //printf("Ausazkoa: %d. hitza: %s\n", ausazkoa, (char *) hitzak[ausazkoa]);
    
    return hitzak[ausazkoa];
}//hitzaHartu

/*
 * @brief Hitz bat emanda, hizkiak ordentatzen ditu
 * @param Parametro gisa hitz bat hartzen du
 * @return Hitz hori bueltatuko du, baina hizkiak nahasturik dituela
 */
char * qs(char zerrenda[],int ezker_muga,int eskuin_muga)
{
    int ezker,eskuin,temp,erdikoa;

    ezker=ezker_muga;
    eskuin = eskuin_muga;
    erdikoa = zerrenda[(ezker+eskuin)/2];

    do{
        while(zerrenda[ezker]<erdikoa && ezker<eskuin_muga)ezker++;
        while(erdikoa<zerrenda[eskuin] && eskuin > ezker_muga)eskuin--;
        if(ezker <=eskuin)
        {
            temp= zerrenda[ezker];
            zerrenda[ezker]=zerrenda[eskuin];
            zerrenda[eskuin]=temp;
            ezker++;
            eskuin--;

        }

    }while(ezker<=eskuin);
    if(ezker_muga<eskuin){qs(zerrenda,ezker_muga,eskuin);}
    if(eskuin_muga>ezker){qs(zerrenda,ezker,eskuin_muga);}
    
    return zerrenda;
}

char * quicksort(char zerrenda[],int n)
{
    qs(zerrenda,0,n-1);
}

/*
 * @brief Metodo nagusia
 * @param 
 * @return Zenbaki bat bueltatzen du
 */
int main(int argc, char *argv[]){
    
    int caseZenb;
    char buf[MAX_BUF];
    /*
    // Egiaztatu argumentu bat pasa dela.    
    if(argc != 2)
    {
            fprintf(stderr, "Erabilera: %s <ekintza>\n", argv[0]);
            exit(1);
    }
    
    //Egiaztatu komandoa existitzen dela
    if(strcmp(argv[0],"hitzaHartu")){
        caseZenb = 1;
    }
    else { caseZenb = 0;}
	
    switch (caseZenb){
        case 1:*/
            //while(fgets(buf, MAX_BUF, stdin) != NULL)
            while(1)
            {
                printf("Sartu gaia: ");
                fgets(buf, MAX_BUF, stdin);
                char * hitza =  malloc(sizeof(char)*MAX_BUF);
                strcpy(hitza, hitzaHartu(buf));
                if(*hitza!='1'){
                    printf("Zure hitza: %s\n",hitza);
                    char * nahastua =  malloc(sizeof(char)*MAX_BUF);
                    strcpy(nahastua, quicksort(hitza, sizeof(hitza)));
                    printf("Nahastutako hitza: %s\n",nahastua);                    
                }
            }
            /*break;
        default:
            printf("Ekintza hori ez da existitzen\n");
            break;
    }*/
    return 1;
}