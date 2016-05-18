/**
 * Polytech Marseille
 * Case 925 - 163, avenue de Luminy
 * 13288 Marseille CEDEX 9
 * 
 * Ce fichier est l'oeuvre d'�l�ves de Polytech Marseille. Il ne peut �tre 
 * reproduit, utilis� ou modifi� sans l'avis express de ses auteurs.
 */

/**
 * @author BISSUEL Valentin <valentin.bissuel@etu.univ-amu.fr>
 * @author DENIS Justine <justine.denis@etu.univ-amu.fr>
 *
 * @version 1.5.0 / 11/05/2016
 * 
 * @todo - 
 * 
 * @bug - 
 */

/**
 * @file gps.c 
 * @brief Le programme permet de g�rer de recevoir les donn�es de l'�cran et du GPS
 * *  Et de lire les trames GGA ainsi que de les s�quencer
 */
 
#include <__cross_studio_io.h>
#include <msp430x16x.h>
#include <string.h>
#include <stdlib.h>

#include "io_led_pad.h"
int GPS_USB_ENABLE = 0;                         // � 0 si GPS activ� ou 1 si USB activ�


/*------------------ Variable globale programme*/
char buf_0[200];
char trame[100];                            //chaine contenant la trame GGA
char UTC[30];
char latitude[10];
char NS[2];                                   //Nord ou Sud
char longitude[10];
char EW[2];                                   //Est ou Ouest
char Pos_ind[10];                             //Position fix indicator
char nb_sat[10];                              //nombre de sattelites
char HDOP[10];                                //horizontal dilution of precision
char alti[10];                                //altitude
char unit_alti[2];                            //unit�  d'altitude
char geoid[10];                               //geoid separation
char unit_geo[2];                             //unit�  geod separation
char age_diff[10];                            // Age of Diff. Corr
char id_station[10];                          // Diff. Ref. Station ID
char checksum[10];
char buf_1[200];
char trame_vtg[50];                           //chaine contenant la trame VTG
int h;


/*------------------ Variable globale gps.c*/
int signal;
unsigned int est_trameGGA;
unsigned char num_octet1,num_octet2;
int rep_ecran;
int flag_1;


/*----------------------------------------- FONCTION POUR CHAR --------------------------------------------------------*/
/* @Brief: copie une chaine de caracteres entre deux rangs donn�s
* Parametres: 
  - buf: nouvelle chaine de caractere
  - s : chaine de caractere � copier
  - beg: rang � partir duquelle on souhaite copier la chaine
  - beg: rang jusqu'o� on souhaite copier la chaine
* Retourne:
  - La nouvelle chaine buf
*/
char * substr(char *buf, const char *s, size_t beg, size_t len)
{
        memcpy(buf, s + beg, len);
        buf[len] = '\0';
        return (buf);
}

/* @Brief: renvoit la position d' un caractere � partir d'un rang donn�
* Parametres: 
  - s : chaine de caractere 
  - carac: caratere chercher
  - debut: rang de la chaine de caractere
* Retourne:
  - Le rang (int) o� se trouve le caractere
*/
int search (char *s, char carac, int debut)
{
    int i;
    for(i = debut; s[i] != '\0';i++)
        if(s[i] == carac)
            return i;
    return 0;
}





/*---------------------------- INITIALISATION UART--------------------------------*/
/*@Brief: initialise les uart 0 et 1
*/
void init_uart(void){
    unsigned int i;
    
    //  Configuration de l'USART0
    UCTL0 = SWRST;                       // mise � 0 de l'UART0 
    UCTL0 |= CHAR;                       // Format des caract�res sur 8-bits
    UTCTL0 |= SSEL1;                     // UCLK = SMCLK
    
    //  Configuration de l'USART1
    UCTL1 = SWRST;                       // mise � 0 de l'UART1 
    UCTL1 |= CHAR;                       // Format des caract�res sur 8-bits
    UTCTL1 |= SSEL1;                     // UCLK = SMCLK

    // Config du taux de transmission pour 9600 bauds
    U0BR0 = 0x41;                        // 8MHz/9600 ~ 833
    U0BR1 = 0x03;                      
    UMCTL0 = 0x21;                       // no modulation

    U1BR0 = 0x41;                        // 8MHz/9600 ~ 833
    U1BR1 = 0x03;                      
    UMCTL1 = 0x21;                       // no modulation

    UCTL0 &= ~SWRST;                     // Fin de l'�tat de reset de l'UART0
    UCTL1 &= ~SWRST;                     // Fin de l'�tat de reset de l'UART1

    // activation transmission et r�c�ption
    ME1 |= URXE0 | UTXE0;                // Enable USART0 TXD/RXD
    IE1 &= ~URXIE0 ;                     // Disable USART0 RX interrup

    ME2 |= URXE1 | UTXE1;                // Enable USART1 TXD/RXD
    IE2 |= URXIE1 ;                      // Enable USART1 RX interrup

    // Config des ports
    P3SEL |= 0x30;                       // P3.4,5 en mode USART0 TXD/RXD
    P3DIR |= 0x20;                       // P3.4 output direction
    P3DIR &= ~0x10;                      // P3.5 input direction
    
    P3SEL |= 0xC0;                       // P3.6,7 en mode USART1 TXD/RXD
    P3DIR |= 0x80;                       // P3.6 output direction
    P3DIR &= ~0x40;                      // P3.7 output direction
    
    // activation des interruptions pour la r�ception
    _EINT();                             // Enable interrupts
}

/*@brief: Recup�re les informations du buffer et stock les informations dans buf_0.
* Et copie les donn�es � exploiter dans buf_1.
*/
void usart0_rx (void) __interrupt[USART0RX_VECTOR]{  
    char c;
    num_octet1 = 0;                     // compteur d'octets
    memset(buf_0, 0, sizeof buf_0);     // D�clare la memoire allou� � buf_0
    
    while (num_octet1 <= (200-1)) {     // tant que le buffer n'est pas rempli
        while (!(IFG1 & URXIFG0)) {}    // tant que flag interrupt en cours, on attend
        buf_0[num_octet1] = U0RXBUF;    // ou RXBUF0;
        num_octet1++;
    }
    buf_1[0] = '\0';                    // Initialise buf_1  
    strcpy(buf_1,buf_0);                // Copie buf_0 dans buf_1
    IE1 &= ~URXIE0 ;                    //      
}

/*@Brief: Stocke la r�ponse de l'�cran dans rep_ecran
*/
void usart1_rx (void) __interrupt[USART1RX_VECTOR]{  
    flag_1 = 1;
    rep_ecran = RXBUF1; 
}

/*-------------------------------- ACTIVER COMMUNICATIONS ------------------------------------------------*/
/* @brief: active la liason entre le microprocesseur et l'usb
*/
void activer_communication_USB(void){
    // on desactive le GPS, P4,0
    P4OUT &= ~BIT0;
    P4OUT = BIT2;                           // = (0000 0100)2 : P4.2 = 1 -> le PORT SERIE 0 communique avec l'USB  
}

/* @brief: active la liason entre le microprocesseur et le GPS
*/
void activer_communication_GPS(void){
    P4OUT |= BIT0 ;                         // = (1111 1011)2 : P4.2 = 0 -> le PORT SERIE 0 communique avec le GPS  
    P4OUT &= ~BIT2;
}

/*----------------------------------------- SELECTION TRAMES GGA -------------------------------------------------------*/
/* @Brief: Selectionne les trames GGA
* Utilise buf_1 et d�coupe dans un nouveaux Char la trame GGA
* Retourne une trame GGA
*/
void selec_trame_gga(void)
{
  char* ptr;
  int retour_chariot;
  ptr = strstr(buf_1,"$GPGGA");                         //Recherche $GPGGA (debut de la trame)
  retour_chariot = search(ptr,10,0);                    //Fin de la trame: retour chariot (10 en ASCII)
  substr(trame,ptr,0,retour_chariot);                   //copie la trame GGA dans trame
}


/* @ Selectionne les objets de la trame (entre les virgules)
* @Parametres: 
* - char * objet: chaine de caractere � selectionner
* @ retourne:
* - L'objet: chaine de caracteres
*/
char* selec_objet (char * objet)
{   if(trame[0] != '\0')                                //Si la trame est vide, on fait rien
    {
    int j=0;                                            
       while (trame[h] != ',' && trame[h] != '\0')      //Copie la chaine tant qu'il y a pas de virgule ou fin de la chaine
       {
          objet[j] = trame[h];
          j++;
          h++;                                          //h: variable globale => rang de d�part pour copier les objets
       }
       h++;                                             //h => utilis� � chaque rappele de fonction
    }
    return(objet);
}

/* @Brief: Initialise les objets de la trame
* permet de re�crire dans le char sans garder les informations pr�c�dentes
* @ Parametre: 
* - Objet (chaine de caracteres)
* - Taille: taille maximum du char
* @ Retourne: Objet effac� 
*/
char * initialise_obj (char * objet, int taille)
{
  int i;
  for(i=0; i<taille; i++)
  {
    objet[i] = '\0';                            //suprimme les information dans la chaine de caractere
  }
  return(objet);
}


/* @Brief: Indique si la trame envoy� est correcte et donc utilisable
* Retourne 1 si trame correcte 
* 0 sinon*/
int GGA_OK (void)
{    
   signal = 0;                                    //fonction atoi: transforme char en int  
  if((atoi(Pos_ind) != 0) && (atoi(nb_sat) >= 4)) //4 statellite et position (flag) doit �tre � plus de 0              
  {
      signal = 1;                               
  }
  return(signal);
}
