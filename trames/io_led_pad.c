
#include <__cross_studio_io.h>
#include <msp430x16x.h>
#include <string.h>

void initialisation_LED_PAD(void){
    P1DIR |= 0x1F;      // = (0001 1111)2 : met les bits P1.0 à P1.4 (LEDs) en sortie 
    
    P2DIR &= 0xE0;      // = (1110 0000)2 : met les bits P2.0 à P2.4 (PAD) en entrée
    P2IE = 0x1F;        // = (1111 1111)2 : met les bits le port 2 en interruption

    P1OUT &= 0xE0;      // les 5 LEDs sont mis à 0
}

void P2_Pad (void) __interrupt[ PORT2_VECTOR ]{  
	debug_printf("interruption P2\n");  
        P1OUT = ~P2IN;
	// reset buffer
	//debug_printf("%s\n", P2IN);                   // RXBUF0 to TXBUF0
}
