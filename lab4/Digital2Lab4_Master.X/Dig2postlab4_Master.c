/* 
 * File:   Dig2postlab4_Master.c
 * Author: PJPE
 *
 * Created on 8 de agosto de 2023, 04:49 PM
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
#include "buttonInterrupt.h"
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
int minutes = 0;
int decMinutes = 0;
int uniMinutes = 0;
int hours = 0;
int decHours = 0;
int uniHours = 0;
int day = 0;
int decDay = 0;
int uniDay = 0;
int month = 0;
int decMonth = 0;
int uniMonth = 0;
int year = 0;
int decYear = 0;
int uniYear = 0;
int check = 0;
int contEdit = 0;
int potValue = 0;
int cenPot = 0;
int decPot = 0;
int uniPot = 0;
/*
 * 
 */
void setup(void);
char NumtoChar(int a);
int increSecMin(int b);

void __interrupt () isr(void){
    if(INTCONbits.RBIF){
        if(RB0 == 0){
            check = 1;
        }
        if(RB1 == 0){
            contEdit++;
            if(contEdit == 7){
                contEdit = 0;
            }
        }
        INTCONbits.RBIF = 0;
    }
}

void main(void) {
    setup();
    Lcd_Init();
    while(1){
        switch(contEdit){
            case 0:
                if(check == 1){
                    I2C_Master_Start();
                    I2C_Master_Write(0xD0);
                    I2C_Master_Write(0x00);
                    I2C_Master_RepeatedStart();
                    I2C_Master_Write(0xD1);
                    seconds = I2C_Master_Read(0);
                    I2C_Master_Stop();
                    seconds = increSecMin(seconds);
                    I2C_Master_Start();
                    I2C_Master_Write(0xD0);
                    I2C_Master_Write(0x00);
                    I2C_Master_Write(seconds);
                    I2C_Master_Stop();
                    check = 0;
                }       
                break;
            case 1:
                if(check == 1){
                    I2C_Master_Start();
                    I2C_Master_Write(0xD0);
                    I2C_Master_Write(0x01);
                    I2C_Master_RepeatedStart();
                    I2C_Master_Write(0xD1);
                    minutes = I2C_Master_Read(0);
                    I2C_Master_Stop();
                    minutes = increSecMin(minutes);
                    I2C_Master_Start();
                    I2C_Master_Write(0xD0);
                    I2C_Master_Write(0x01);
                    I2C_Master_Write(minutes);
                    I2C_Master_Stop();
                    check = 0;
                }
                break;
            case 2:
                if(check == 1){
                    I2C_Master_Start();
                    I2C_Master_Write(0xD0);
                    I2C_Master_Write(0x02);
                    I2C_Master_RepeatedStart();
                    I2C_Master_Write(0xD1);
                    hours = I2C_Master_Read(0);
                    I2C_Master_Stop();
                    hours = increHour(hours);
                    I2C_Master_Start();
                    I2C_Master_Write(0xD0);
                    I2C_Master_Write(0x02);
                    I2C_Master_Write(hours);
                    I2C_Master_Stop();
                    check = 0;
                }
                break;
            case 3:
                if(check == 1){
                    I2C_Master_Start();
                    I2C_Master_Write(0xD0);
                    I2C_Master_Write(0x04);
                    I2C_Master_RepeatedStart();
                    I2C_Master_Write(0xD1);
                    day = I2C_Master_Read(0);
                    I2C_Master_Stop();
                    day = increDay(day);
                    I2C_Master_Start();
                    I2C_Master_Write(0xD0);
                    I2C_Master_Write(0x04);
                    I2C_Master_Write(day);
                    I2C_Master_Stop();
                    check = 0;
                }
                break;
            case 4:
                if(check == 1){
                    I2C_Master_Start();
                    I2C_Master_Write(0xD0);
                    I2C_Master_Write(0x05);
                    I2C_Master_RepeatedStart();
                    I2C_Master_Write(0xD1);
                    month = I2C_Master_Read(0);
                    I2C_Master_Stop();
                    month = increMonth(month);
                    I2C_Master_Start();
                    I2C_Master_Write(0xD0);
                    I2C_Master_Write(0x05);
                    I2C_Master_Write(month);
                    I2C_Master_Stop();
                    check = 0;
                }
                break;
            case 5:
                if(check == 1){
                    I2C_Master_Start();
                    I2C_Master_Write(0xD0);
                    I2C_Master_Write(0x06);
                    I2C_Master_RepeatedStart();
                    I2C_Master_Write(0xD1);
                    year = I2C_Master_Read(0);
                    I2C_Master_Stop();
                    year = increYear(year);
                    I2C_Master_Start();
                    I2C_Master_Write(0xD0);
                    I2C_Master_Write(0x06);
                    I2C_Master_Write(year);
                    I2C_Master_Stop();
                    check = 0;
                }
                break;
            default:
                break;
        }
        
        I2C_Master_Start();
        I2C_Master_Write(0x50);
        I2C_Master_Write(0);
        I2C_Master_Stop();
        __delay_ms(1);
        
        I2C_Master_Start();
        I2C_Master_Write(0x51);
        potValue = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(1);
        
        I2C_Master_Start();
        I2C_Master_Write(0xD0);
        I2C_Master_Write(0x00);
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0xD1);
        seconds = I2C_Master_Read(0);
        I2C_Master_Stop();
        
        I2C_Master_Start();
        I2C_Master_Write(0xD0);
        I2C_Master_Write(0x01);
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0xD1);
        minutes = I2C_Master_Read(0);
        I2C_Master_Stop();
        
        I2C_Master_Start();
        I2C_Master_Write(0xD0);
        I2C_Master_Write(0x02);
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0xD1);
        hours = I2C_Master_Read(0);
        I2C_Master_Stop();
        
        I2C_Master_Start();
        I2C_Master_Write(0xD0);
        I2C_Master_Write(0x04);
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0xD1);
        day = I2C_Master_Read(0);
        I2C_Master_Stop();
        
        I2C_Master_Start();
        I2C_Master_Write(0xD0);
        I2C_Master_Write(0x05);
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0xD1);
        month = I2C_Master_Read(0);
        I2C_Master_Stop();
        
        I2C_Master_Start();
        I2C_Master_Write(0xD0);
        I2C_Master_Write(0x06);
        I2C_Master_RepeatedStart();
        I2C_Master_Write(0xD1);
        year = I2C_Master_Read(0);
        I2C_Master_Stop();
        
        decSeconds = seconds>>4;
        uniSeconds = 0b00001111 & seconds;
        decMinutes = minutes>>4;
        uniMinutes = 0b00001111 & minutes;
        decHours = hours>>4;
        uniHours = 0b00001111 & hours;
        decDay = day>>4;
        uniDay = 0b00001111 & day;
        decMonth = month>>4;
        uniMonth = 0b00001111 & month;
        decYear = year>>4;
        uniYear = 0b00001111 & year;
        cenPot = (int)potValue/100;
        decPot = ((int)potValue%100)/10;
        uniPot = ((int)potValue%100)%10;
        Lcd_Clear();
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String("Pot1:");
        Lcd_Set_Cursor(1,8);
        if(contEdit == 3){
            Lcd_Write_Char('>');
        }
        Lcd_Write_Char(NumtoChar(decDay));
        Lcd_Write_Char(NumtoChar(uniDay));
        Lcd_Write_Char('/');
        if(contEdit == 4){
            Lcd_Write_Char('>');
        }
        Lcd_Write_Char(NumtoChar(decMonth));
        Lcd_Write_Char(NumtoChar(uniMonth));
        Lcd_Write_Char('/');
        if(contEdit == 5){
            Lcd_Write_Char('>');
        }
        Lcd_Write_Char(NumtoChar(decYear));
        Lcd_Write_Char(NumtoChar(uniYear));
        Lcd_Set_Cursor(2,1);
        Lcd_Write_Char(NumtoChar(cenPot));
        Lcd_Write_Char(NumtoChar(decPot));
        Lcd_Write_Char(NumtoChar(uniPot));
        Lcd_Set_Cursor(2,8);
        if(contEdit == 2){
            Lcd_Write_Char('>');
        }
        Lcd_Write_Char(NumtoChar(decHours));
        Lcd_Write_Char(NumtoChar(uniHours));
        Lcd_Write_Char(':');
        if(contEdit == 1){
            Lcd_Write_Char('>');
        }
        Lcd_Write_Char(NumtoChar(decMinutes));
        Lcd_Write_Char(NumtoChar(uniMinutes));
        Lcd_Write_Char(':');
        if(contEdit == 0){
            Lcd_Write_Char('>');
        }
        Lcd_Write_Char(NumtoChar(decSeconds));
        Lcd_Write_Char(NumtoChar(uniSeconds));
    }
    
}

void setup (void){
    ANSEL = 0;
    ANSELH = 0;
    TRISA = 0;
    TRISB = 0;
    TRISD = 0;
    TRISE = 0;
    
    PORTA = 0;
    PORTB = 0;
    PORTD = 0;
    PORTE = 0;
    
    ioc_inint(0);
    ioc_inint(1);
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
int increSecMin(int b){
    int dec = b>>4;
    int uni = 0b00001111 & b;
    int val = 0;
    
    if((uni+1)== 10){
        uni = 0;
        dec++;
        if(dec == 6){
            dec = 0;
        }
    }else{
        uni++;
    }
    
    dec = (dec<<4) & 0b11110000;
    val = dec | uni;
    return val;
}
int increHour(int b){
    int dec = b>>4;
    int uni = 0b00001111 & b;
    int val = 0;
    
    
    if((uni+1)== 10){
        uni = 0;
        dec++;
    }else{
        uni++;
        if(dec == 2 && uni == 4){
            uni = 0;
            dec = 0;
        }
    }
    
    dec = (dec<<4) & 0b11110000;
    val = dec | uni;
    return val;
}
int increDay(int b){
    int dec = b>>4;
    int uni = 0b00001111 & b;
    int val = 0;
    
    
    if((uni+1)== 10){
        uni = 0;
        dec++;
    }else{
        uni++;
        if(dec == 3 && uni == 2){
            uni = 1;
            dec = 0;
        }
    }
    
    dec = (dec<<4) & 0b11110000;
    val = dec | uni;
    return val;
}
int increMonth(int b){
    int dec = b>>4;
    int uni = 0b00001111 & b;
    int val = 0;
    
    
    if((uni+1)== 10){
        uni = 0;
        dec++;
    }else{
        uni++;
        if(dec == 1 && uni == 3){
            uni = 1;
            dec = 0;
        }
    }
    
    dec = (dec<<4) & 0b11110000;
    val = dec | uni;
    return val;
}
int increYear(int b){
    int dec = b>>4;
    int uni = 0b00001111 & b;
    int val = 0;
    
    
    if((uni+1)== 10){
        uni = 0;
        dec++;
        if(dec == 10){
            dec = 0;
        }
    }else{
        uni++;
    }
    
    dec = (dec<<4) & 0b11110000;
    val = dec | uni;
    return val;
}