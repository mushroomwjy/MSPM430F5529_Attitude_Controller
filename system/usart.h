#include <msp430.h>
#include "delay.h"
#include "oled.h"

char rxbuffer;

void USART0_Init(void);
void send_Buffer(unsigned char *txbuffer);
