#include <stdlib.h>

void main(void){ 

while(1)
{
    float cap1 = 0;
    selec_trame_vtg();
    
    cap1 = selec_cap (trame_vtg, cap1);
     debug_printf("Cap main %f\n",cap1);
     boussole_LED(cap1);
}
}
