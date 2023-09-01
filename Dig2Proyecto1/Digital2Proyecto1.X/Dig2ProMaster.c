/* 
 * File:   Dig2ProMaster.c
 * Author: PJPE
 *
 * Created on 24 de agosto de 2023, 12:42 AM
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
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <string.h>
#include "I2C.h"
#include "LCD8bits.h"
#include "floatstr.h"
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
float temperature = 0;
char buffer[5];
char data[6];
uint16_t ch0 = 0;
uint16_t ch1 = 0;
uint16_t ch2 = 0;
char bufferch0[48];
char bufferch1[48];
char bufferch2[48];
char buffTime[10];
int seconds = 0;
int decSeconds = 0;
int uniSeconds = 0;
int minutes = 0;
int decMinutes = 0;
int uniMinutes = 0;
int hours = 0;
int decHours = 0;
int uniHours = 0;
int light = 0;
char bufferTotal[15];

int DCmotor = 0;
int nothing = 0;

void setup(void);
char NumtoChar(int a);

void RTC(void);
void AHT10_Soft_Reset(void);
void AHT10_Read(void);
void AHT10_Init(void);
void TSL2561_Init(void);
void TSL2561_Read(void);
void SendESP(void);
void cadena(char txt[]);
void DCOnOff(void);
/*
 * 
 */
void main(void) {
    setup();
    Lcd_Init();
    AHT10_Soft_Reset();
    AHT10_Init();
    TSL2561_Init();
    while(1){
        /*AHT10_Soft_Reset();
        AHT10_Init();*/
        AHT10_Read();
        //TSL2561_Init();
        TSL2561_Read();
        //Lcd_Set_Cursor(1,1);
        //Lcd_Write_String("idk");
        RTC();
        SendESP();
        DCOnOff();
    }
    
}

void DCOnOff(void){
    if(light == 1 && (int)temperature >= 25){
        PORTBbits.RB7 = 1;
    }else{
        PORTBbits.RB7 = 0;
    }
    PORTA = DCmotor;
}

void setup(void){
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
    
    I2C_Master_Init(100000);        // Inicializar Comuncación I2C
    OSCCONbits.IRCF = 0b111; //8Mhz
    OSCCONbits.SCS = 1;
    
    UART_TX_config(207);
    UART_RX_config(207);
}

void cadena(char txt[]){
    int cont0 = 0;
    while(txt[cont0]!=0){
        cont0++;
    }
    //cont0++;
    //cont0 = sizeof(txt)/sizeof(char);
    for(int i = 0; i < cont0; i++){
        __delay_ms(20);
        TXREG = txt[i];
    }
    return;
}

void SendESP(void){
    UART_write_char('x');
    bufferTotal[0] = buffer[0];
    bufferTotal[1] = buffer[1];
    bufferTotal[2] = buffer[2];
    bufferTotal[3] = buffer[3];
    bufferTotal[4] = buffer[4];
    bufferTotal[5] = buffTime[0];
    bufferTotal[6] = buffTime[1];
    bufferTotal[7] = buffTime[2];
    bufferTotal[8] = buffTime[3];
    bufferTotal[9] = buffTime[4];
    bufferTotal[10] = buffTime[5];
    bufferTotal[11] = buffTime[6];
    bufferTotal[12] = buffTime[7];
    bufferTotal[13] = NumtoChar(light);
    bufferTotal[14] = '\n';
    cadena(bufferTotal);
}

void RTC(void){
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

    decSeconds = seconds>>4;
    uniSeconds = 0b00001111 & seconds;
    decMinutes = minutes>>4;
    uniMinutes = 0b00001111 & minutes;
    decHours = hours>>4;
    uniHours = 0b00001111 & hours;

    buffTime[0] = NumtoChar(decHours);
    buffTime[1] = NumtoChar(uniHours);
    buffTime[2] = ':';
    buffTime[3] = NumtoChar(decMinutes);
    buffTime[4] = NumtoChar(uniMinutes);
    buffTime[5] = ':';
    buffTime[6] = NumtoChar(decSeconds);
    buffTime[7] = NumtoChar(uniSeconds);

    Lcd_Set_Cursor(1,1);
    /*Lcd_Write_Char(NumtoChar(decHours));
    Lcd_Write_Char(NumtoChar(uniHours));
    Lcd_Write_Char(':');
    Lcd_Write_Char(NumtoChar(decMinutes));
    Lcd_Write_Char(NumtoChar(uniMinutes));
    Lcd_Write_Char(':');
    Lcd_Write_Char(NumtoChar(decSeconds));
    Lcd_Write_Char(NumtoChar(uniSeconds));*/
    Lcd_Write_String(buffTime);
}

void AHT10_Init(void){
    __delay_ms(40); //delay de 40ms
    uint8_t status; //variable para status del sensor
    I2C_Master_Start(); //Inicializar comunicación I2C
    I2C_Master_Write(0x70); //Direccion de sensor de temperatura
    I2C_Master_Write(0x71); //Enviar para obtener el status del sensor
    I2C_Master_RepeatedStart(); //Repeated Start
    I2C_Master_Write(0x71); //Leer del sensor de temperatura
    status = I2C_Master_Read(0); //Guardar status
    I2C_Master_Stop(); //detener comunicacion*/
    
    I2C_Master_Start(); //Inicializar comunicación I2C
    I2C_Master_Write(0x70); //Direccion de sensor de temperatura
    I2C_Master_Write(0xE1); //Enviar secuencia de inicialización
    I2C_Master_Write(0x08);
    I2C_Master_Write(0x00);
    I2C_Master_Stop(); //detener comunicacion

    __delay_ms(10); //delay de 10ms
    
}

void AHT10_Read(void){
    uint8_t data[7]; //arreglo para guardar los datos recibidos del sensor de temperatura
    uint8_t r; //Variable para determinar si el sensor está listo para volver a realizar una medición
    I2C_Master_Start(); //Inicializar comunicación I2C
    I2C_Master_Write(0x70); //Direccion de sensor de temperatura
    I2C_Master_Write(0xAC); //Enviar secuencia de medición
    I2C_Master_Write(0x33);
    I2C_Master_Write(0x00);
    I2C_Master_Stop(); //detener comunicacion
    __delay_ms(80); //delay de 80ms
    
    I2C_Master_Start(); //Inicializar comunicación I2C
    I2C_Master_Write(0x70); //Direccion de sensor de temperatura
    I2C_Master_Write(0x71); //Secuencia para obtener status
    I2C_Master_RepeatedStart(); //Repeated start
    I2C_Master_Write(0x71); //Dirección mas bit de escritura
    r = I2C_Master_Read(0); //Guardar status
    I2C_Master_Stop(); //detener comunicacion
    
    r = r & 0b00000000; //convertir todas las variables en 0
    while (r != 0b00000000); //Mientras sean 0 no hacer nada
    
    I2C_Master_Start(); //Inicializar comunicación I2C
    I2C_Master_Write(0x71); //Enviar dirección mas bit de escritura
    data[0] = I2C_Master_Read(1); //Guardar
    data[1] = I2C_Master_Read(1); //Guardar status
    data[2] = I2C_Master_Read(1); //Guardar valor de humedad 1
    data[3] = I2C_Master_Read(1); //Guardar valor de humedad 2
    data[4] = I2C_Master_Read(1); //Guardar valor de humedad 3 y temperatura 1
    data[5] = I2C_Master_Read(1); //Guardar valor de temperatura 2
    data[6] = I2C_Master_Read(0); //Guardar valor de temperatura 3
    I2C_Master_Stop(); //detener comunicacion
    
	temperature = (((uint32_t)data[3] & 0x0F) << 16) + ((uint16_t)data[4] << 8) + data[5]; //Unir datos de temperatura en uno solo
    temperature = ((temperature/1048576)*200-50); //Realizar conversión indicada por el fabricante
    //PORTB = temperature;
    floattostr(temperature, buffer, 2); //Convertir el dato a cadena
    Lcd_Set_Cursor(1,12); //Setear cursor en 1,12
    Lcd_Write_String(buffer); //Mostrar en LCD
}

void AHT10_Soft_Reset(void){
    __delay_ms(40); //delay de 40ms
    I2C_Master_Start(); //Inicializar comunicación I2C
    I2C_Master_Write(0x70); //Direccion de sensor de temperatura
    I2C_Master_Write(0xBA);//Enviar secuencia de reset
    I2C_Master_Stop(); //detener comunicacion
    __delay_ms(25); //delay de 25ms
}

void TSL2561_Init(void){
    I2C_Master_Start();
    I2C_Master_Write(0x72);
    I2C_Master_Write(0x80);
    I2C_Master_Write(0x03);
    I2C_Master_Stop();
    
    I2C_Master_Start();
    I2C_Master_Write(0x72);
    I2C_Master_Write(0x81);
    I2C_Master_Write(0x10);
    I2C_Master_Stop();
    __delay_ms(300);
}

void TSL2561_Read(void){
    I2C_Master_Start();
    I2C_Master_Write(0x72);
    I2C_Master_Write(0x8C);
    I2C_Master_RepeatedStart();
    I2C_Master_Write(0x73);

    data[0] = I2C_Master_Read(0);
    data[1] = I2C_Master_Read(0);
    data[2] = I2C_Master_Read(0);
    data[3] = I2C_Master_Read(1);
    //data[4] = I2C_Master_Read(1);

    I2C_Master_Stop();
    //ch0 = data[1];
    //ch0 = ch0 * 256;
    //ch0 = ch0 + data[0];
    ch0 = (data[1] << 8) | data[0];
    //ch1 = data[3];
    //ch1 = ch1 * 256;
    //ch1 = ch1 + data[2];
    ch1 = (data[3] << 8) | data[2];
    ch2 = ch0 - ch1;
    //ch0 = (ch0 << 8)/ 65536;
    /*ch0 = (ch0/65536);
    ch1 = (ch1/65536);
    ch2 = (ch2/65536);

    /*floattostr(-(float)ch0, bufferch0, 0);
    floattostr(-(float)ch1, bufferch1, 0);
    floattostr(-(float)ch2, bufferch2, 0);*/

    sprintf(bufferch0, "%u", ch0);
    sprintf(bufferch1, "%u", ch1);
    sprintf(bufferch2, "%u", ch2);

    Lcd_Set_Cursor(2,1);
    if(ch0 <= 65290){
        Lcd_Write_String("dark ");
        light = 0;
    }else{
        Lcd_Write_String("light");
        light = 1;
    }
    //Lcd_Write_String(bufferch1);
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