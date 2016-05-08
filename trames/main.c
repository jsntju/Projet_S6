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
 * @version 1.0.0 / 11/04/2016
 * 
 * @todo - 
 * 
 * @bug - 
 */

/**
 * @file main.c
 * @brief Le programme permet de A DECRIRE !!!! 
 * 
 * A DECRIRE !!!!  
 */

#include <__cross_studio_io.h>
#include  <msp430x16x.h>
#include <string.h>

#include "gps.h"
#include "boussole.h"
#include "io_led_pad.h"

/*Données trames GPS*/
char* buf;
char* trameGGA;


/* --- Prototype de fonctions --- */

void aff_config_uart (void);

/* --- Variable globale --- */

void main(void){ 
  int i;
    // initialisation
    WDTCTL = WDTPW + WDTHOLD;             // Stop watchdog timer
    BCSCTL1 &= ~XT2OFF;                  // XT2 est allumé
    // verification nécéssaire du OSCFault lorsqu'on modifie la config des Clock
    do {
        //debug_printf("IFG1  = %x\n",IFG1);
        IFG1 &= ~OFIFG;                  // Clear OSCFault flag
        for (i = 0xFF; i > 0; i--);      // Time for flag to set
    }while ( (IFG1 & OFIFG) != 0);       // OSCFault flag still set?    
                
    BCSCTL2 |= SELM1+SELS;               // MCLK = SMCLK = XT2 (safe) avec SMCLK = 8Mhz       
    
    initialisation_LED_PAD();
    
    // Clock MCLK en sortie sur le port P5.4 
    P5DIR |= 0x70;                        // P5.4 output direction
    P5SEL |= 0x70;                        // P5.4 = MCLK option select
    
    //activer_communication_GPS();
    //activer_communication_USB();
    init_uart0();   
    // verification de la config des registres UART pour la communication RS232
    //aff_config_uart();

    // programme principal
    while(1){
        //unsigned int i;
        /*i = 1000;                          // Delay
        do (i--);
        while (i != 0);*/
        /*
        debug_printf("\n %s ",PORT2_VECTOR);
        P1OUT ^= 0x01;
        */

// ********** TRAMES GPS:
     //buf = select_buf0();
     //debug_printf("buf main: %s ",buf);

      selec_trame_gga(/*buf_0*/);       //REMETRE LE CHAR BUF_0
      h = search(trame,',',0)+1;
      //debug_printf("H AV= %i\n", h);
      debug_printf("TRAME: %s \n",trame);

      /*selec_objet(UTC);                               /*Séparation de la trame en données*/  
      /*selec_objet(latitude);
      /*selec_objet(NS);
      selec_objet(longitude);
      selec_objet(EW);
      selec_objet(Pos_ind);
      selec_objet(nb_sat);
      selec_objet(HDOP);
      selec_objet(alti);
      selec_objet(unit_alti);
      selec_objet(geoid);
      selec_objet(unit_geo);
      selec_objet(age_diff);
      selec_objet(id_station);  
      selec_objet(checksum); */

      /*debug_printf("UTC: %s\n", UTC);
      debug_printf("lati: %s\n", latitude);
      debug_printf("H AP= %i\n", h);*/

     // initialise_obj(UTC,30);
      /*initialise_obj(latitude,10);
      initialise_obj(NS,2);
      initialise_obj(longitude,10);
      initialise_obj(EW,2);
      initialise_obj(Pos_ind,10);
      initialise_obj(nb_sat,10);
      initialise_obj(HDOP,10);
      initialise_obj(alti,10);
      initialise_obj(unit_alti,2);
      initialise_obj(geoid,10);
      initialise_obj(unit_geo,2);
      initialise_obj(age_diff,10);
      initialise_obj(id_station,10);  
      initialise_obj(checksum,10);*/
       
    }
    debug_exit(0);

    
}

void aff_config_uart (void){
    debug_printf("\n UCTL0,UTCTL0,URCTL0 %x | %x | %x",UCTL0,UTCTL0,URCTL0);
    debug_printf("\n UBR00,UBR10,UMCTL0  %x | %x | %x",UBR00,UBR10,UMCTL0);
    debug_printf("\n RXBUF0,TXBUF0       %x | %x ",RXBUF0,TXBUF0);
    debug_printf("\n UCTL1,UTCTL1,URCTL1 %x | %x | %x",UCTL1,UTCTL1,URCTL1);
    debug_printf("\n UBR01,UBR11,UMCTL1  %x | %x | %x",UBR01,UBR11,UMCTL1);
    debug_printf("\n RXBUF1,TXBUF1       %x | %x ",RXBUF1,TXBUF1);
    debug_printf("\n ME1,ME2,IE1,IE2     %x | %x | %x | %x",ME1,ME2,IE1,IE2);
    debug_printf("\n IFG1,IFG2           %x | %x ",IFG1,IFG2);
}
