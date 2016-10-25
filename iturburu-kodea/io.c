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
        return "1";
    }
    
    i = 0;
    
    char * hitza = malloc(sizeof(char)*128);
    while (fgets(lerroa, sizeof(lerroa), fitxategia)!=NULL) {
        strcpy(hitza, lerroa);
        strtok(hitza, "\n");
        hitzak[i] = malloc(sizeof(char)*MAX_BUF);
        strcpy(hitzak[i],hitza);
        printf("%d. hitza: %s\n", i, hitza);
        i ++;
    }
    printf("\n");
    
    fclose (fitxategia);
    
    ausazkoa = rand() % i;
    
    printf("Ausazkoa: %d. hitza: %s\n", ausazkoa, (char *) hitzak[ausazkoa]);
    
    return hitzak[ausazkoa];
}

int main(int argc, char *argv[]){
    
    int caseZenb;
    char buf[MAX_BUF];
    
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
        case 1:
            printf("Sartu gaia: ");
            while(fgets(buf, MAX_BUF, stdin) != NULL)
            {            
                printf("Zure hitza: %s\n",hitzaHartu(buf));                    
            }
            break;
        default:
            printf("Ekintza hori ez da existitzen\n");
            break;
    }
    return 1;
}