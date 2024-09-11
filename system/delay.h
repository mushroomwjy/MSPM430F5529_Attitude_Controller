#include <msp430.h>

#define CPU_F ((double)12000000)
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
