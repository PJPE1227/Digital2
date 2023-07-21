/* 
 * File:   Dig2lab1.c
 * Author: PJPE
 *
 * Created on 18 de julio de 2023, 04:09 PM
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
#include "buttonInterrupt.h"
#include "adcInterrupt.h"
#define _XTAL_FREQ 8000000
#define _tmr0_value 250

int potValue = 0;
int secondNum = 0;
int firstNum = 0;
int display1 = 0;
int display2 = 0;
int cont0 = 0;
int alarm = 0;

void setup(void);
int _displays(int a);

void __interrupt()isr(void){
    if(T0IF){
        PORTD = 0;
        PORTC = 0;
        if(cont0 == 0){
            PORTC = (int)display1;
            PORTDbits.RD5 = 1;
            cont0++;
        }else if(cont0 == 1){
            PORTC = (int)display2;
            PORTDbits.RD4 = 1;
            cont0 = 0;
        }
        if(potValue > alarm){
            PORTDbits.RD7 = 1;
        }else{
            PORTDbits.RD7 = 0;
        }
        T0IF = 0;
    }
    
    if(INTCONbits.RBIF){
        if(RB0 == 0){
            PORTA++;
            alarm = PORTA;
        }
        if(RB1 == 0){
            PORTA--;
            alarm = PORTA;
        }
        INTCONbits.RBIF = 0;
    }
    
    if(ADIF){
        potValue = adc_read();
        PIR1bits.ADIF = 0;
    }
}

int main(void) {
    setup();
    while(1){
        secondNum = (int)potValue/16;
        display1 = _displays(secondNum);
        firstNum = ((int)potValue%16)%16;
        display2 = _displays(firstNum);
       if(ADCON0bits.GO == 0){
            __delay_ms(10);
            ADCON0bits.GO = 1;
        } 
       
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
    
    //config tmrm0
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS = 0b110;
    TMR0 = _tmr0_value;
    
    // configuracion de la interrupcion tmr0
    INTCONbits.T0IF = 0;
    INTCONbits.T0IE = 1;
    INTCONbits.PEIE = 0;
    INTCONbits.GIE = 1;
    
    ioc_inint(0);
    ioc_inint(1);
    
    adc_init(8);
    
    return;
}

int _displays(int a){
    switch(a){
        case 0:
            return 0b11000000; //0
        case 1:
            return 0b11111001; //1
        case 2:
            return 0b10100100; //2
        case 3:
            return 0b10110000; //3
        case 4:
            return 0b10011001; //4
        case 5:
            return 0b10010010; //5
        case 6:
            return 0b10000010; //6
        case 7:
            return 0b11111000; //7
        case 8:
            return 0b10000000; //8
        case 9:
            return 0b10010000; //9
        case 10:
            return 0b10001000; //a
        case 11:
            return 0b10000011; //b
        case 12:
            return 0b11000110; //c
        case 13:
            return 0b10100001; //d
        case 14:
            return 0b10000110; //e
        case 15:
            return 0b10001110; //f
        default:
            return 0b00000000;
    }
}

