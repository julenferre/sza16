#include <stdio.h>

/*
 * @brief erabiltzailea eta pasahitza emanda, logina zuzena den ala ez bueltatzen du
 * @param Parametro gisa erabiltzailea eta pasahitza karaktere kateak hartzen ditu
 * @return 0 ondo badago, 2 erabiltzailea txarto badago, 3 pasahitza txarto badago
 */


int login(char USER[], char PASSWORD[]) {
    
    /*Fitxategia irakurtzen da*/
    int erab_k, erab_i; 
    FILE *fitx1, *fitx2;
    const char *erab[100], *pass[100];
    char patherab[], pathpass[];
    strcpy(patherab,"./erabiltzaileak/users");
    strcpy(pathpass,"./erabiltzaileak/passwords");
    
    /*ERABILTZAILEen zerrenda kargatzen da*/
    fitx1= fopen(patherab, "r");    
    if (!fitx1)  {
        printf("%s ez da existitzen\n", fitx1);
        return 1;
    }    
    erab_k = 0;    
    while (!feof(fitx1))
    {  
        fgets(erab[erab_k], sizeof(erab[erab_k]), fitx1);
        erab_k ++;
    }    
    fclose (fitx1);
    
    /*PASAHITZen zerrenda kargatzen da*/
    fitx2= fopen(pathpass, "r");    
    if (!fitx2)  {
        printf("%s ez da existitzen\n", fitx2);
        return 1;
    }    
    erab_k = 0;    
    while (!feof(fitx2))
    {  erab
        fgets(pass[erab_k], sizeof(pass[erab_k]), fitx2);
        erab_k ++;
    }    
    fclose (fitx2);
    
    /*Erabiltzailea bilatzen da*/
    erab_i = -1;
    for(int j = 0; j<p; j++){
        if(erab[j]==USER)
        {
            erab_i = j;
            break;
        }
    }
    if(erab_i == -1){
        return 2;
    }
    
    /*Pasahitzarekin konparatzen da*/
    if(pass[erab_i]!=PASS){
        return 3;
    }
    
    /*Dena ondo joan da*/
    return 0;
}