/**
 * Polytech Marseille
 * Case 925 - 163, avenue de Luminy
 * 13288 Marseille CEDEX 9
 * 
 * Ce fichier est l'oeuvre d'élèves de Polytech Marseille. Il ne peut être 
 * reproduit, utilisé ou modifié sans l'avis express de ses auteurs.
 */

/**
 * @author BISSUEL Valentin <valentin.bissuel@etu.univ-amu.fr>
 * @author DENIS Justine <justine.denis@etu.univ-amu.fr>
 *
 * @version 1.0.0 / 11/05/2016
 * 
 * @todo - 
 * 
 * @bug - 
 */

/**
 * @file boussole.c
 * @brief Le programme permet de réaliser une boussole avec les LEDs du GPS
 */

#include <__cross_studio_io.h>
#include <msp430x16x.h>
#include <string.h>
#include <gps.h>
#include <math.h>
#include <stdlib.h>

/* @Brief: Selectionne les trames VTG
* Retourne une trame VTG
*/
void selec_trame_vtg(void)
{
    char* ptr;
    int retour_chariot;
    ptr = strstr(buf_1,"$GPVTG");                                           //Recherche $GPVTG
    retour_chariot = search(ptr,10,0);   
    substr(trame_vtg,ptr,0,retour_chariot);                                 //Decoupe la trame
}

/* @ Selectionne les objets de la trame (entre les virgules)
* @Parametres: 
* - char * objet: chaine de caractere à selectionner
* @ retourne:
* - le cap en degrèes exprimer en float 
  - ou si le cap est mauvais -1 (erreur)
*/
float selec_cap (char * objet, float cap_anc)
{
    if(trame_vtg[0] != '\0')
    {
        int j=0;                                        
        int i;
        int f = 0;
        char valid[2];
        char cap[20];
        f = search(trame_vtg,',',0)+1;                        //se positionne après le nom du protocole
       
        for(i=0; i<20; i++)                                   //initialise la chaine de caractere
        {
            cap[i] = '\0';                                    //suprimme les information dans la chaine de caractere
        }
        while (trame_vtg[f] != ',' && trame_vtg[f] != '\0')
        {
            cap[j] = trame_vtg[f];                            //Enregistre l'angle du cap
            j++;
            f++;  
        }
        if((atof(cap))!= 0)                                   //Si le cap est correcte
        {
            return(atof(cap));                                //retourne transformer en float
        }
        else
        {
            return(-1);                                       //Sinon envoie une erreur
        }
    }else
    {
        return(/*cap_anc*/-1);
    }
}

/*
* @Brief: Allumer LED en fonction de la direction
*   @Parametres: 
    - azimut: angle (direction) entre le pole nord et le points geographique
*/
void boussole_LED(float azimut)
{
    if((azimut>=345 && azimut<=360) ||(azimut>0 && azimut<=15))  //Nord
    {
        P1OUT = 0x02;                                             //allume D2
    }
    else if(azimut>15 && azimut<75)                               //Nord OUEST
    {
                                                                  //allume D4 ET D2
        P1OUT = 0x0A;  
    }
    else if(azimut>=75 && azimut<=105)                            //EST
    {
        P1OUT = 0x10;                                             //allume D5
    }
    else if(azimut>105 && azimut<165)                             //SUD EST
    {
        P1OUT = 0x14;                                               //allume D4 D2
    }
    else if(azimut>=165 && azimut<=195)                           //SUD 
    { 
        P1OUT = 0x04;                                             //allume D2
    }
    else if(azimut>195 && azimut<255)                             //SUD OUEST
    {
        P1OUT = 0x0C;                                             //allume D2 et D5
    }
    else if(azimut>=255 && azimut<=285)                           //OUEST
    {
        P1OUT = 0x08;                                             //allume D5
    }
    else if(azimut>285 && azimut<345)                             //Nord EST
    {
        P1OUT = 0x12;                                             //allume D3 D4
    }
    else                                                          //Si angle correspond à aucune valeur
    { 
        P1OUT = 1;                                                //Allumer Diode centrale ==> aucune information reçu ou exploitable
    }
}
