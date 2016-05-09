
#include <__cross_studio_io.h>
#include <msp430x16x.h>
#include <string.h>
#include <gps.h>
#include <math.h>



/* @brief: Transformer les coordonnées en Degres minute senconde (DMS) en degres decimaux (DD)
*   @Parametre:
  - La chaine de caractere contenat la coordonnée
  - Lati_longi (booleen): si 1 -> latitude, si 0 ->longitude
  - cardi: Point cardinaux: NS pour latitude et EW pour longitude
*   @Retourne:
  - DD: la coordonnée en de gres decimeaux
*/
float DMS_en_DD(char* coord,int lati_longi, char* cardi)
{
  float DD;                                                   //coordonnée decimale
  char* tmp;                                                  //Temporaire 
  int degre, min, sec;
//      Extraction des degres, minutes et secondes
    if(lati_longi)                                             //Latitude
    {
      degre = atoi(substr(tmp,coord,0,1));                    //degres = 2 permieres variables (atoi transforme en int)
      if(strcmp(cardi,"S") == 0)
      {
        degre = -degre;
      }
      min = atoi(substr(tmp,coord,2,3));
      sec = atoi(substr(tmp,coord,5,search(coord,'\0',5)));   //sec jusqu'à la fin de la chaine
    }
    else                                                      //Longitude
    {
      degre = atoi(substr(tmp,coord,0,2));                    //degres = 3 permieres variables (atoi transforme en int)
      if(strcmp(cardi,"O") == 0)
      {
        degre = -degre;
      }

      min = atoi(substr(tmp,coord,3,4));
      sec = atoi(substr(tmp,coord,6,search(coord,'\0',6)));   //sec jusqu'à la fin de la chaine
    }

    DD = degre+(min/60)+(sec/3600);                           //Calcul degres decimaux
  return(DD);
}



/*
* @Brief: Calcul l'angle entre le pole Nord et un point géographique
*   @Parametres:
    - Longi: longitude en DD du point géographique
    - Lati: latitude en DD du point géographique
*   @Retourne: azimut -> l'angle du vecteur
*/
float direction_nord(float longi, float lati)
{
  float longi_nord = 82.116667;                               //coordonnée DD pole Nord
  float lati_nord = -114.068888;
  float azimut;                                               //angle poleNord-point
  float x, y;

  x = cos(lati)*sin(lati_nord)-sin(lati)*cos(lati_nord)*cos(longi_nord-longi);
  y = sin(longi_nord-longi)*cos(lati_nord);
  azimut = atan2(y,x);                                        //Calcul de l'angle en radian

  return(azimut);
}



/*
* @Brief: Allumer LED en fonction de la direction
*   @Parametres: 
    - azimut: angle (direction) entre le pole nord et le points geographique
*/
void boussole_LED(float azimut)
{
  azimut = azimut*(180/3.14);                               //transforme en degres
  if(azimut>=345 && azimut<=15)                             //Nord
  {
    P1OUT &= 0xE0; 
    P1OUT |= 0x04;                                          //allume D3
  }
  if(azimut>15 && azimut<75)                                //Nord EST
  {
     //P1OUT = 00000111;                                    //allume D3 D4
  }
  if(azimut>=75 && azimut<=105)                             //EST
  {
    P1OUT &= 0xE0; 
    P1OUT |= 0x08;                                          //allume D4
  }
  if(azimut>105 && azimut<165)                              //SUD EST
  {
    //P1OUT = 00000110;                                     //allume D4 D2
  }
  if(azimut>=165 && azimut<=195)                            //SUD 
  {
    P1OUT &= 0xE0; 
    P1OUT |= 0x02;                                          //allume D2
  }
  if(azimut>195 && azimut<255)                              //SUD OUEST
  {
    //allume D2 et D5
  }
  if(azimut>=255 && azimut<=285)                            //OUEST
  {
    P1OUT &= 0xE0; 
    P1OUT |= 0x10;                                          //allume D5
  }
  if(azimut>285 && azimut<345)                              //Nord OUEST
  {
    //allume D5 ET D3
  }
}
