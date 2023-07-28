/* 
 * File:   Dig2lab2.c
 * Author: PJPE
 *
 * Created on 25 de julio de 2023, 04:17 PM
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
#include "LCD8bits.h"
#include "Usart.h"

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

int potValue = 0;
int centenaVolt = 0;
int decenaVolt = 0;
int unidadVolt = 0;
int cont = 0;
int centenaCont = 0;
int decenaCont = 0;
int unidadCont = 0;

void setup(void);
char NumtoChar(int a);

void __interrupt() isr(){
    if(ADIF){
        potValue = adc_read();
        uint16_t temp = map(potValue, 0, 255, 0, 50)*10;
        centenaVolt = temp/100;
        decenaVolt = (temp%100)/10;
        unidadVolt = (temp%100)%10;
        PIR1bits.ADIF = 0;
    }
    if(PIR1bits.RCIF){
        UART_write_char(RCREG);
        char tempEntry = UART_read_char();
        
        if(tempEntry == 43){
            if(cont == 255){
                cont = 255;
            }else{
                cont++;
            }

        }
        if(tempEntry == 45){
            if(cont == 0){
                cont = 0;
            }else{
                cont--;
            }
        }
        centenaCont = cont/100;
        decenaCont = (cont%100)/10;
        unidadCont = (cont%100)%10;
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
        Lcd_Set_Cursor(1,2);
        Lcd_Write_String("Pot1 :");
        Lcd_Set_Cursor(2,3);
        Lcd_Write_Char(NumtoChar(centenaVolt));
        Lcd_Write_Char('.');
        Lcd_Write_Char(NumtoChar(decenaVolt));
        Lcd_Write_Char(NumtoChar(unidadVolt));
        Lcd_Set_Cursor(1,11);
        Lcd_Write_String("CPU :");
        Lcd_Set_Cursor(2,11);
        Lcd_Write_Char(NumtoChar(centenaCont));
        Lcd_Write_Char(NumtoChar(decenaCont));
        Lcd_Write_Char(NumtoChar(unidadCont));
        __delay_ms(500);
    }
}

void setup(void){
    ANSELH = 0;
    ANSEL = 0;
    
    TRISA = 0;
    TRISB = 0;
    TRISD = 0;
    TRISE = 0;
    
    PORTA = 0;
    PORTB = 0;
    PORTD = 0;
    PORTE = 0;
        
    OSCCONbits.IRCF = 0b111; //8Mhz
    OSCCONbits.SCS = 1;
    
    adc_init(0);
    UART_TX_config(207);
    UART_RX_config(207);
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