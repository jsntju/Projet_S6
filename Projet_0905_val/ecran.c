#include <__cross_studio_io.h>
#include <msp430x16x.h>
#include <string.h>

#include "gps.h"
#include "ecran.h"
#include "io_led_pad.h"

int choix_ecran;
int choix_ecran_prec;
int push_actif,top_actif,bot_actif,left_actif,right_actif;

void init_ecran(void){
    clean_screen();
    SSTimeout(0);                        // arret du defilement de l'écran
    gfx_ScreenMode(1);                   // mode paysage inversé
    
    gfx_RectangleFilled(0,0,128,15,0451);
    gfx_RectangleFilled(0,15,128,128,65535);
    txt_FGcolour(65535);
    txt_BGcolour(0451);
    gfx_MoveTo(3,3);
    putstr("GPS",3);
    gfx_Rectangle(91,3,101,10,0xFFFF);
    gfx_Rectangle(89,5,91,8,0xFFFF);
    gfx_RectangleFilled(93,5,99,8,0xFFFF);
    gfx_MoveTo(104,3);
    putstr("99%",3);
    
    gfx_BGcolour(0xFFFF);
    init_choix_main();
}

void init_choix_main(void){
    txt_FGcolour(65535);
    
    txt_BGcolour(0x630C);
    gfx_RectangleFilled(10,20,118,37,0x630C);
    gfx_MoveTo(13,23);
    putstr("Navigation",10);

    gfx_RectangleFilled(10,42,118,59,0x630C);
    gfx_MoveTo(13,45);
    putstr("Enregistrement",14);

    gfx_RectangleFilled(10,64,118,81,0x630C);
    gfx_MoveTo(13,67);
    putstr("Parametres",10);

    gfx_RectangleFilled(10,86,118,103,0x630C);
    gfx_MoveTo(13,89);
    putstr("Tests",5);
}

void select_main(void){
    switch(choix_ecran){
        case 0:
          choix_ecran = 4;
          select_main();          
          break;
        case 1:
          gfx_RectangleFilled(10,20,118,37,0x11F6);
          gfx_MoveTo(13,23);
          txt_BGcolour(0x11F6);
          putstr("Navigation",10);
          break;
        case 2:
          gfx_RectangleFilled(10,42,118,59,0x11F6);
          gfx_MoveTo(13,45);
          txt_BGcolour(0x11F6);
          putstr("Enregistrement",14);
          break;
        case 3:
          gfx_RectangleFilled(10,64,118,81,0x11F6);
          gfx_MoveTo(13,67);
          txt_BGcolour(0x11F6);
          putstr("Parametres",10);
          break;
        case 4:
          gfx_RectangleFilled(10,86,118,103,0x11F6);
          gfx_MoveTo(13,89);
          txt_BGcolour(0x11F6);
          putstr("Tests",5);
          break;
        case 5:
          choix_ecran = 0;
          select_main();
          break;
    }
    switch(choix_ecran_prec){
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

void choix_main_pad(void){
    if(top_actif == 1) { 
        choix_ecran_prec = choix_ecran;
        choix_ecran--;
        top_actif = 0;
        select_main();
    } else if(bot_actif == 1) {
        choix_ecran_prec = choix_ecran;
        choix_ecran++;
        bot_actif = 0;
        select_main();
    }
}


void send_char(int c){
    while (!(IFG2 & UTXIFG1)) {}
    TXBUF1 = c ;
}

void clean_screen(void){
    int i; 
    int trame_ecran[4] = {0x0F,0x0F,0x0D,0x07};
    do{
        while (!(IFG2 & UTXIFG1)) {}
        TXBUF1 = 0xFF;
        while (!(IFG2 & UTXIFG1)) {}
        TXBUF1 = 0xD7;
        while( flag_1 != 1){}
        flag_1 = 0;
    }while( rep_ecran != 6);
}

void SSTimeout(int t){
    int trame_ecran[4] = {0x0F,0x0F,0x0D,0x07};
    do{
        while (!(IFG2 & UTXIFG1)) {}
        TXBUF1 = 0x00;
        while (!(IFG2 & UTXIFG1)) {}
        TXBUF1 = 0x0C;
        while (!(IFG2 & UTXIFG1)) {}
        TXBUF1 = t>>4;
        while (!(IFG2 & UTXIFG1)) {}
        TXBUF1 = (0x00FF)&t ;


        while( flag_1 != 1){}
        flag_1 = 0;
    }while( rep_ecran != 6);
}

void gfx_ScreenMode(int mode){
    do{
        send_char(0xFF);
        send_char(0x68);
        send_char(0x00);
        send_char(mode);
        while( flag_1 != 1){}
        flag_1 = 0;
    }while( rep_ecran != 6);
}

void gfx_Rectangle(int X1, int Y1, int X2, int Y2, int color){
    do{
        send_char(0xFF);
        send_char(0xCF);
        send_char(0x00);send_char(X1);
        send_char(0x00);send_char(Y1);
        send_char(0x00);send_char(X2);
        send_char(0x00);send_char(Y2);
        send_char(color>>8);send_char(color&(0x00FF));

        while( flag_1 != 1){}
        flag_1 = 0;
    }while( rep_ecran != 6);
}

void gfx_RectangleFilled(int X1, int Y1, int X2, int Y2, int color){
    do{
        send_char(0xFF);
        send_char(0xCE);
        send_char(0x00);send_char(X1);
        send_char(0x00);send_char(Y1);
        send_char(0x00);send_char(X2);
        send_char(0x00);send_char(Y2);
        send_char(color>>8);send_char(color&(0x00FF));

        while( flag_1 != 1){}
        flag_1 = 0;
    }while( rep_ecran != 6);
}

void gfx_MoveTo(int X, int Y){
    do{
        send_char(0xFF);
        send_char(0xD6);
        send_char(0x00);send_char(X);
        send_char(0x00);send_char(Y);
        while( flag_1 != 1){}
        flag_1 = 0;
    }while( rep_ecran != 6);
}

void gfx_BGcolour(int color){
    do{
        send_char(0xFF);
        send_char(0x6E);
        send_char(color>>8);send_char(color&(0x00FF));
        while( flag_1 != 1){}
        flag_1 = 0;
    }while( rep_ecran != 6);
}

void txt_FGcolour(int color){
    do{
        send_char(0xFF);
        send_char(0x7F);
        send_char(color>>8);send_char(color&(0x00FF));
        while( flag_1 != 1){}
        flag_1 = 0;
    }while( rep_ecran != 6);
}

void txt_BGcolour(int color){
    do{
        send_char(0xFF);
        send_char(0x7E);
        send_char(color>>8);send_char(color&(0x00FF));
        while( flag_1 != 1){}
        flag_1 = 0;
    }while( rep_ecran != 6);
}

void putCH(char c){
    do{
        send_char(0xFF);
        send_char(0xFE);
        send_char(c>>4);send_char(c&(0x00FF));
        while( flag_1 != 1){}
        flag_1 = 0;
    }while( rep_ecran != 6);
}

void putstr(char* chaine,int taille){
    int i;
    do{
        send_char(0x00);
        send_char(0x06);
        for( i=0 ; i<taille ; i++){
            send_char(chaine[i]);
        }
        send_char(0x00);
        while( flag_1 != 1){}
        flag_1 = 0;
    }while( rep_ecran != 6);
}



