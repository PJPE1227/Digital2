/* 
 * File:   Dig2prelab1.c
 * Author: PJPE
 *
 * Created on 25 de julio de 2023, 01:03 AM
 */

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>
#include <string.h>
#include "adcInterrupt.h"
#include "LCD4bits.h"

#define _XTAL_FREQ 8000000
#define RS RD2
#define EN RD3
#define D4 RD4
#define D5 RB5
#define D6 RD6
#define D7 RD7

int potValue = 0;
int one = 0;
int two = 0;
int three = 0;

void setup(void);
char NumtoChar(int a);

void __interrupt() isr(){
    if(ADIF){
        potValue = adc_read();
        PORTB = potValue;
        one = potValue/100;
        two = (potValue%100)/10;
        three = (potValue%100)%10;
        PIR1bits.ADIF = 0;
    }
}

int main(void) {
    setup();
    Lcd_Init();
    while(1){
        if(ADCON0bits.GO == 0){
            __delay_ms(10);
            ADCON0bits.GO = 1;
        }
        Lcd_Clear();
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String("Pot1 :");
        Lcd_Set_Cursor(2,1);
        Lcd_Write_Char(NumtoChar(one));
        Lcd_Write_Char(NumtoChar(two));
        Lcd_Write_Char(NumtoChar(three));
        __delay_ms(2000);
    }
}

void setup(void){
    ANSELH = 0;
    ANSEL = 0;
    
    TRISA = 0;
    TRISB = 0;
    TRISC = 0;
    TRISD = 0;
    TRISE = 0;
    
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
        
    OSCCONbits.IRCF = 0b111; //8Mhz
    OSCCONbits.SCS = 1;
    
    adc_init(0);
}

char NumtoChar(int a){
    switch(a){
        case 0:
            return '0';
        case 1:
            return '1';
        case 2:
            return '2';
        case 3:
            return '3';
        case 4:
            return '4';
        case 5:
            return '5';
        case 6:
            return '6';
        case 7:
            return '7';
        case 8:
            return '8';
        case 9:
            return '9';
    }
}