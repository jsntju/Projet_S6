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

#include "gps.h"

/* --- Prototype de fonctions -- */
void initialisation_LED_PAD(void);
void activer_communication_USB(void);
void activer_communication_GPS(void);

char buf[1000];
volatile flag = 0;

void main(void){ 
    // initialisation
    WDTCTL = WDTPW + WDTHOLD;             // Stop watchdog timer
    
    initialisation_LED_PAD();
    
    P5DIR |= 0x70;                        // P5.4 output direction
    P5SEL |= 0x70;                        // P5.4 = MCLK option select
    
    activer_communication_GPS();
    init_uart0();  
    
    debug_printf("\nUCTL0,UTCTL0,URCTL0 %x | %x | %x",UCTL0,UTCTL0,URCTL0);
    debug_printf("\nUBR00,UBR10,UMCTL0  %x | %x | %x",UBR00,UBR10,UMCTL0);
    debug_printf("\nRXBUF0,TXBUF0       %x | %x ",RXBUF0,TXBUF0);
    debug_printf("\nME1,ME2,IE1,IE2     %x | %x | %x | %x",ME1,ME2,IE1,IE2);
    debug_printf("\nIFG1,IFG2           %x | %x ",IFG1,IFG2);
    
    // programme principal
    while(1){
        unsigned int i;
        //usart0_rx();
        /*i = 1000;                          // Delay
        do (i--);
        while (i != 0);*/

        debug_printf("\n %x | %2x",RXBUF0,URCTL0);
        debug_printf("\n %x | %2x",TXBUF0,UTCTL0);
        P1OUT ^= 0x01;
        /*while ((IFG1 & UTXIFG0) == 0);        // USART0 TX buffer ready?
        TXBUF0 = RXBUF0;                      // RXBUF0 to TXBUF0
        */
        
        //P3OUT = 0xA0&(~P3IN) ; 
        /*if(flag == 1){
            P1OUT ^= 0x01;
            for(i=0 ; i<1000 ; i++){
                debug_printf(" %c ",buf[i]);
            }
            flag = 0;
        }*/
        //if(BITCLK%1) P1OUT ^= 0x01;            
    }
    debug_exit(0);
}

void initialisation_LED_PAD(void){
    P1DIR |= 0x1F;      // = (0001 1111)2 : met les bits P1.0 à P1.4 (LEDs) en sortie 
    P2DIR &= 0xE0;      // = (1110 0000)2 : met les bits P2.0 à P2.4 (PAD) en sortie

    P1OUT &= 0xE0;      // les 5 LEDs sont mis à 0
}

void activer_communication_USB(void){
    P4OUT |= 0x04;      // = (0000 0100)2 : P4.2 = 1 -> le PORT SERIE 0 communique avec l'USB  
    if((P4OUT&BIT2)!= 0)debug_printf("Communication USB activé\n");
}

void activer_communication_GPS(void){
    P4OUT &= 0xFB;      // = (1111 1011)2 : P4.2 = 0 -> le PORT SERIE 0 communique avec le GPS  
    if((P4OUT&BIT2)== 0) debug_printf("Communication GPS activé\n");
}

void usart0_rx (void) __interrupt[USART0RX_VECTOR]
{
    /*unsigned int q=0;
    char tmp;
    
    while (!(IFG1 & URXIFG0));        // USART0 TX buffer ready?
    tmp = RXBUF0;                      // RXBUF0 to TXBUF0
    if(q<10){
        buf[q] = tmp;
        q++;
    } else {
        q=0;
        flag = 1;
    }*/
    while ((IFG1 & UTXIFG0) == 0);        // USART0 TX buffer ready?
    TXBUF0 = RXBUF0;                      // RXBUF0 to TXBUF0

}
