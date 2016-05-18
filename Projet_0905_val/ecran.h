#ifndef __ecran
#define __ecran

extern int choix_ecran;
extern int choix_ecran_prec;
extern int push_actif;
extern int top_actif;
extern int bot_actif;
extern int left_actif;
extern int right_actif;

void init_ecran(void);
void init_choix_main(void);
void init_choix_navigation(void);
void init_parcours(void);
void select_main(void);
void select_navigation(void);
void select_parcours(void);
void choix_main_pad(void);
void choix_navig_pad(void);
void choix_parcours(void);


// ------- FONCTION ELEMENTAIRE POUR LA PROGRAMMATION DE L'ECRAN ---------------
void send_char(int c);

void send_trame(char* trame_ecran, int taille_trame);

void clean_screen(void);
void SSTimeout(int t);

void gfx_ScreenMode(int mode);
void gfx_Rectangle(int X1, int Y1, int X2, int Y2, int color);
void gfx_RectangleFilled(int X1, int Y1, int X2, int Y2, int color);
void gfx_MoveTo(int X, int Y);
void gfx_BGcolour(int color);

void txt_FGcolour(int color);
void txt_BGcolour(int color);

void putCH(char c);
void putstr(char* chaine,int taille);

#endif
