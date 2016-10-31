#include <stdio.h>//fopen
#include <string.h> //strcpy
#include <stdlib.h> //malloc

#define MAX_BUF 128

int sartuErab(char USER[], char PASS[]) {

    /*Erabiltzaile eta pasahitzen fitxategiak irekitzen dira*/
    int i, p, j;
    FILE *fitx1, *fitx2;
    char * erab[MAX_BUF];
    char patherab[MAX_BUF];
    char pathpasa[MAX_BUF];
    char lerroa[MAX_BUF];
    strcpy(patherab,"../erabiltzaileak/users");
    strcpy(pathpasa,"../erabiltzaileak/passwords");

    /*ERABILTZAILEen zerrenda kargatzen da*/
    fitx1= fopen(patherab, "r");
    if (!fitx1)  {
        printf("%s ez da existitzen\n", patherab);
        return -1;
    }

    /*ERABILTZAILE berria sartzen da*/
    char * erabBerria = malloc(sizeof(char)*MAX_BUF);
    strcpy(erabBerria,USER);
    strcat(erabBerria,"\n");
    fputs(erabBerria,fitx1);

    /*Fitxategia ixten da*/
    fclose (fitx1);

    /*PASAHITZen zerrenda kargatzen da*/
    fitx2= fopen(pathpasa, "r");
    if (!fitx2)  {
        printf("%s ez da existitzen\n", pathpasa);
        return -1;
    }

    /*PASAHITZA berria sartzen da*/
    char * pasaBerria = malloc(sizeof(char)*MAX_BUF);
    strcpy(pasaBerria,PASS);
    strcat(pasaBerria,"\n");
    fputs(pasaBerria,fitx2);

    /*Fitxategia ixten da*/
    fclose (fitx2);

    return 1;
}//loginUser