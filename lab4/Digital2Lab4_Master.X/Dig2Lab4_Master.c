/* 
 * File:   Dig2Lab4_Master.c
 * Author: PJPE
 *
 * Created on 8 de agosto de 2023, 03:55 PM
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
#include "I2C.h"
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

int seconds = 0;
int decSeconds = 0;
int uniSeconds = 0;
/*
 * 
 */
void setup(void);
char NumtoChar(int a);

void main(void) {
    setup();
    Lcd_Init();
    while(1){
        I2C_Master_Start();
        I2C_Master_Write(0xD0);
        I2C_Master_Write(0x00);
        //I2C_Master_Stop();
        //__delay_ms(200);
        //I2C_Master_Start();
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0xD1);
        seconds = I2C_Master_Read(0);
        I2C_Master_Stop();
        //__delay_ms(200);
        
        decSeconds = seconds>>4;
        uniSeconds = 0b00001111 & seconds;
        
        Lcd_Clear();
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String("seconds:");
        Lcd_Set_Cursor(2,1);
        Lcd_Write_Char(NumtoChar(decSeconds));
        Lcd_Write_Char(NumtoChar(uniSeconds));
    }
    
}

void setup (void){
    ANSEL = 0;
    ANSELH = 0;
    TRISB = 0;
    TRISD = 0;
    TRISE = 0;
    
    PORTB = 0;
    PORTD = 0;
    PORTE = 0;
    I2C_Master_Init(100000);        // Inicializar Comuncación I2C
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