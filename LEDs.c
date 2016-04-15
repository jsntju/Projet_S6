#include <__cross_studio_io.h> 
#include  <msp430x16x.h> 

 
 

void main(void){  

  WDTCTL = WDTPW + WDTHOLD;             // Stop watchdog timer 

  P1DIR |= 0x1F;                        // Set P1.0 to output direction 

  P2DIR &= 0xE0; 

  P1OUT &= 0xE0; 

   

  for (;;){ 

      unsigned int i; 

      debug_printf("%x\n",~P2IN); 

   

      //P1OUT = ~P2IN;                      // Toggle P1.0 using exclusive-OR 

      /* 

      i = 50000;                          // Delay 

      do (i--); 

      while (i != 0);*/ 

   

      switch(~P2IN&0x0000001F){ 

          case (0x01): 

              P1OUT &= 0xE0; 

              P1OUT |= 0x01; 

              break; 

          case (0x02): 

              P1OUT &= 0xE0; 

              P1OUT |= 0x04; 

              break; 

          case (0x04): 

              P1OUT &= 0xE0; 

              P1OUT |= 0x02; 

              break; 

          case (0x08): 

              P1OUT &= 0xE0; 

              P1OUT |= 0x10; 

              break; 

          case (0x10): 

              P1OUT &= 0xE0; 

              P1OUT |= 0x08; 

              break; 

      } 

  } 

  //debug_exit(0); 

} 
