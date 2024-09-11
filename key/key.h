// Ctrl Pins:   (from left to right) AWSD Y(low)N(high)
// Ctrl Pins:   A -> P1.2, W -> P1.3, S -> P1.4, D -> P1.5,
//              Y -> P4.2, N -> P4.1
// Ctrl Pins:   Pin(high output) == Button == GND
#include <msp430.h>
#include "parameter_type.h"

void key_Init(void);
char key_Read(void);
