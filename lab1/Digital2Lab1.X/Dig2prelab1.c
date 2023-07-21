/* 
 * File:   Dig2prelab1.c
 * Author: PJPE
 *
 * Created on 18 de julio de 2023, 03:42 PM
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
#define _XTAL_FREQ 1000000

int cont = 0;
int cont2 = 0;

void setup(void);

void __interrupt()isr(void){

}

int main(void) {
    setup();
    while(1){
        if(RB0 == 0){
            cont = 1;
        }
        if(RB0 == 1 && cont == 1){
            PORTA++;
            cont = 0;
        }
        if(RB1 == 0){
            cont2 = 1;
        }
        if(RB1 == 1 && cont2 == 1){
            PORTA--;
            cont2 = 0;
        }
    }
}

void setup(void){
    ANSELH = 0;
    ANSEL = 0;
    
    TRISA = 0;
    TRISB = 0b00000011;
    
    PORTA = 0;
    PORTB = 0;
    
    //configuradion de pullups
    OPTION_REGbits.nRBPU = 0; //enable individual pullups
    WPUBbits.WPUB0 = 1;
    WPUBbits.WPUB1 = 1;
    
     return;
}

