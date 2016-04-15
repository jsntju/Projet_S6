/*
* @file: Leds_pad.c 
* @Brief: Fonctions permettant d'allumer la LED correspondant à la direction du PAD (haut, bas, droite, gauche, centre).
*/


#include <__cross_studio_io.h>        //Pour le printf
#include  <msp430x16x.h> 

 
 

void main(void){  

  WDTCTL = WDTPW + WDTHOLD;             // Stop watchdog timer 
  P1DIR |= 0x1F;                        // P1 mis en sortie
  P2DIR &= 0xE0;                        // P2 mis en entrée
  P1OUT &= 0xE0;                        // Initialise la sortie de P1 à 0

   

  for (;;){ 
      unsigned int i; 

      //debug_printf("%x\n",~P2IN); 
      //P1OUT = ~P2IN;                      // Led en fonction de la sortie de P2
      /* 
      i = 50000;                          // Delay 
      do (i--); 
      while (i != 0);*/ 

   

      switch(~P2IN&0x0000001F){ 
          case (0x01):                    //Centre
              P1OUT &= 0xE0; 
              P1OUT |= 0x01; 
              break; 

          case (0x02):                   // Haut
              P1OUT &= 0xE0; 
              P1OUT |= 0x04; 
              break; 

          case (0x04):                   // Bas 
              P1OUT &= 0xE0; 
              P1OUT |= 0x02; 
              break; 

          case (0x08):                  // Gauche
              P1OUT &= 0xE0; 
              P1OUT |= 0x10; 
              break; 

          case (0x10):                 // Droite
              P1OUT &= 0xE0; 
              P1OUT |= 0x08; 
              break; 
      } 
  } 
  //debug_exit(0); 
} 
