
#include <__cross_studio_io.h>
#include <msp430x16x.h>
#include <string.h>
#include "ecran.h"


/*@Brief: initilialise les LED et le PAD
*/
void initialisation_LED_PAD(void){
    P1DIR |= 0x1F;      	// = (0001 1111)2 : met les bits P1.0 à P1.4 (LEDs) en sortie 
    P2DIR = 0x00;      		// = (1110 0000)2 : met les bits P2.0 à P2.4 (PAD) en entrée
    P2IE = 0xFF;        	// = (1111 1111)2 : met les bits le port 2 en interruption
    P2IES = 0xFF;
    P1OUT = 0x1F;       	// les 5 LEDs sont mis à 1 pour verifier leur états au départ
}


/*
@Brief: Ajoute un délai
@Parametres:
	- t (entier): taille du délais (en ms) 
	(Exemple: si t=10000 alors  t*100 = 1 seconde)
*/
void delay(unsigned int t){
    int i,j;
    for( j=0 ; j<=t ; j++ ){
        for( i=0 ; i<=100 ; i++ ){}
    }
}



/*
@Brief: Indique les choix effectuer lorsque l'on utilise le PAD 
*/
void P2_Pad (void) __interrupt[ PORT2_VECTOR ]{  
	
        P1OUT = P2IN;					//Initialisation du PAD
        push_actif = 0;
        top_actif = 0;
        bot_actif = 0;
        left_actif = 0;
        right_actif = 0;

        if(P2IN == 0x1E) push_actif = 1;		//Indique les choix du pad (1= choix enclencher)	
        if(P2IN == 0x1D) top_actif = 1;
        if(P2IN == 0x1B) bot_actif = 1;
        if(P2IN == 0x17) left_actif = 1;
        if(P2IN == 0x0F) right_actif = 1;

        P2IFG = 0x00 ;					// reset buffer
        delay(100);
        P2IFG = 0x00 ;
}


