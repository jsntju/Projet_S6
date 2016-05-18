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
 * @version 1.1.0 / 11/05/2016
 * 
 * @todo - 
 * 
 * @bug - 
 */

/**
 * @file ecran.c
 * @brief Le programme permet d'afficher l'interface graphique sur l'écran du GPS
 */


#include <__cross_studio_io.h>
#include <msp430x16x.h>
#include <string.h>
#include <stdlib.h>

#include "gps.h"
#include "ecran.h"
#include "io_led_pad.h"

int choix_ecran;
int choix_ecran_prec;
int push_actif,top_actif,bot_actif,left_actif,right_actif;

/* @Brief: permet de faire les configurations nécessaires pour afficher 
 * des textes ou des figures sur l'écran + affichage du bandeau superieur
*/
void init_ecran(void){
    clean_screen();                           // permet de netoyer l'écran
    SSTimeout(0);                             // arret du defilement de l'écran
    gfx_ScreenMode(1);                        // mode paysage inversé
    
    // création du bandeau supérieur
    gfx_RectangleFilled(0,0,128,15,0451);     // bandeau DARKCYAN de (0,0) à (128,15) 
    txt_FGcolour(65535);                      // couleur de la police blanc (0xFF) 
    txt_BGcolour(0451);                       // couleur d'arrière-plan du texte DARKCYAN (0d0451) 
    gfx_MoveTo(3,3);                          // curseur sur (3,3)
    putstr("GPS",3);                          // affichage de la chaîne de caractère "GPS"    
    gfx_Rectangle(86,3,96,10,0xFFFF);         // dessin de la batterie (!! l'affichage de la charge
    gfx_Rectangle(84,5,86,8,0xFFFF);          // de la batterie n'est pas fonctionnel, on ne 
    gfx_RectangleFilled(88,5,94,8,0xFFFF);    // donne qu'une idée de comment on peut la représenter!!)
    gfx_MoveTo(97,3);                         // curseur sur (104,3)
    putstr("100%",4);                         // affichage de la capacité de la batterie
    gfx_BGcolour(0xFFFF);                     // arrière plan blanc

    // création du contenu
    init_choix_main();
}

/* @Brief: permet d'afficher le menu princpal
*/
void init_choix_main(void){
    gfx_RectangleFilled(0,15,128,128,0xFFFF); // rectangle blanc sur le reste de l'écran en dessous du bandeau
    txt_FGcolour(65535);                     // couleur de la police blanc (0xFF) 
    
    // dessin du bouton Navigation (seléctionné)
    txt_BGcolour(0x11F6);                    // couleur d'arrière-plan du texte BLEU (0x11F6) 
    gfx_RectangleFilled(10,20,118,37,0x11F6);// rectangle bleu (0x11F6) 
    gfx_MoveTo(13,23);                       // ""
    putstr("Navigation",10);                 // "" 
    
    // dessin du bouton Enregistrement 
    txt_BGcolour(0x630C);                    // couleur d'arrière-plan du texte GRIS (0x630C) 
    gfx_RectangleFilled(10,42,118,59,0x630C);// rectangle gris (0x630C) 
    gfx_MoveTo(13,45);                       // ""
    putstr("Enregistrement",14);             // ""
    
    // dessin du bouton Paramètres
    gfx_RectangleFilled(10,64,118,81,0x630C);// couleur d'arrière-plan du texte GRIS (0x630C) 
    gfx_MoveTo(13,67);                       // rectangle gris (0x630C) 
    putstr("Parametres",10);                 // ""
    
    // dessin du bouton test
    gfx_RectangleFilled(10,86,118,103,0x630C);//couleur d'arrière-plan du texte GRIS (0x630C) 
    gfx_MoveTo(13,89);                       // rectangle gris (0x630C) 
    putstr("Tests",5);                       // ""
}

/* @Brief: permet d'afficher le menu du mode Navigation
*/
void init_choix_navigation(void){
    gfx_RectangleFilled(0,15,128,128,0xFFFF);// rectangle blanc sur le reste de l'écran en dessous du bandeau
    txt_FGcolour(65535);                    // couleur de la police blanc (0xFF) 
    
    // dessin du bouton Parcours seléctionné
    txt_BGcolour(0x11F6);                   // couleur d'arrière-plan du texte BLEU (0x11F6) 
    gfx_RectangleFilled(10,20,118,37,0x11F6);//rectangle bleu (0x11F6) 
    gfx_MoveTo(13,23);                      // ""
    putstr("Mode Parcours",13);             // ""
    
    // dessin du bouton Boussole
    txt_BGcolour(0x630C);                   // couleur d'arrière-plan du texte GRIS (0x630C) 
    gfx_RectangleFilled(10,42,118,59,0x630C);//rectangle gris (0x630C) 
    gfx_MoveTo(13,45);                      // ""
    putstr("Mode Boussole",13);             // ""
    
    // dessin du bouton Retour
    gfx_RectangleFilled(10,64,118,81,0x630C);//couleur d'arrière-plan du texte GRIS (0x630C) 
    gfx_MoveTo(13,67);                     // ""
    putstr("Retour",6);                    // ""
}

/* @Brief: permet la page correspondant au mode parcours
*/
void init_parcours(void){
    gfx_RectangleFilled(0,15,128,95,0xFFFF);//rectangle blanc sur le reste de l'écran en dessous du bandeau
    txt_FGcolour(0x0000);                  // couleur de la police noir (0x0000) 
    txt_BGcolour(0xFFFF);                  // couleur d'arrière-plan du texte BLANC (0xFFFF) 

    gfx_MoveTo(10,20);                     // ""
    putstr("UTC:",4);                      // "" 
    gfx_MoveTo(10,30);                     // ""
    putstr("lat:",4);                      // "" 
    gfx_MoveTo(10,40);                     // ""
    putstr("NS :",4);                      // ""
    gfx_MoveTo(10,50);                     // ""
    putstr("lon:",4);                      // ""
    gfx_MoveTo(10,60);                     // ""
    putstr("EW :",4);                      // ""
    gfx_MoveTo(10,70);                     // ""
    putstr("sat:",4);                      // ""
    gfx_MoveTo(10,80);                     // ""
    putstr("Alt:",4);                      // ""
}
/* @Brief: cette fonction est appelée lors d'un appui sur TOP ou BOT du pad et 
 * permet de remettre à jour le choix sélectionné dans le menu principal
*/
void select_main(void){
    switch(choix_ecran){                            // bouton coloré en bleu selon le choix selectionné
        case 0:                                     // choix_ecran = 0 : est au-dessus du bouton Navigation
          choix_ecran = 4;                          // on redescend tout en bas au choix n°4
          select_main();          
          break;
        case 1:                                     // choix_ecran = 1 : bouton Navigation séléctionnée
          gfx_RectangleFilled(10,20,118,37,0x11F6);
          gfx_MoveTo(13,23);
          txt_BGcolour(0x11F6);
          putstr("Navigation",10);
          break;
        case 2:                                     // choix_ecran = 2 : bouton Parcours séléctionnée
          gfx_RectangleFilled(10,42,118,59,0x11F6);
          gfx_MoveTo(13,45);
          txt_BGcolour(0x11F6);
          putstr("Enregistrement",14);
          break;
        case 3:                                     // choix_ecran = 3 : bouton Paramètres séléctionnée
          gfx_RectangleFilled(10,64,118,81,0x11F6);
          gfx_MoveTo(13,67);
          txt_BGcolour(0x11F6);
          putstr("Parametres",10);
          break;
        case 4:                                     // choix_ecran = 4 : bouton Tests séléctionnée
          gfx_RectangleFilled(10,86,118,103,0x11F6);
          gfx_MoveTo(13,89);
          txt_BGcolour(0x11F6);
          putstr("Tests",5);
          break;
        case 5:                                    // choix_ecran = 5 : on est en-dessous du bouton Tests
          choix_ecran = 1;                         // on remonte tout en haut au choix n°1
          select_main();
          break;
    }
    switch(choix_ecran_prec){                      // on recolore en gris le bouton correspondant au choix précedent
        case 1:
          txt_BGcolour(0x630C);
          gfx_RectangleFilled(10,20,118,37,0x630C);
          gfx_MoveTo(13,23);
          putstr("Navigation",10);
          break;
        case 2:
          txt_BGcolour(0x630C);
          gfx_RectangleFilled(10,42,118,59,0x630C);
          gfx_MoveTo(13,45);
          putstr("Enregistrement",14);
          break;
        case 3:
          txt_BGcolour(0x630C);
          gfx_RectangleFilled(10,64,118,81,0x630C);
          gfx_MoveTo(13,67);
          putstr("Parametres",10);
          break;
        case 4:
          txt_BGcolour(0x630C);
          gfx_RectangleFilled(10,86,118,103,0x630C);
          gfx_MoveTo(13,89);
          putstr("Tests",5);
          break;
    }
}
/* @Brief: cette fonction est appelée lors d'un appui sur TOP ou BOT du pad et 
 * permet de remettre à jour le choix sélectionné dans le menu du mode navigation
*/
void select_navigation(void){
    switch(choix_ecran){
        case 0:
          choix_ecran = 3;
          select_navigation();          
          break;
        case 1:
          gfx_RectangleFilled(10,20,118,37,0x11F6);
          gfx_MoveTo(13,23);
          txt_BGcolour(0x11F6);
          putstr("Mode Parcours",13);
          break;
        case 2:
          gfx_RectangleFilled(10,42,118,59,0x11F6);
          gfx_MoveTo(13,45);
          txt_BGcolour(0x11F6);
          putstr("Mode Boussole",13);
          break;
        case 3:
          txt_BGcolour(0xE000);
          gfx_RectangleFilled(10,64,118,81,0xE000);
          gfx_MoveTo(13,67);
          putstr("Retour",6);
          break;
        case 4:
          choix_ecran = 1;
          select_navigation(); 
          break;
    }
    switch(choix_ecran_prec){
        case 1:
          txt_BGcolour(0x630C);
          gfx_RectangleFilled(10,20,118,37,0x630C);
          gfx_MoveTo(13,23);
          putstr("Mode Parcours",13);
          break;
        case 2:
          txt_BGcolour(0x630C);
          gfx_RectangleFilled(10,42,118,59,0x630C);
          gfx_MoveTo(13,45);
          putstr("Mode Boussole",13);
          break;
        case 3:
          txt_BGcolour(0x630C);
          gfx_RectangleFilled(10,64,118,81,0x630C);
          gfx_MoveTo(13,67);
          putstr("Retour",6);
          break;
    }
}
/* @Brief: cette fonction est appelée lors d'un appui sur TOP ou BOT du pad et 
 * permet de remettre à jour le choix sélectionné dans le menu du mode parcours
*/
void select_parcours(void){
    switch(choix_ecran){
        case 0:
          txt_BGcolour(0x630C);
          txt_FGcolour(65535);
          gfx_RectangleFilled(10,100,118,117,0x630C);
          gfx_MoveTo(13,103);
          putstr("Retour",6);
          break;
        case 1:
          gfx_RectangleFilled(10,100,118,117,0xE000);
          txt_BGcolour(0xE000);
          txt_FGcolour(65535);
          gfx_MoveTo(13,103);
          putstr("Retour",6);
          break;
    }
}
/* @Brief: cette fonction est appelée pour remettre à jour le choix selectionné. un appui sur TOP ou BOT du pad  
 * permet respectivement de decrémenter ou incrémenter choix_ecran donc "monter" ou "descendre" dans les choix
 * dans le menu principal
*/
void choix_main_pad(void){
    if(top_actif == 1) { 
        top_actif = 0;
        choix_ecran_prec = choix_ecran;
        choix_ecran--;
        select_main();
    } else if(bot_actif == 1) {
        bot_actif = 0;
        choix_ecran_prec = choix_ecran;
        choix_ecran++;
        select_main();
    }
}
/* @Brief: cette fonction est appelée pour remettre à jour le choix selectionné. un appui sur TOP ou BOT du pad  
 * permet respectivement de decrémenter ou incrémenter choix_ecran donc "monter" ou "descendre" dans les choix
 * dans le menu du mode navigation
*/
void choix_navig_pad(void){
    if(top_actif == 1) { 
        choix_ecran_prec = choix_ecran;
        choix_ecran--;
        top_actif = 0;
        select_navigation();
    } else if(bot_actif == 1) {
        choix_ecran_prec = choix_ecran;
        choix_ecran++;
        bot_actif = 0;
        select_navigation();
    }
}
/* @Brief: cette fonction est appelée pour remettre à jour le choix selectionné. un appui sur TOP ou BOT du pad  
 * permet respectivement de selectionner ou deselectionner le bouton retour
*/
void choix_parcours(void){
    if(top_actif || bot_actif) { 
        choix_ecran_prec = choix_ecran;
        choix_ecran = !choix_ecran_prec;
        top_actif = 0;
        bot_actif = 0;
        select_parcours();
        //debug_printf("ce: %i\n",choix_ecran);
    }
}

// ------- FONCTION ELEMENTAIRE POUR LA PROGRAMMATION DE L'ECRAN ---------------

/* @Brief: permet d'envoyer une donnée type char à l'écran*/
void send_char(int c){
    while (!(IFG2 & UTXIFG1)) {}
    TXBUF1 = c ;
}

/* @Brief: permet de nettoyer l'écran */
void clean_screen(void){
    int i; 
    do{
        send_char(0xFF);          // envoi du code pour la fonction clean_screen de l'écran
        send_char(0xD7);
        while( flag_1 != 1){}
        flag_1 = 0;
    }while( rep_ecran != 6);
}

/* @Brief: permet de fixer de temps de défilement de l'interface sur l'écran 
 * @Parametres: 
 * - int t: temps en ms à fixer
*/
void SSTimeout(int t){
    int trame_ecran[4] = {0x0F,0x0F,0x0D,0x07};
    do{
        send_char(0x00);                       // envoi du code pour la fonction SSTimeout de l'écran
        send_char(0x0C); 
        send_char(t>>8); send_char((0x00FF)&t);// envoi du temps
        while( flag_1 != 1){}
        flag_1 = 0;
    }while( rep_ecran != 6);
}
/* @Brief: permet de définir l'orientation des prochains éléments sur l'écran
 * @Parametres: 
 * - int mode: mode sélectionné
*/
void gfx_ScreenMode(int mode){
    do{
        send_char(0xFF);    // envoi du code pour la fonction gfx_ScreenMode de l'écran
        send_char(0x68);
        send_char(0x00);
        send_char(mode);    // envoi du n° de mode (0,1,2 ou 3)
        while( flag_1 != 1){}
        flag_1 = 0;
    }while( rep_ecran != 6);
}

/* @Brief: permet de dessiner un rectangle sur l'écran 
 * @Parametres: 
 * - int X1: coordonnées horizontales en pixels du point supérieur gauche du rectangle
 * - int Y1: coordonnées verticales en pixels du point supérieur gauche du rectangle
 * - int X2: coordonnées horizontales en pixels du point inférieur droit du rectangle
 * - int Y2: coordonnées verticales en pixels du point inférieur droit du rectangle
 * - int color: couleur du rectangle
*/
void gfx_Rectangle(int X1, int Y1, int X2, int Y2, int color){
    do{
        send_char(0xFF);                              // envoi du code pour la fonction gfx_Rectangle de l'écran
        send_char(0xCF);
        send_char(0x00);send_char(X1);                // envoi des différentes coordonnées 
        send_char(0x00);send_char(Y1);
        send_char(0x00);send_char(X2);
        send_char(0x00);send_char(Y2);
        send_char(color>>8);send_char(color&(0x00FF));
        while( flag_1 != 1){}
        flag_1 = 0;
    }while( rep_ecran != 6);
}

/* @Brief: permet de dessiner un rectangle plein sur l'écran 
 * @Parametres: 
 * - int X1: coordonnées horizontales en pixels du point supérieur gauche du rectangle
 * - int Y1: coordonnées verticales en pixels du point supérieur gauche du rectangle
 * - int X2: coordonnées horizontales en pixels du point inférieur droit du rectangle
 * - int Y2: coordonnées verticales en pixels du point inférieur droit du rectangle
 * - int color: couleur du rectangle
*/
void gfx_RectangleFilled(int X1, int Y1, int X2, int Y2, int color){
    do{
        send_char(0xFF);                              // envoi du code pour la fonction gfx_RectangleFilled de l'écran
        send_char(0xCE);
        send_char(0x00);send_char(X1);                // envoi des différentes coordonnées 
        send_char(0x00);send_char(Y1);
        send_char(0x00);send_char(X2);
        send_char(0x00);send_char(Y2);
        send_char(color>>8);send_char(color&(0x00FF));
        
        while( flag_1 != 1){}
        flag_1 = 0;
    }while( rep_ecran != 6);
}

/* @Brief: permet de repositionner le pointeur pour l'écriture des caractères sur l'écran
 * @Parametres: 
 * - int X: coordonnées horizontales du curseur
 * - int Y: coordonnées verticales du curseur
 */
void gfx_MoveTo(int X, int Y){
    do{
        send_char(0xFF);                  // envoi du code pour la fonction gfx_MoveTo de l'écran
        send_char(0xD6);
        send_char(0x00);send_char(X);     // envoi des coordonnées 
        send_char(0x00);send_char(Y);
        while( flag_1 != 1){}
        flag_1 = 0;
    }while( rep_ecran != 6);
}

/* @Brief: permet de modifier la couleur d'arrière-fond
 * @Parametres: 
 * - int color: couleur de fond à fixer 
*/
void gfx_BGcolour(int color){
    do{
        send_char(0xFF);                                // envoi du code pour la fonction gfx_BGcolour de l'écran
        send_char(0x6E);
        send_char(color>>8);send_char(color&(0x00FF));  // envoi de la couleur
        while( flag_1 != 1){}
        flag_1 = 0;
    }while( rep_ecran != 6);
}
/* @Brief: permet de modifier la couleur de la police de caractère
 * @Parametres: 
 * - int color: couleur de la police de caractère à fixer 
*/
void txt_FGcolour(int color){
    do{
        send_char(0xFF);                              // envoi du code pour la fonction txt_FGcolour de l'écran
        send_char(0x7F);
        send_char(color>>8);send_char(color&(0x00FF));// envoi de la couleur
        while( flag_1 != 1){}
        flag_1 = 0;
    }while( rep_ecran != 6);
}

/* @Brief: permet de modifier la couleur d'arrière-plan du texte
 * @Parametres: 
 * - int color: couleur d'arrière-plan du texte à fixer 
*/
void txt_BGcolour(int color){
    do{
        send_char(0xFF);
        send_char(0x7E);
        send_char(color>>8);send_char(color&(0x00FF));
        while( flag_1 != 1){}
        flag_1 = 0;
    }while( rep_ecran != 6);
}

/* @Brief: permet d'écrire un caractère sur l'écran
 * @Parametres: 
 * - char c: caractère ascii à afficher sur l'écran
*/
void putCH(char c){
    do{
        send_char(0xFF);
        send_char(0xFE);
        send_char(c>>8);send_char(c&(0x00FF));
        while( flag_1 != 1){}
        flag_1 = 0;
    }while( rep_ecran != 6);
}

/* @Brief: permet d'écrire une chaine de caractère sur l'écran
 * @Parametres: 
 * - char* chaine: chaine de caractère à afficher
 * - int taille: nombre de caractères à afficher
*/
void putstr(char* chaine,int taille){
    int i;
    do{
        send_char(0x00);
        send_char(0x06);
        i=0;
        while((i<taille) && (chaine[i] != '\0')){
            send_char(chaine[i]);
            i++;
        }
        send_char(0x00);
        while( flag_1 != 1){}
        flag_1 = 0;
    }while( rep_ecran != 6);
}



