// Ctrl Pins:   (from left to right) AWSD Y(low)N(high)
// Ctrl Pins:   A -> P1.2, W -> P1.3, S -> P1.4, D -> P1.5,
//              Y -> P4.2, N -> P4.1
// Ctrl Pins:   Pin(high output) == Button == GND
#include "key.h"
#include "delay.h"

char AWSDNY[6] = { 'a', 'w', 's', 'd', 'n', 'y' };

void key_Init(void)
{
    // Set Inputs
    P1DIR &= ~(BIT1 + BIT3 + BIT4 + BIT5);
    P4DIR &= ~(BIT1 + BIT2);
    P2DIR &= ~BIT3;
    // Pull up
    P1REN = (BIT1 + BIT3 + BIT4 + BIT5);
    P4REN = (BIT1 + BIT2 );
    P2REN = BIT3;
    // Output High
    P1OUT |= (BIT1 + BIT3 + BIT4 + BIT5);
    P4OUT |= (BIT1 + BIT2 );
    P2OUT |= BIT3;
}

char key_Read(void)
{
    char ret_command;
    // if press P1.1, Ctrl_flag: 1->0,0->1.
    if (!(P2IN & BIT3))
    {
        delay_ms(10);
        if (!(P2IN & BIT3))
        {
            ret_command = AWSDNY[0];
        }
    }
    else if (!(P1IN & BIT3))
    {
        delay_ms(10);
        if (!(P1IN & BIT3))
        {
            ret_command = AWSDNY[1];
        }
    }
    else if (!(P1IN & BIT4))
    {
        delay_ms(10);
        if (!(P1IN & BIT4))
        {
            ret_command = AWSDNY[2];
        }
    }
    else if (!(P1IN & BIT5))
    {
        delay_ms(10);
        if (!(P1IN & BIT5))
        {
            ret_command = AWSDNY[3];
        }
    }
    else if (!(P4IN & BIT1))
    {
        delay_ms(10);
        if (!(P4IN & BIT1))
        {
            ret_command = AWSDNY[4];
        }
    }
    else if (!(P4IN & BIT2))
    {
        delay_ms(10);
        if (!(P4IN & BIT2))
        {
            ret_command = AWSDNY[5];
        }
    }
    else
        ret_command = 0;
    return ret_command;

}
