
#include <__cross_studio_io.h>
#include <msp430x16x.h>
#include <string.h>

#include "io_led_pad.h"
int GPS_USB_ENABLE = 0;                         // à 0 si GPS activé ou 1 si USB activé


/*------------------ Variable globale programme*/
char buf_0[200];
char trame[100];
char UTC[10];
char latitude[10];
char NS[2];                                   //Nord ou Sud
char longitude[10];
char EW[2];                                   //Est ou Ouest
char Pos_ind[10];                             //Position fix indicator
char nb_sat[10];                              //nombre de sattelites
char HDOP[10];                                //horizontal dilution of precision
char alti[10];                                //altitude
char unit_alti[2];                            //unité  d'altitude
char geoid[10];                               //geoid separation
char unit_geo[2];                             //unité  geod separation
char age_diff[10];                            // Age of Diff. Corr
char id_station[10];                          // Diff. Ref. Station ID
char checksum[10];
char buf_1[200];
int h;


/*------------------ Variable globale gps.c*/
int signal;
unsigned int est_trameGGA;
unsigned char num_octet1,num_octet2;
int rep_ecran;
int flag_1;


/*----------------------------------------- FONCTION POUR CHAR --------------------------------------------------------*/
/* @Brief: copie une chaine de caracteres entre deux rangs donnés
* Parametres: 
  - buf: nouvelle chaine de caractere
  - s : chaine de caractere à copier
  - beg: rang à partir duquelle on souhaite copier la chaine
  - beg: rang jusqu'où on souhaite copier la chaine
* Retourne:
  - La nouvelle chaine buf
*/
char * substr(char *buf, const char *s, size_t beg, size_t len)
{
        memcpy(buf, s + beg, len);
        buf[len] = '\0';
        return (buf);
}

/* @Brief: renvoit la position d' un caractere à partir d'un rang donné
* Parametres: 
  - s : chaine de caractere 
  - carac: caratere chercher
  - debut: rang de la chaine de caractere
* Retourne:
  - Le rang (int) où se trouve le caractere
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
/*@Brief: initialise l'uart
*/
void init_uart0(void){
    unsigned int i;
    
    //  Configuration de l'USART0
    UCTL0 = SWRST;                       // mise à 0 de l'UART0 
    UCTL0 |= CHAR;                       // Format des caractères sur 8-bits
    UTCTL0 |= SSEL1;                     // UCLK = SMCLK
    
    //  Configuration de l'USART1
    UCTL1 = SWRST;                       // mise à 0 de l'UART1 
    UCTL1 |= CHAR;                       // Format des caractères sur 8-bits
    UTCTL1 |= SSEL1;                     // UCLK = SMCLK

    // Config du taux de transmission pour 9600 bauds
    U0BR0 = 0x41;                        // 8MHz/9600 ~ 833
    U0BR1 = 0x03;                      
    UMCTL0 = 0x21;                       // no modulation

    U1BR0 = 0x41;                        // 8MHz/9600 ~ 833
    U1BR1 = 0x03;                      
    UMCTL1 = 0x21;                       // no modulation

    UCTL0 &= ~SWRST;                     // Fin de l'état de reset de l'UART0
    UCTL1 &= ~SWRST;                     // Fin de l'état de reset de l'UART1

    // activation transmission et récéption
    ME1 |= URXE0 | UTXE0;                // Enable USART0 TXD/RXD
    IE1 |= URXIE0 ;                      // Enable USART0 RX interrup

    ME2 |= URXE1 | UTXE1;                // Enable USART1 TXD/RXD
    IE2 |= URXIE1 ;                      // Enable USART1 RX interrup

    // Config des ports
    P3SEL |= 0x30;                       // P3.4,5 en mode USART0 TXD/RXD
    P3DIR |= 0x20;                       // P3.4 output direction
    P3DIR &= ~0x10;                      // P3.5 input direction
    
    P3SEL |= 0xC0;                       // P3.6,7 en mode USART1 TXD/RXD
    P3DIR |= 0x80;                       // P3.6 output direction
    P3DIR &= ~0x40;                      // P3.7 output direction
     
    // activation des interruptions pour la réception
    _EINT();                             // Enable interrupts
}




/*@brief: Recupére les informations du buffer et stock les informations dans buf_0.
* Et copie les données à exploiter dans buf_1.
*/
void usart0_rx (void) __interrupt[USART0RX_VECTOR]{  
    char c;
    num_octet1 = 0;                     //compteur d'octets
    memset(buf_0, 0, sizeof buf_0);     //Déclare la memoire alloué à buf_0
    
    while (num_octet1 <= (100-1)) {     // tant que le buffer n'est pas rempli
        while (!(IFG1 & URXIFG0)) {}    // tant que flag interrupt en cours, on attend
        buf_0[num_octet1] = U0RXBUF;    // ou RXBUF0;
        num_octet1++;
    }
    buf_1[0] = '\0';                    //Initialise buf_1  
    strcpy(buf_1,buf_0);                //Copie buf_0 dans buf_1
    IE1 &= ~URXIE0 ;                    //
    IFG1 &= ~(IFG1 & URXIFG0);          //
}

void usart1_rx (void) __interrupt[USART1RX_VECTOR]{  
    //P1OUT ^= 0x02;
    // init buffer
    //while ((IFG2 & URXIFG1)) {}
    //c = U1RXBUF;
    //debug_printf("0R: %s\n", c); 
    //debug_printf("1R: %c | %i\n",RXBUF1, RXBUF1);  
    flag_1 = 1;
    rep_ecran = RXBUF1; 
    //debug_printf("1R: %i\n", c);  
    //while (!(IFG1 & UTXIFG0)) {}
    //TXBUF0 = rep_ecran ;
    //TXBUF0 = RXBUF1 ;
    //P1OUT ^= 0x02;
}





/*-------------------------------- ACTIVER COMMUNICATIONS ------------------------------------------------*/
/* @brief: active la liason entre le microprocesseur et l'usb
*/
void activer_communication_USB(void){
    // on desactive le GPS, P4,0
    P4OUT &= ~BIT0;
    P4OUT = BIT2;                           // = (0000 0100)2 : P4.2 = 1 -> le PORT SERIE 0 communique avec l'USB  
    if((P4OUT&BIT2)!= 0)debug_printf("Communication USB activé\n");
    GPS_USB_ENABLE = 1;
}

/* @brief: active la liason entre le microprocesseur et le GPS
*/
void activer_communication_GPS(void){
    P4OUT |= BIT0 ;                         // = (1111 1011)2 : P4.2 = 0 -> le PORT SERIE 0 communique avec le GPS  
    P4OUT &= ~BIT2;
    if((P4OUT&BIT2)== 0) debug_printf("Communication GPS activé\n");
    GPS_USB_ENABLE = 0;
}



/*----------------------------------------- SELECTION TRAMES GGA -------------------------------------------------------*/
/* @Brief: Selectionne les trames GGA
* Utilise buf_1 et découpe dans un nouveaux Char la trame GGA
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
* - char * objet: chaine de caractere à selectionner
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
          h++;                                          //h: variable globale => rang de départ pour copier les objets
       }
       h++;                                             //h => utilisé à chaque rappele de fonction
    }
    return(objet);
}

/* @Brief: Initialise les objets de la trame
* permet de reécrire dans le char sans garder les informations précédentes
* @ Parametre: 
* - Objet (chaine de caracteres)
* - Taille: taille maximum du char
* @ Retourne: Objet effacé 
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


/* @Brief: Indique si la trame envoyé est correcte et donc utilisable
* Retourne 1 si trame correcte 
* 0 sinon*/
int GGA_OK (void)
{    
   signal = 0;                                    //fonction atoi: transforme char en int  
  if((atoi(Pos_ind) != 0) && (atoi(nb_sat) >= 4)) //4 statellite et position (flag) doit être à plus de 0              
  {
      signal = 1;                               
  }
  return(signal);
}
