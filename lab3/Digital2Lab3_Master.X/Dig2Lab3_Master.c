/* 
 * File:   Dig2Lab3_Master.c
 * Author: PJPE
 *
 * Created on 1 de agosto de 2023, 03:52 PM
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
#include "SPI.h"
#include "LCD8bits.h"
#define _XTAL_FREQ 8000000
#define RS RE0
#define EN RE1
#define D0 RD0
#define D1 RD1
#define D2 RD2
#define D3 RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7

int potValue0 = 0;
int potValue1 = 0;
int centenas0 = 0;
int decenas0 = 0;
int unidades0 = 0;
int centenas1 = 0;
int decenas1 = 0;
int unidades1 = 0;
int centenas2 = 0;
int decenas2 = 0;
int unidades2 = 0;
int cont = 0;

void setup(void);
char NumtoChar(int a);
/*
 * 
 */
int main(void) {
    setup();
    Lcd_Init();
    while(1){
        PORTCbits.RC1 = 0;       //Slave Select
       __delay_ms(5);
       
       spiWrite(0);
       potValue0 = spiRead();
       
       __delay_ms(5);
       PORTCbits.RC1 = 1;       //Slave Deselect
       __delay_ms(5);
       PORTCbits.RC1 = 0;       //Slave Select
       __delay_ms(5);
       
       spiWrite(3);
       cont = spiRead();
       
       __delay_ms(5);
       PORTCbits.RC1 = 1;       //Slave Deselect
       __delay_ms(5);
       PORTCbits.RC2 = 0;       //Slave Select
       __delay_ms(5);
       
       spiWrite(0);
       potValue1 = spiRead();
       
       __delay_ms(5);
       PORTCbits.RC2 = 1;       //Slave Deselect 
       PORTCbits.RC1 = 1;
       
       centenas0 = (int)potValue0/100;
       decenas0 = ((int)potValue0%100)/10;
       unidades0 = ((int)potValue0%100)%10;
       
       centenas1 = (int)potValue1/100;
       decenas1 = ((int)potValue1%100)/10;
       unidades1 = ((int)potValue1%100)%10;
       
       centenas2 = (int)cont/100;
       decenas2 = ((int)cont%100)/10;
       unidades2 = ((int)cont%100)%10;
       
       Lcd_Clear();
       Lcd_Set_Cursor(1,1);
       Lcd_Write_String("Pot1:");
       Lcd_Set_Cursor(2,1);
       Lcd_Write_Char(NumtoChar(centenas0));
       Lcd_Write_Char(NumtoChar(decenas0));
       Lcd_Write_Char(NumtoChar(unidades0));
       
       Lcd_Set_Cursor(1,7);
       Lcd_Write_String("Pot2:");
       Lcd_Set_Cursor(2,7);
       Lcd_Write_Char(NumtoChar(centenas1));
       Lcd_Write_Char(NumtoChar(decenas1));
       Lcd_Write_Char(NumtoChar(unidades1));
       
       Lcd_Set_Cursor(1,13);
       Lcd_Write_String("con:");
       Lcd_Set_Cursor(2,13);
       Lcd_Write_Char(NumtoChar(centenas2));
       Lcd_Write_Char(NumtoChar(decenas2));
       Lcd_Write_Char(NumtoChar(unidades2));
       __delay_ms(200);
    }
    
}

void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    
    TRISC1 = 0;
    TRISC2 = 0;
    TRISB = 0;
    TRISD = 0;
    TRISE = 0;
    
    PORTB = 0;
    PORTD = 0;
    PORTE = 0;
    
    PORTCbits.RC1 = 1;
    PORTCbits.RC2 = 1;
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
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

