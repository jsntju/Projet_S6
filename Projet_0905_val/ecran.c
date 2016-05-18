/**
 * Polytech Marseille
 * Case 925 - 163, avenue de Luminy
 * 13288 Marseille CEDEX 9
 * 
 * Ce fichier est l'oeuvre d'�l�ves de Polytech Marseille. Il ne peut �tre 
 * reproduit, utilis� ou modifi� sans l'avis express de ses auteurs.
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
 * @brief Le programme permet d'afficher l'interface graphique sur l'�cran du GPS
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

/* @Brief: permet de faire les configurations n�cessaires pour afficher 
 * des textes ou des figures sur l'�cran + affichage du bandeau superieur
*/
void init_ecran(void){
    clean_screen();                           // permet de netoyer l'�cran
    SSTimeout(0);                             // arret du defilement de l'�cran
    gfx_ScreenMode(1);                        // mode paysage invers�
    
    // cr�ation du bandeau sup�rieur
    gfx_RectangleFilled(0,0,128,15,0451);     // bandeau DARKCYAN de (0,0) � (128,15) 
    txt_FGcolour(65535);                      // couleur de la police blanc (0xFF) 
    txt_BGcolour(0451);                       // couleur d'arri�re-plan du texte DARKCYAN (0d0451) 
    gfx_MoveTo(3,3);                          // curseur sur (3,3)
    putstr("GPS",3);                          // affichage de la cha�ne de caract�re "GPS"    
    gfx_Rectangle(86,3,96,10,0xFFFF);         // dessin de la batterie (!! l'affichage de la charge
    gfx_Rectangle(84,5,86,8,0xFFFF);          // de la batterie n'est pas fonctionnel, on ne 
    gfx_RectangleFilled(88,5,94,8,0xFFFF);    // donne qu'une id�e de comment on peut la repr�senter!!)
    gfx_MoveTo(97,3);                         // curseur sur (104,3)
    putstr("100%",4);                         // affichage de la capacit� de la batterie
    gfx_BGcolour(0xFFFF);                     // arri�re plan blanc

    // cr�ation du contenu
    init_choix_main();
}

/* @Brief: permet d'afficher le menu princpal
*/
void init_choix_main(void){
    gfx_RectangleFilled(0,15,128,128,0xFFFF); // rectangle blanc sur le reste de l'�cran en dessous du bandeau
    txt_FGcolour(65535);                     // couleur de la police blanc (0xFF) 
    
    // dessin du bouton Navigation (sel�ctionn�)
    txt_BGcolour(0x11F6);                    // couleur d'arri�re-plan du texte BLEU (0x11F6) 
    gfx_RectangleFilled(10,20,118,37,0x11F6);// rectangle bleu (0x11F6) 
    gfx_MoveTo(13,23);                       // ""
    putstr("Navigation",10);                 // "" 
    
    // dessin du bouton Enregistrement 
    txt_BGcolour(0x630C);                    // couleur d'arri�re-plan du texte GRIS (0x630C) 
    gfx_RectangleFilled(10,42,118,59,0x630C);// rectangle gris (0x630C) 
    gfx_MoveTo(13,45);                       // ""
    putstr("Enregistrement",14);             // ""
    
    // dessin du bouton Param�tres
    gfx_RectangleFilled(10,64,118,81,0x630C);// couleur d'arri�re-plan du texte GRIS (0x630C) 
    gfx_MoveTo(13,67);                       // rectangle gris (0x630C) 
    putstr("Parametres",10);                 // ""
    
    // dessin du bouton test
    gfx_RectangleFilled(10,86,118,103,0x630C);//couleur d'arri�re-plan du texte GRIS (0x630C) 
    gfx_MoveTo(13,89);                       // rectangle gris (0x630C) 
    putstr("Tests",5);                       // ""
}

/* @Brief: permet d'afficher le menu du mode Navigation
*/
void init_choix_navigation(void){
    gfx_RectangleFilled(0,15,128,128,0xFFFF);// rectangle blanc sur le reste de l'�cran en dessous du bandeau
    txt_FGcolour(65535);                    // couleur de la police blanc (0xFF) 
    
    // dessin du bouton Parcours sel�ctionn�
    txt_BGcolour(0x11F6);                   // couleur d'arri�re-plan du texte BLEU (0x11F6) 
    gfx_RectangleFilled(10,20,118,37,0x11F6);//rectangle bleu (0x11F6) 
    gfx_MoveTo(13,23);                      // ""
    putstr("Mode Parcours",13);             // ""
    
    // dessin du bouton Boussole
    txt_BGcolour(0x630C);                   // couleur d'arri�re-plan du texte GRIS (0x630C) 
    gfx_RectangleFilled(10,42,118,59,0x630C);//rectangle gris (0x630C) 
    gfx_MoveTo(13,45);                      // ""
    putstr("Mode Boussole",13);             // ""
    
    // dessin du bouton Retour
    gfx_RectangleFilled(10,64,118,81,0x630C);//couleur d'arri�re-plan du texte GRIS (0x630C) 
    gfx_MoveTo(13,67);                     // ""
    putstr("Retour",6);                    // ""
}

/* @Brief: permet la page correspondant au mode parcours
*/
void init_parcours(void){
    gfx_RectangleFilled(0,15,128,95,0xFFFF);//rectangle blanc sur le reste de l'�cran en dessous du bandeau
    txt_FGcolour(0x0000);                  // couleur de la police noir (0x0000) 
    txt_BGcolour(0xFFFF);                  // couleur d'arri�re-plan du texte BLANC (0xFFFF) 

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
/* @Brief: cette fonction est appel�e lors d'un appui sur TOP ou BOT du pad et 
 * permet de remettre � jour le choix s�lectionn� dans le menu principal
*/
void select_main(void){
    switch(choix_ecran){                            // bouton color� en bleu selon le choix selectionn�
        case 0:                                     // choix_ecran = 0 : est au-dessus du bouton Navigation
          choix_ecran = 4;                          // on redescend tout en bas au choix n�4
          select_main();          
          break;
        case 1:                                     // choix_ecran = 1 : bouton Navigation s�l�ctionn�e
          gfx_RectangleFilled(10,20,118,37,0x11F6);
          gfx_MoveTo(13,23);
          txt_BGcolour(0x11F6);
          putstr("Navigation",10);
          break;
        case 2:                                     // choix_ecran = 2 : bouton Parcours s�l�ctionn�e
          gfx_RectangleFilled(10,42,118,59,0x11F6);
          gfx_MoveTo(13,45);
          txt_BGcolour(0x11F6);
          putstr("Enregistrement",14);
          break;
        case 3:                                     // choix_ecran = 3 : bouton Param�tres s�l�ctionn�e
          gfx_RectangleFilled(10,64,118,81,0x11F6);
          gfx_MoveTo(13,67);
          txt_BGcolour(0x11F6);
          putstr("Parametres",10);
          break;
        case 4:                                     // choix_ecran = 4 : bouton Tests s�l�ctionn�e
          gfx_RectangleFilled(10,86,118,103,0x11F6);
          gfx_MoveTo(13,89);
          txt_BGcolour(0x11F6);
          putstr("Tests",5);
          break;
        case 5:                                    // choix_ecran = 5 : on est en-dessous du bouton Tests
          choix_ecran = 1;                         // on remonte tout en haut au choix n�1
          select_main();
          break;
    }
    switch(choix_ecran_prec){                      // on recolore en gris le bouton correspondant au choix pr�cedent
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
/* @Brief: cette fonction est appel�e lors d'un appui sur TOP ou BOT du pad et 
 * permet de remettre � jour le choix s�lectionn� dans le menu du mode navigation
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
/* @Brief: cette fonction est appel�e lors d'un appui sur TOP ou BOT du pad et 
 * permet de remettre � jour le choix s�lectionn� dans le menu du mode parcours
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
/* @Brief: cette fonction est appel�e pour remettre � jour le choix selectionn�. un appui sur TOP ou BOT du pad  
 * permet respectivement de decr�menter ou incr�menter choix_ecran donc "monter" ou "descendre" dans les choix
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
/* @Brief: cette fonction est appel�e pour remettre � jour le choix selectionn�. un appui sur TOP ou BOT du pad  
 * permet respectivement de decr�menter ou incr�menter choix_ecran donc "monter" ou "descendre" dans les choix
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
/* @Brief: cette fonction est appel�e pour remettre � jour le choix selectionn�. un appui sur TOP ou BOT du pad  
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

/* @Brief: permet d'envoyer une donn�e type char � l'�cran*/
void send_char(int c){
    while (!(IFG2 & UTXIFG1)) {}
    TXBUF1 = c ;
}

/* @Brief: permet de nettoyer l'�cran */
void clean_screen(void){
    int i; 
    do{
        send_char(0xFF);          // envoi du code pour la fonction clean_screen de l'�cran
        send_char(0xD7);
        while( flag_1 != 1){}
        flag_1 = 0;
    }while( rep_ecran != 6);
}

/* @Brief: permet de fixer de temps de d�filement de l'interface sur l'�cran 
 * @Parametres: 
 * - int t: temps en ms � fixer
*/
void SSTimeout(int t){
    int trame_ecran[4] = {0x0F,0x0F,0x0D,0x07};
    do{
        send_char(0x00);                       // envoi du code pour la fonction SSTimeout de l'�cran
        send_char(0x0C); 
        send_char(t>>8); send_char((0x00FF)&t);// envoi du temps
        while( flag_1 != 1){}
        flag_1 = 0;
    }while( rep_ecran != 6);
}
/* @Brief: permet de d�finir l'orientation des prochains �l�ments sur l'�cran
 * @Parametres: 
 * - int mode: mode s�lectionn�
*/
void gfx_ScreenMode(int mode){
    do{
        send_char(0xFF);    // envoi du code pour la fonction gfx_ScreenMode de l'�cran
        send_char(0x68);
        send_char(0x00);
        send_char(mode);    // envoi du n� de mode (0,1,2 ou 3)
        while( flag_1 != 1){}
        flag_1 = 0;
    }while( rep_ecran != 6);
}

/* @Brief: permet de dessiner un rectangle sur l'�cran 
 * @Parametres: 
 * - int X1: coordonn�es horizontales en pixels du point sup�rieur gauche du rectangle
 * - int Y1: coordonn�es verticales en pixels du point sup�rieur gauche du rectangle
 * - int X2: coordonn�es horizontales en pixels du point inf�rieur droit du rectangle
 * - int Y2: coordonn�es verticales en pixels du point inf�rieur droit du rectangle
 * - int color: couleur du rectangle
*/
void gfx_Rectangle(int X1, int Y1, int X2, int Y2, int color){
    do{
        send_char(0xFF);                              // envoi du code pour la fonction gfx_Rectangle de l'�cran
        send_char(0xCF);
        send_char(0x00);send_char(X1);                // envoi des diff�rentes coordonn�es 
        send_char(0x00);send_char(Y1);
        send_char(0x00);send_char(X2);
        send_char(0x00);send_char(Y2);
        send_char(color>>8);send_char(color&(0x00FF));
        while( flag_1 != 1){}
        flag_1 = 0;
    }while( rep_ecran != 6);
}

/* @Brief: permet de dessiner un rectangle plein sur l'�cran 
 * @Parametres: 
 * - int X1: coordonn�es horizontales en pixels du point sup�rieur gauche du rectangle
 * - int Y1: coordonn�es verticales en pixels du point sup�rieur gauche du rectangle
 * - int X2: coordonn�es horizontales en pixels du point inf�rieur droit du rectangle
 * - int Y2: coordonn�es verticales en pixels du point inf�rieur droit du rectangle
 * - int color: couleur du rectangle
*/
void gfx_RectangleFilled(int X1, int Y1, int X2, int Y2, int color){
    do{
        send_char(0xFF);                              // envoi du code pour la fonction gfx_RectangleFilled de l'�cran
        send_char(0xCE);
        send_char(0x00);send_char(X1);                // envoi des diff�rentes coordonn�es 
        send_char(0x00);send_char(Y1);
        send_char(0x00);send_char(X2);
        send_char(0x00);send_char(Y2);
        send_char(color>>8);send_char(color&(0x00FF));
        
        while( flag_1 != 1){}
        flag_1 = 0;
    }while( rep_ecran != 6);
}

/* @Brief: permet de repositionner le pointeur pour l'�criture des caract�res sur l'�cran
 * @Parametres: 
 * - int X: coordonn�es horizontales du curseur
 * - int Y: coordonn�es verticales du curseur
 */
void gfx_MoveTo(int X, int Y){
    do{
        send_char(0xFF);                  // envoi du code pour la fonction gfx_MoveTo de l'�cran
        send_char(0xD6);
        send_char(0x00);send_char(X);     // envoi des coordonn�es 
        send_char(0x00);send_char(Y);
        while( flag_1 != 1){}
        flag_1 = 0;
    }while( rep_ecran != 6);
}

/* @Brief: permet de modifier la couleur d'arri�re-fond
 * @Parametres: 
 * - int color: couleur de fond � fixer 
*/
void gfx_BGcolour(int color){
    do{
        send_char(0xFF);                                // envoi du code pour la fonction gfx_BGcolour de l'�cran
        send_char(0x6E);
        send_char(color>>8);send_char(color&(0x00FF));  // envoi de la couleur
        while( flag_1 != 1){}
        flag_1 = 0;
    }while( rep_ecran != 6);
}
/* @Brief: permet de modifier la couleur de la police de caract�re
 * @Parametres: 
 * - int color: couleur de la police de caract�re � fixer 
*/
void txt_FGcolour(int color){
    do{
        send_char(0xFF);                              // envoi du code pour la fonction txt_FGcolour de l'�cran
        send_char(0x7F);
        send_char(color>>8);send_char(color&(0x00FF));// envoi de la couleur
        while( flag_1 != 1){}
        flag_1 = 0;
    }while( rep_ecran != 6);
}

/* @Brief: permet de modifier la couleur d'arri�re-plan du texte
 * @Parametres: 
 * - int color: couleur d'arri�re-plan du texte � fixer 
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

/* @Brief: permet d'�crire un caract�re sur l'�cran
 * @Parametres: 
 * - char c: caract�re ascii � afficher sur l'�cran
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

/* @Brief: permet d'�crire une chaine de caract�re sur l'�cran
 * @Parametres: 
 * - char* chaine: chaine de caract�re � afficher
 * - int taille: nombre de caract�res � afficher
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



