
#ifndef __gps
#define __gps

void init_uart0(void);
void usart0_rx (void);
void usart1_rx (void);
void activer_communication_USB(void);
void activer_communication_GPS(void);

void selec_trame_gga(void);/*char *buf_0);*/
char* selec_objet (char * objet);
char* initialise_obj (char *objet, int taille);
void gps_gga (char *buf_0);
char * select_buf0(void);
int search (char *s, char carac, int debut);
char * substr(char *buf, const char *s, size_t beg, size_t len);
int GGA_OK (void);

/* Variable globale */
extern char buf_0[200];
extern char buf_1[200];
extern char trame[100];
extern char UTC[30];
extern char latitude[10];
extern char NS[1];               //Nord ou Sud
extern char longitude[10];
extern char EW[1];              //Est ou Ouest
extern char Pos_ind[10];         //Position fix indicator
extern char nb_sat[10];          //nombre de sattelites
extern char HDOP[10];            //horizontal dilution of precision
extern char alti[10];            //altitude
extern char unit_alti[2];        //unité  d'altitude
extern char geoid[10];            //geoid separation
extern char unit_geo[2];        //unité  geod separation
extern char age_diff[10];      // Age of Diff. Corr
extern char id_station[10];    // Diff. Ref. Station ID
extern char checksum[10];
extern int h;

extern int rep_ecran;
extern int flag_1;

#endif

