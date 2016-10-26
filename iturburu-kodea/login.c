#include <stdio.h>//fopen
#include <string.h> //strcpy
#include <stdlib.h> //malloc

#define MAX_BUF 128

/*
 * @brief erabiltzailea eta pasahitza emanda, logina zuzena den ala ez bueltatzen du
 * @param Parametro gisa erabiltzailea eta pasahitza karaktere kateak hartzen ditu
 * @return 1 ondo badago, 2 erabiltzailea txarto badago, 3 pasahitza txarto badago
 */

char * login(char USER[], char PASS[]) {
    
    /*Fitxategia irakurtzen da*/
    int i, p, j;
    FILE *fitx1, *fitx2;
    char * erab[MAX_BUF], * pass[MAX_BUF];
    char patherab[MAX_BUF], pathpass[MAX_BUF];
    char lerroa[MAX_BUF];
    strcpy(patherab,"/home/julen/Dropbox/EHU3/SZA/PO2/erabiltzaileak/users");
    strcpy(pathpass,"/home/julen/Dropbox/EHU3/SZA/PO2/erabiltzaileak/passwords");
    
    /*ERABILTZAILEen zerrenda kargatzen da*/
    fitx1= fopen(patherab, "r");    
    if (!fitx1)  {
        printf("%s ez da existitzen\n", patherab);
        return "0";
    }
    
    i = 0;
    
    char * hitza = malloc(sizeof(char)*MAX_BUF);
    
    while (fgets(lerroa, sizeof(lerroa), fitx1)!=NULL) {
        strcpy(hitza, lerroa);
        strtok(hitza, "\n");
        erab[i] = malloc(sizeof(char)*MAX_BUF);
        strcpy(erab[i],hitza);
        //printf("%d. hitza: %s\n", i, hitza);
        i ++;
    }
    
    fclose (fitx1);
    
    /*PASAHITZen zerrenda kargatzen da*/
    fitx2= fopen(pathpass, "r");    
    if (!fitx2)  {
        printf("%s ez da existitzen\n", pathpass);
        return "0";
    }
  
    p=0;
    
    while (fgets(lerroa, sizeof(lerroa), fitx2)!=NULL) {
        strcpy(hitza, lerroa);
        strtok(hitza, "\n");
        pass[p] = malloc(sizeof(char)*MAX_BUF);
        strcpy(pass[p],hitza);
        //printf("%d. pasahitza: %s\n", p, hitza);
        p ++;
    }
    free(hitza);
    fclose (fitx2);
    
    /*Erabiltzaileari eta pasahitzari lerro jauziak kentzen zaie*/
    strtok(USER, "\n");
    strtok(PASS, "\n");
        
    /*Erabiltzailea bilatzen da*/
    i = -1;
    for(j = 0; j<p; j++){
        printf("Erab[j]: %s <-> USER = %s\n", erab[j], USER);
        if(strcmp(erab[j],USER))
        {
            i = j;
            break;
        }
    }
    if(i == -1){
        return "2";
    }
    
    printf("Pass[j]: %s <-> PASS = %s\n", pass[i], PASS);
    
    /*Emandako pasahitza aldagai batean sartzen da*/
    char * emandakoPass = malloc(sizeof(char)*MAX_BUF);
    strcpy(emandakoPass, pass[i]);
    
    /*Erabiltzaileen eta pasahitzen zerrendak askatzen dira*/
    for(int k = 0; k < i; k++){
        free(erab[k]);
        free(pass[k]);
    }
    
    /*Pasahitzarekin konparatzen da*/
    if(!strcmp(emandakoPass,PASS)){
        free(emandakoPass);
        return "3";
    }
    
    /*Dena ondo joan da*/
    return "1";
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
            char * erab =  malloc(sizeof(char)*MAX_BUF);
            char * pass =  malloc(sizeof(char)*MAX_BUF);
            strcpy(buf,"2");
            while(buf!="1")
            {
                printf("Sartu erabiltzailea: ");
                fgets(buf, MAX_BUF, stdin);
                strcpy(erab, login(buf,""));                
                if(*erab=='2'){
                    printf("Erabiltzaile okerra\n");                   
                }
                else {
                    printf("Sartu pasahitza: ");
                    fgets(buf, MAX_BUF, stdin);
                    strcpy(pass, login(erab,buf));                
                    if(*erab=='3'){
                        printf("Pasahitza okerra\n");                
                    }
                    else{
                        printf("*** LOGIN ZUZENA ***\n");    
                    }
                }
            }
            /*break;
        default:
            printf("Ekintza hori ez da existitzen\n");
            break;
    }*/
    return 1;
}