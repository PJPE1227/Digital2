#include "adcInterrupt.h"
#define _XTAL_FREQ 8000000

void adc_init(int channel){
    
    switch(channel){
        case 0:
            ANSEL = ANSEL | 0b00000001;
            TRISA = TRISA | 0b00000001;
            ADCON0bits.CHS = 0b0000;
            break;
        case 1:
            ANSEL = ANSEL | 0b00000010;
            TRISA = TRISA | 0b00000010;
            ADCON0bits.CHS = 0b0001;
            break;
        case 2:
            ANSEL = ANSEL | 0b00000100;
            TRISA = TRISA | 0b00000100;
            ADCON0bits.CHS = 0b0010;
            break;
        case 3:
            ANSEL = ANSEL | 0b00001000;
            TRISA = TRISA | 0b00001000;
            ADCON0bits.CHS = 0b0011;
            break;
        case 4:
            ANSEL = ANSEL | 0b00010000;
            TRISA = TRISA | 0b00100000;
            ADCON0bits.CHS = 0b0100;
            break;
        case 5:
            ANSEL = ANSEL | 0b00100000;
            TRISE = TRISE | 0b00000001;
            ADCON0bits.CHS = 0b0101;
            break;
        case 6:
            ANSEL = ANSEL | 0b01000000;
            TRISE = TRISE | 0b00000010;
            ADCON0bits.CHS = 0b0110;
            break;
        case 7:
            ANSEL = ANSEL | 0b10000000;
            TRISE = TRISE | 0b00000100;
            ADCON0bits.CHS = 0b0111;
            break;
        case 8:
            ANSELH = ANSELH | 0b00000001;
            TRISB = TRISB | 0b00000100;
            ADCON0bits.CHS = 0b1000;
            break;
        case 9:
            ANSELH = ANSELH | 0b00000010;
            TRISB = TRISB | 0b00001000;
            ADCON0bits.CHS = 0b1001;
            break;
        case 10:
            ANSELH = ANSELH | 0b00000100;
            TRISB = TRISB | 0b00000010;
            ADCON0bits.CHS = 0b1010;
            break;
        case 11:
            ANSELH = ANSELH | 0b00001000;
            TRISB = TRISB | 0b00010000;
            ADCON0bits.CHS = 0b1011;
            break;
        case 12:
            ANSELH = ANSELH | 0b00010000;
            TRISB = TRISB | 0b00000001;
            ADCON0bits.CHS = 0b1100;
            break;
        case 13:
            ANSELH = ANSELH | 0b00100000;
            TRISB = TRISB | 0b00100000;
            ADCON0bits.CHS = 0b1101;
            break;
    }
    
    ADCON1bits.ADFM = 0;
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
    
    ADCON0bits.ADCS = 0b10;
    ADCON0bits.ADON = 1;
    __delay_us(50);
    
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    return;
}
int adc_read(){
    return ADRESH;
}
void adc_change_channel(int channel){
    
    //ADCON0bits.CHS = channel;
    return;
}
int adc_get_channel(){
    return ADCON0bits.CHS;
}
