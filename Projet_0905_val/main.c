
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
 * @version 1.0.0 / 16/05/2016
 * 
 * @todo - 
 * 
 * @bug - 
 */

/**
 * @file main.c
 * @brief Le programme principal permet d'appeler toutes les fonctions nécessaires pour
 * gérer la navigation dans l'interface graphique, l'activation de la recéption des trames 
 * GPS et la boussole
 * 
 */

#include <__cross_studio_io.h>
#include  <msp430x16x.h>
#include <string.h>
#include <stdlib.h>

#include "gps.h"
#include "boussole.h"
#include "io_led_pad.h"
#include "ecran.h"

/* Données trames GPS */
char* buf;
char* trameGGA;

/* --- Variable globale --- */
float cap1 = 0;  


void main(void){ 
    int i;
    // initialisation de la clock (prioritaire et à faire en premier !)
    // sequence à faire pour le switch entre 2 clock (cf page 4-12 du guide de l'utilisateur)
                                  
    WDTCTL = WDTPW + WDTHOLD;           // arrêt du watchdog : 0x5A00 + 0x0080 (= Mot de passe + Watchdog Timer(WDT) arrété)                             
                                        // Configuration du Basic Clock System Control
    BCSCTL1 &= ~XT2OFF;                 // XT2 est allumé                
    do {                                // verification nécéssaire du OSCFault lorsqu'on modifie la config des Clock
        IFG1 &= ~OFIFG;                 // Clear OSCFault flag
        for (i = 0xFF; i > 0; i--);     // Time for flag to set
    }while ( (IFG1 & OFIFG) != 0);      // OSCFault flag still set?               
    BCSCTL2 |= SELM1+SELS;              // MCLK = SMCLK = XT2 (safe) avec SMCLK = 8Mhz  
    
    // initialisation du Port P1 et P2 (LED et PAD)
    initialisation_LED_PAD();
    P1OUT = 0x1E;                       // led0 éteint : initialisation du port 1 et 2 terminée

    // initialisation du port P4 (P4.0 : ENABLE_GPS, P4.1 : RESET_LCD, P4.2 : CMD_SWITCH)
    P4DIR = 0x07;                       // P4.0,1 et 2 en sortie 
    P4OUT |= BIT0 ;                     // on active avec le GPS 
    P4OUT &= ~BIT1 ;                    // on crée un front montant sur RESET_LCD  
    delay(100);
    P4OUT |= BIT1 ;
    delay(60000);                       // délai de 9 secondes 
    delay(30000);                       // le temps que l'écran s'initialise
    P1OUT = 0x1C;                       // led1 éteint : initialisation du port 4 terminé

    // initialisation du port P3 (uart0 et uart1)
    init_uart();   
    P1OUT = 0x18;                       // led2 éteint : initialisation du port 3 terminé 

    // initialisation de l'écran (aprés activation de l'uart1)
    init_ecran();
    P1OUT = 0x10;                       // led3 éteint : initialisation de l'écran terminé
    P1OUT = 0x00;                       // initialisation terminé
    
    //P4OUT &= ~BIT2 & ~BIT0 ;          // Pour le moment, le GPS est désactivé mais on est branché 
                                        // sur le GPS
    P4OUT &= ~BIT2 ;                    // on est branché sur le GPS

    // ---------- FIN DE L'INITIALISATION ---------- //

    activer_communication_GPS();
    IE1 &= ~URXIE0 ;                    // Disable USART0 RX interrup 

    choix_ecran = 1;

    // ---------- PROGRAMME PRINCIPAL -------------- //
    while(1){
        choix_main_pad();                                           // actualisation du menu principal
        if(push_actif == 1){                                        // choix selectionné ?
            push_actif = 0;
            if(choix_ecran == 1){                                   // choix 1 fait : MODE NAVIGATION
                do{                                                 // tant qu'on est dans le menu navigation
                    choix_ecran = 1;
                    init_choix_navigation();                        // initialisation du menu navigation
                    push_actif = 0;
                    top_actif == 0;bot_actif == 0;
                    do{                                             // tant qu'on a pas saisi un choix
                        choix_navig_pad();                          // actualisation du menu navigation
                    }while( (push_actif != 1) );
                    if(push_actif == 1){                            // mode saisi?
                        push_actif = 0;
                        if(choix_ecran == 1){                       // MODE PARCOURS
                            choix_ecran = 0;                        // retour non selectionné
                            gfx_RectangleFilled(0,15,128,128,0xFFFF);// initialisation de l'écran blanc
                            init_parcours();                        // affichage de l'écran de base
                            gfx_RectangleFilled(10,100,118,117,0x630C);//affichage du bouton retour
                            txt_BGcolour(0x630C);
                            txt_FGcolour(65535);
                            gfx_MoveTo(13,103);
                            putstr("Retour",6);
                            push_actif = 0;                         // bouton push inactif
                            do{
                                do{
                                    IE1 |= URXIE0 ;                 // détection des trames GPS activé
                                    selec_trame_gga();              // recherche de la trame GGA
                                    h = search(trame,',',0)+1;      // pointeur sur 1er element de la trame
                                    choix_parcours();               // màj du menu: bouton retour selectionné ?
                                                                    // Séparation de la trame en données
                                    selec_objet(UTC);               // recupération de l'heure 
                                    selec_objet(latitude);          // "" de la latitude en DMS
                                    selec_objet(NS);                // "" du N/S
                                    selec_objet(longitude);         // "" de la longitude en DMS
                                    selec_objet(EW);                // "" du E/W
                                    selec_objet(Pos_ind);           // "" du position fix indicator
                                    selec_objet(nb_sat);            // "" du nombre de satellite
                                    selec_objet(HDOP);              // "" du horizontal dilution of precision
                                    selec_objet(alti);              // "" de l'altitude
                                    selec_objet(unit_alti);         // "" de l'unité  d'altitude
                                    
                                    choix_parcours();               // màj du menu: bouton retour selectionné ?
                                    if(GGA_OK()){                   // si signal OK affichage des trames 
                                        gfx_RectangleFilled(36,15,128,95,0xFFFF);
                                                                    // affichage de l'écran de base
                                        txt_BGcolour(0xFFFF);       // 
                                        txt_FGcolour(0x0000);       //
                                        gfx_MoveTo(37,20);          // suivi des données
                                        
                                        putCH(UTC[0]);              //
                                        putCH(UTC[1]);              //
                                        putCH(':'); 
                                        putCH(UTC[2]); 
                                        putCH(UTC[3]);
                                        putCH(' '); 
                                        putCH(UTC[4]);
                                        putCH(UTC[5]);
                                        putstr("sec",3);
                                        
                                        gfx_MoveTo(37,30);
                                        
                                        putCH(latitude[0]);   
                                        putCH(latitude[1]);
                                        putCH('\''); 
                                        putCH(latitude[2]); 
                                        putCH(latitude[3]);
                                        putCH('\''); 
                                        putCH(latitude[5]);
                                        putCH(latitude[6]);
                                        putCH('.'); 
                                        putCH(latitude[7]);
                                        putCH(latitude[8]);
                                        putCH(latitude[9]);
                                        putCH('"'); 
                                        
                                        gfx_MoveTo(37,40);
                                        putstr(NS,2);
                                        gfx_MoveTo(37,50);
                                        
                                        putCH(longitude[0]);   
                                        putCH(longitude[1]);
                                        putCH(longitude[2]); 
                                        putCH('\''); 
                                        putCH(longitude[3]);
                                        putCH(longitude[4]);
                                        putCH('\''); 
                                        putCH(longitude[6]);
                                        putCH(longitude[7]);
                                        putCH('.'); 
                                        putCH(longitude[8]);
                                        putCH(longitude[9]);
                                        putCH('"'); 
                                        gfx_MoveTo(37,60);
                                        putstr(EW,2);
                                        gfx_MoveTo(37,70);
                                        putstr(nb_sat,2);
                                        gfx_MoveTo(37,80);
                                        putstr(alti,10);putCH(' '); 
                                        putstr(unit_alti,1);
                                    }
                                    choix_parcours();               // màj du menu: bouton retour selectionné ?
                                    initialise_obj(trame,200);      // réinitialisation des tableaux de données
                                    initialise_obj(UTC,30);
                                    initialise_obj(latitude,10);
                                    initialise_obj(NS,2);
                                    initialise_obj(longitude,10);
                                    initialise_obj(EW,2);
                                    initialise_obj(Pos_ind,10);
                                    initialise_obj(nb_sat,10);
                                    initialise_obj(HDOP,10);
                                    initialise_obj(alti,10);
                                    initialise_obj(unit_alti,2);
                                    choix_parcours();               // màj du menu: bouton retour selectionné ?
                                }while(push_actif != 1);
                                push_actif = 0;                     // sortie du mode PARCOURS si "retour" selectionné
                            }while(choix_ecran != 1);               // et bouton PUSH activé
                            IE1 &= ~URXIE0 ;                        // détection des trames GPS désactivé  
                            choix_ecran = 1;
                        } else if(choix_ecran == 2){    // MODE BOUSSOLE
                            choix_ecran = 0;                        // retour non selectionné
                            gfx_RectangleFilled(0,15,128,128,0xFFFF);// initialisation de l'écran blanc
                            txt_BGcolour(0xFFFF);       // 
                            txt_FGcolour(0x0000);       //
                            gfx_MoveTo(10,20);
                            putstr("Mode Boussole:",13);
                            gfx_MoveTo(10,30);
                            putstr("La boussole est",15);
                            gfx_MoveTo(10,40);
                            putstr("represente sur",14);
                            gfx_MoveTo(10,50);
                            putstr("les 5 LEDs --->",15);

                            gfx_RectangleFilled(10,100,118,117,0x630C);//affichage du bouton retour
                            txt_BGcolour(0x630C);
                            txt_FGcolour(65535);
                            gfx_MoveTo(13,103);
                            putstr("Retour",6);
                          
                            push_actif = 0;                         // bouton push inactif
                            do{
                                do{
                                    IE1 |= URXIE0 ;                 // détection des trames GPS activé
                                    choix_parcours();               // màj du menu: bouton retour selectionné ?       
                                    selec_trame_vtg();
                                    cap1 = selec_cap (trame_vtg, cap1);
                                    choix_parcours();               // màj du menu: bouton retour selectionné ?
                                    boussole_LED(cap1);
                                    choix_parcours();               // màj du menu: bouton retour selectionné ?
                                }while(push_actif != 1);
                                push_actif = 0;                     // sortie du mode PARCOURS si "retour" selectionné
                            }while(choix_ecran != 1);               // et bouton PUSH activé
                            IE1 &= ~URXIE0 ;                        // détection des trames GPS désactivé  
                            cap1 = 0;
                            choix_ecran = 1;
                            P1OUT = 0x00;                           // LEDs éteintes
                        }
                    }
                }while( (choix_ecran != 3) );           // bouton retour saisi? 
                init_choix_main();                      // on quitte le mode navigation 
                choix_ecran = 1;                        // et on retourne au menu principal
            } else if(choix_ecran == 2){                // choix 2 fait : MODE ENREGISTREMENT

            } else if(choix_ecran == 3){                // choix 3 fait : PARAMETRES

            } else if(choix_ecran == 4){                // choix 4 fait : TESTS

            }
        }
        
    }
    debug_exit(0);
}
