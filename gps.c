//******************************************************************************
//  MSP-FET430P140 Demo - USART0 UART 9600 Echo ISR, HF XTAL ACLK 
//
//  Description; Echo a received character, RX ISR used. Normal mode is LPM0, 
//  USART0 RX interrupt triggers TX Echo. Though not required, MCLK = LFXT1  
//  ACLK = MCLK = UCLK0 = LFXT1 = 3.58MHz
//  Baud rate divider with 3.58Mhz XTAL @9600 = 3.58MHz/9600 = 372 (0174h)
//  //*An external 3.58Mhz XTAL on XIN XOUT is required for ACLK*//	  
//
//                MSP430F149   
//             -----------------
//         /|\|              XIN|-  
//          | |                 | 3.58MHz 
//          --|RST          XOUT|-
//            |                 |
//            |             P3.4|------------> 
//            |                 | 9600 - 8N1
//            |             P3.5|<------------
//
//       
//  M. Buccini
//  Texas Instruments, Inc
//  February 2002
//  Built with IAR Embedded Workbench Version: 1.25A
//******************************************************************************
#include <__cross_studio_io.h>
#include <msp430x16x.h>



void init_uart0(void){
    unsigned int i;
    // arrêt du watchdog
    WDTCTL = WDTPW + WDTHOLD;             // 0x5A00 + 0x0080 : Mot de passe + Watchdog Timer(WDT) arrété
                         
    // sequence à faire pour le switch entre 2 clock (cf page 4-12 du guide de l'utilisateur)
    // Configuration du Basic Clock System Control
    BCSCTL1 &= ~XT2OFF;                    // XT2 est allumé
    debug_printf("BCSCTL1  = %2x\n",BCSCTL1);
    // verification nécéssaire du OSCFault lorsqu'on modifie la config des Clock
    do {
        debug_printf("IFG1  = %x\n",IFG1);
        IFG1 &= ~OFIFG;                   // Clear OSCFault flag
        for (i = 0xFF; i > 0; i--);       // Time for flag to set
    }while ((IFG1&OFIFG) != 0);           // OSCFault flag still set?                
         
    BCSCTL2 |= SELM1+SELS;                  // MCLK = SMCLK = XT2 (safe) avec SMCLK = 8Mhz
    debug_printf("BCSCTL2  = %2x\n",BCSCTL2);         

    //  Configuration de l'USART0
    UCTL0 |= SWRST;                        // mise à 0 de l'UART (la doc dit de le faire mais l'exemple ne le fait pas! WTF)
    UCTL0 |= CHAR;                         // Format des caractères sur 8-bits
    UTCTL0 |= SSEL1;                       // UCLK = SMCLK
    // Config du taux de transmission pour 9600 bauds
    UBR00 = 0x41;                         // 8MHz/9600 ~ 833
    UBR10 = 0x03;                         //
    /*
    UBR00 = 0x03;                         // 32768Hz/9600 ~ 3
    UBR10 = 0x00;                         //
    */
    UMCTL0 = 0x00;                        // no modulation
    UCTL0 &= ~SWRST;                      // Fin de l'état de reset de l'UART

    // activation transmission et récéption
    ME1 |= URXE0;                         // Enable USART0 RXD
    
    // Config des ports
    P3SEL |= 0x30;                        // P3.4,5 = USART0 TXD/RXD
    P3DIR |= 0x10;                        // P3.4 output direction
    P3DIR &= ~0x20;                       // P3.5 input direction
    // activation des interruptions pour la réception
    IE1 |= UTXIE0+URXIE0;                 // Enable USART0 RX/TX interrupt
    _EINT();                              // Enable interrupts
    
    //_BIS_SR(CPUOFF);                    // Enter LPM0
    /*
    for (;;){
      _BIS_SR(CPUOFF);                    // Enter LPM0
      //_NOP();                             // Required only for C-spy
    }*/
}



