#include <stdio.h>//fopen
#include <string.h> //strcpy
#include <stdlib.h> //malloc

#define MAX_BUF 128

/*
 * @brief erabiltzailea emanda, erabiltzailea zuzena den ala ez bueltatzen du
 * @param Parametro gisa erabiltzailearen izenaren karaktere katea hartzen du
 * @return Ondo badago, erabiltzaile-zenbakia (zerrendako posizioa); bestela, -1
 */
int loginUser(char USER[]) {
    
    /*Fitxategia irakurtzen da*/
    int i, p, j;
    FILE *fitx1;
    char * erab[MAX_BUF];
    char patherab[MAX_BUF];
    char lerroa[MAX_BUF];
    strcpy(patherab,"../erabiltzaileak/users");
    
    /*ERABILTZAILEen zerrenda kargatzen da*/
    fitx1= fopen(patherab, "r");    
    if (!fitx1)  {
        printf("%s ez da existitzen\n", patherab);
        return -1;
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

    /*Erabiltzaileari lerro jauziak kentzen zaizkio*/
    strtok(USER, "\r\n");

    /*Erabiltzailea bilatzen da*/
    p = i;
    i = -1;
    for(j = 0; j<p; j++){
        //printf("%s <-> USER: %s (%d)\n", erab[j], USER, strcmp(erab[j],USER));
        if(strcmp(erab[j],USER)==0)
        {
            i = j;
            break;
        }
    }

    /*Erabiltzaileen zerrenda askatzen da*/
    for(int k = 0; k < i; k++){
        free(erab[k]);
    }

    return i;
}//loginUser

/*
 * @brief erabiltzaile-zenbakia eta pasahitza emanda, logina zuzena den ala ez bueltatzen du
 * @param Parametro gisa erabiltzaile-zenbakia eta pasahitzaren karaktere katea hartzen du
 * @return Ondo badago, 1; bestela, -1
 */
int loginPass(int USER, char PASS[]) {

    /*Fitxategia irakurtzen da*/
    int p;
    FILE *fitx2;
    char * pass[MAX_BUF];
    char pathpass[MAX_BUF];
    char lerroa[MAX_BUF];
    strcpy(pathpass,"../erabiltzaileak/passwords");

    /*PASAHITZen zerrenda kargatzen da*/
    fitx2= fopen(pathpass, "r");
    if (!fitx2)  {
        printf("%s ez da existitzen\n", pathpass);
        return -1;
    }
    p=0;
    char * hitza = malloc(sizeof(char)*MAX_BUF);
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

    /*Pasahitzari lerro jauziak kentzen zaizkio*/
    strtok(PASS, "\r\n");

    /*Emandako pasahitza aldagai batean sartzen da*/
    char * emandakoPass = malloc(sizeof(char)*MAX_BUF);
    strcpy(emandakoPass, pass[USER]);

    //printf("Erab: %d, Pass: %s", USER, emandakoPass);

    /*Pasahitzen zerrenda askatzen da*/
    for(int k = 0; k < p; k++){
        free(pass[k]);
    }

    /*Pasahitzarekin konparatzen da*/
    if(strcmp(emandakoPass,PASS)!=0){
        free(emandakoPass);
        return -1;
    }

    /*Dena ondo joan da*/
    return 1;
}//loginUser