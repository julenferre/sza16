#include <stdio.h>
#include <math.h> //round-erako

/*
 * @brief Gai bat emanda, gai horretako hitz bat bueltatzen dituen metodoa
 * @param Parametro gisa hautatutako gaia hartzen du
 * @return Zerrendako hitz bat bueltatuko du
 */


char[] hitzaHartu(char gaia[]) {
    
    /*Fitxategia irakurtzen da*/
    int i; 
    FILE *fitxategia;
    const char *hitzak[100];
    char pathosoa[];
    strcpy(pathosoa,"./zerrendak/");
    strcat(pathosoa,gaia);
    fitxategia= fopen(pathosoa, "r");
    
    if (!fitxategia)  {
        printf("%s gaia ez da existitzen\n", gaia);
        return 1;
    }
    
    i = 0;
    
    while (!feof(fitxategia))
    {  
        fgets(hitzak[i], sizeof(hitzak[i]), fitxategia);
        i ++;
    }
    
    fclose (fitxategia);
    
    ausazkoa = round(rand() * i);
     
    return hitzak[ausazkoa];#include <math.h>
}