#include <msp430.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "key.h"
#include "oled.h"
#include "usart.h"
#include "delay.h"
#include "mpu6050.h"
#include "parameter_type.h"

// Baud115200:  4MHzSMCLK, 34, 6, 0.
// OLED:        SCL -> P6.3, SDA -> P6.4
// BlueTooth:   P3.3 -> RXD, P3.4 -> TXD (115200)
// MPU6050:     SCL -> P8.1, SDA -> P8.2
// Ctrl Pins:   (from left to right) AWSD Y(low)N(high)
// Ctrl Pins:   A -> P1.2, W -> P1.3, S -> P1.4, D -> P1.5,
//              Y -> P4.2, N -> P4.1
// Ctrl Pins:   Pin(high output) == Button == GND
// P1.2 output (x) -> P2.3

#define KEY_CTRL 1
#define ATTI_CTRL 0
int Timer_count = 0;
int Ctrl_flag = KEY_CTRL;
char ctrlmode[] = "Keys";
char movement[] = "Wait..";
unsigned char ctrl_command = 0;
int Key_Change_Flag = 0;

void clock_Init(void);
void sys_Init(void);
void beep_Init(void);
void refreshParameters(float r, float p, char *move, char *mode);
void interrupt_Init(void);

// main
int main(void)
{
    sys_Init();

    float roll, pitch;

    while (True)
    {
        roll = Mpu6050AccelAngle(ACCEL_XOUT);
        pitch = Mpu6050AccelAngle(ACCEL_YOUT);

        // if press P1.1, Ctrl_flag: 1->0,0->1.
        if (!(P1IN & BIT1))
        {
            delay_ms(10);
            if (!(P1IN & BIT1))
            {
                P2IFG |= BIT1;
                // beep
                P2OUT |= BIT5;
                delay_ms(30);
                P2OUT &= ~BIT5;
                delay_ms(30);
            }
        }
        else
        {
            P2IFG &= ~BIT1;
        }

        // design ctrl channel through ctrl_flag
        if (Ctrl_flag == KEY_CTRL)
        {
            strcpy(ctrlmode, "Keys");
            ctrl_command = key_Read();
            // if first change -> send signal./
            if (!Key_Change_Flag)
            {
                Key_Change_Flag = 1;
                send_Buffer(&ctrl_command);
            }
            if (!ctrl_command)
            {
                Key_Change_Flag = 0;
            }
        }
        else if (Ctrl_flag == ATTI_CTRL) // use timer to send attitude command
        {
            strcpy(ctrlmode, "Atti");
            if (roll > 30)
                ctrl_command = 'w';
            else if (roll < -30)
                ctrl_command = 's';
            else if (pitch > 25)
                ctrl_command = 'a';
            else if (pitch < -25)
                ctrl_command = 'd';
            else
                ctrl_command = 0;
        }

        switch (ctrl_command)
        {
        case 'w':
            strcpy(movement, "Up    ");
            break;
        case 'a':
            strcpy(movement, "Left  ");
            break;
        case 's':
            strcpy(movement, "Down  ");
            break;
        case 'd':
            strcpy(movement, "Right ");
            break;
        case 'y':
            strcpy(movement, "Yes   ");
            break;
        case 'n':
            strcpy(movement, "No    ");
            break;
        default:
            strcpy(movement, "Wait..");
        }

        refreshParameters(roll, pitch, movement, ctrlmode);

        delay_ms(10);
    }
}

// clock init
void clock_Init(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    // DCO -> 4MHz, ACLK -> 32768Hz, SMCLK -> 4MHz
    UCSCTL3 = SELREF_2; // set FLL-clock -> REFO
    UCSCTL4 |= SELA_2;  // ACLK -> REFO

    __bis_SR_register(SCG0); // forbid FLL
    UCSCTL0 = 0x0000;        // set DCOx and MODx
    UCSCTL1 = DCORSEL_5;     // DCO range 16MHz
    UCSCTL2 = FLLD_1 + 122;  // set DCO double_freq FLL div_freq
    __bic_SR_register(SCG0); // enable FLL

    // wait for DCO ---
    __delay_cycles(375000);

    // set SMCLK -> DCO
    UCSCTL4 |= SELS_3;

    // clear oscillator-fault flag
    SFRIFG1 &= ~OFIFG;
    UCSCTL7 &= ~(DCOFFG + XT1LFOFFG + XT2OFFG);
    SFRIFG1 &= ~OFIFG;
}

// Init the controller
void sys_Init(void)
{
    clock_Init();
    key_Init();
    OLED_Init();
    USART0_Init();
    InitMPU6050();
    beep_Init();
    OLED_Prepare();
    interrupt_Init();

    P2IE |= BIT1;                // enable P2.6 interrupt
    P2IFG &= ~BIT1;              // clean interrupt flag
    __enable_interrupt();        // enable interrupt
}

void beep_Init(void)
{
    P2DIR |= BIT5;

    // Toot toot!
    P2OUT |= BIT5;
    delay_ms(100);
    P2OUT &= ~BIT5;
    delay_ms(50);

    P2OUT |= BIT5;
    delay_ms(30);
    P2OUT &= ~BIT5;
    delay_ms(30);
    P2OUT |= BIT5;
    delay_ms(30);
    P2OUT &= ~BIT5;
}

void refreshParameters(float r, float p, char *move, char *mode)
{
    OLED_ShowFloat(70, 3, r, 1);
    OLED_ShowFloat(70, 4, p, 1);
    OLED_ShowString(70, 6, move, 1);
    OLED_ShowString(70, 7, mode, 1);
}

void interrupt_Init(void)
{
    TA1CCR0 = 40000;
    TA1CCTL0 = CCIE;                    // CCR0 interrupt enable
    TA1CTL = TASSEL_2 + ID__8 + TAIE + TACLR + MC_1;   // use SMCLK, upper
    TA1CTL &= ~TAIFG; // clear interrupt flag
    _EINT();
}

// PORT2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void port_2(void)
{
    Ctrl_flag = 1 - Ctrl_flag;
    P2IFG &= ~BIT1;                                      // clean interrupt flag
}

// timer interrupt
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{
    if ((TA1CTL & TAIFG))
    {
        if (Ctrl_flag == ATTI_CTRL)
        {
            Timer_count++;
            if (Timer_count == 10)
            {
                send_Buffer(&ctrl_command);
                Timer_count = 0;
            }
        }
        TA1CTL &= ~TAIFG; //clear interrupt flag
    }
}

