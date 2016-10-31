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

    strcpy(pathosoa,"../zerrendak/");
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

    /*Ausazko bat kalkulatzen da*/
    ausazkoa = i*(rand()/(double) RAND_MAX);

    //printf("Ausazkoa: %d. hitza: %s\n", ausazkoa, (char *) hitzak[ausazkoa]);

    return hitzak[ausazkoa];
}//hitzaHartu

/*
 * @brief Bi elementu konparatzeko funtzioa
 * @param Bi elementu hartzen ditu parametros
 * @return Zenbaki bat bueltatzen du, lehenengo elementua bigarrena baino
 * txikiagoa edo handiagoa edo berdinak diren adierazten duena.
 */
int compare(const void *a, const void *b) {
    return *(const char *)a - *(const char *)b;
}


char * ausazkoa(char *jasotakoa){

    //int caseZenb;
    //char buf[MAX_BUF];
    char * nahastua =  malloc(sizeof(char)*MAX_BUF);

//    printf("Sartu gaia: ");
//    fgets(buf, MAX_BUF, stdin);
//    char * hitza =  malloc(sizeof(char)*MAX_BUF);
//    strcpy(hitza, jasotakoa);
//    if(*hitza!='1'){
//        printf("Zure hitza: %s\n",hitza);
    printf("Nahastu gabe: %s\n",jasotakoa);
    strcpy(nahastua, jasotakoa);
    qsort(nahastua, strlen(nahastua), sizeof(char), compare);
    printf("Nahastutako hitza: %s\n",nahastua);

//    }

    return nahastua;
}


/*
 * @brief Metodo nagusia
 * @param
 * @return Zenbaki bat bueltatzen du
 */
/*char * ausazkoHitza(int argc, char *argv[]){

    int caseZenb;
    char buf[MAX_BUF];
    char * nahastua =  malloc(sizeof(char)*MAX_BUF);

    printf("Sartu gaia: ");
    fgets(buf, MAX_BUF, stdin);
    char * hitza =  malloc(sizeof(char)*MAX_BUF);
    strcpy(hitza, hitzaHartu(buf));
    if(*hitza!='1'){
        printf("Zure hitza: %s\n",hitza);
        strcpy(nahastua, hitza);
        qsort(nahastua, sizeof(nahastua), sizeof(char), compare);
        printf("Nahastutako hitza: %s\n",nahastua);
    }

    return nahastua;
}*/
