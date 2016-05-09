#ifndef __boussole
#define __boussole

float DMS_en_DD(char* coord,int lati_longi, char* cardi);

float direction_nord(float longi, float lati);

void boussole_LED(float azimut);

#endif
