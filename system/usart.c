//initialize
#include "usart.h"
extern char rxbuffer;

void USART0_Init(void)
{
    P3SEL |= BIT3 + BIT4;                        // P3.3,4 = USCI_A1 TXD/RXD
    UCA0CTL1 |= UCSWRST;                         // **Put state machine in reset**
    UCA0CTL1 |= UCSSEL__SMCLK;                    // ACLK
    UCA0BR0 = 34;                              // BoudRate=115200
    UCA0BR1 = 0x00;                             // 32768Hz 115200
    UCA0MCTL |=  UCBRS_6 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    UCA0IE |= UCRXIE;                          // Enable USCI_A1 RX interrupt
    UCA0IFG &= ~UCRXIFG;       //clear interrupt flags
}

// send buffer
void send_Buffer(unsigned char *txbuffer)
{
    while (*txbuffer != '\0')
    {
//      txbuffer_shown following used to test as typewriter: send_Buffer("love Yanyan OvO\n");
//      unsigned char* txbuffer_shown = txbuffer;
        while (!(UCA0IFG & UCTXIFG))
            ;
        UCA0TXBUF = *txbuffer;
        txbuffer++;
//      OLED_ShowString(1, 3, txbuffer_shown, 1);
//      OLED_Clear();
        delay_ms(10);
    }
}


// interrupt callback
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
    UCA0IFG &= ~UCRXIFG;
    // receive rxbuffer
    if((UCA0IFG)&(UCRXIFG)){
        rxbuffer = UCA0RXBUF;
        if(rxbuffer == 'a')
            {
                P4OUT &= ~BIT7;
                P1OUT |= BIT0;
            }
        else if(rxbuffer =='b')
        {
            P1OUT &= ~BIT0;
            P4OUT |= BIT7;
        }
        else
            ;
    }
}
