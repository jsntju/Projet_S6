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
#include "ecran.h"

/*Données trames GPS*/
char* buf;
char* trameGGA;

/* --- Prototype de fonctions --- */

void aff_config_uart (void);

/* --- Variable globale --- */

void main(void){ 
    int i;

    // initialisation de la clock
    // arrêt du watchdog           
    WDTCTL = WDTPW + WDTHOLD;            // 0x5A00 + 0x0080 : Mot de passe + Watchdog Timer(WDT) arrété
    
    // sequence à faire pour le switch entre 2 clock (cf page 4-12 du guide de l'utilisateur)
    // Configuration du Basic Clock System Control
    BCSCTL1 &= ~XT2OFF;                  // XT2 est allumé
    // verification nécéssaire du OSCFault lorsqu'on modifie la config des Clock
    do {
        //debug_printf("IFG1  = %x\n",IFG1);
        IFG1 &= ~OFIFG;                  // Clear OSCFault flag
        for (i = 0xFF; i > 0; i--);      // Time for flag to set
    }while ( (IFG1 & OFIFG) != 0);       // OSCFault flag still set?               
    BCSCTL2 |= SELM1+SELS;               // MCLK = SMCLK = XT2 (safe) avec SMCLK = 8Mhz  
    
    initialisation_LED_PAD();
    
    // initialisation du port P4
    // P4.0,1 et 2 en sortie (0 : ENABLE_GPS, 1:RESET_LCD, 2 : CMD_SWITCH)
    // on met RESET_LCD à 1 puis 0 pendant A PRECISER puis on la remet à 1
    // pour initialiser la communication entre l'écran et l'µc
    P4DIR = 0x07;
    P4OUT = BIT2 | BIT1;
    delay(100);
    P4OUT &= ~BIT1 ;                     
    delay(100);
    P4OUT |= BIT1 ;
    delay(10000);
    delay(10000);
    delay(10000);
    delay(10000);
    delay(10000);
    delay(10000);
    delay(10000);
    delay(10000);
    delay(10000);
    // on communique avec l'USB 
    P1OUT = 0x1E;                        // led0 éteint : initialisation du port 4 terminé

    // initialisation de l'uart 0 et 1
    init_uart0();   
    P1OUT = 0x1C;                        // led1 éteint : initialisation de l'UART 0 et 1 terminé 

    // verification de la config des registres UART pour la communication RS232
    //aff_config_uart();
    init_ecran();
    P1OUT = 0x18;                        // led2 éteint : initialisation de l'écran
    P1OUT = 0x00;                        // initialisation terminé
    
    P4OUT &= ~BIT2 & ~BIT0 ; 
    //activer_communication_GPS();
    //activer_communication_USB();
    choix_ecran = 1;
    // programme principal
    while(1){
        /*selec_trame_gga(buf_0);       //REMETRE LE CHAR BUF_0
        h = search(trame,',',0)+1;
        //debug_printf("H AV= %i\n", h);
        debug_printf("TRAME: %s \n",trame);*/
        choix_main_pad();
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
