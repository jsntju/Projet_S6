
#ifndef __io_led_pad
#define __io_led_pad

/* Permet de lancer un d�lai de t A PRECISER */
void delay(unsigned int t);

/* Permet d'initialiser les ports de l'�c pour pouvoir allumer les Leds
 * et recevoir l'�tat du port P2 */
void initialisation_LED_PAD(void);

#endif

