#include "buttonInterrupt.h"

void ioc_inint(int pin){
    
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    
    
    
    INTCONbits.RBIE = 1;
    INTCONbits.RBIF = 0;
    
    switch(pin){
        case 0:
            TRISB = TRISB | 0b00000001;
            OPTION_REGbits.nRBPU = 0; //enable individual pullups
            WPUBbits.WPUB0 = 1;
            IOCBbits.IOCB0 = 1;
        break;
        case 1:
            TRISB = TRISB | 0b00000010;
            OPTION_REGbits.nRBPU = 0; //enable individual pullups
            WPUBbits.WPUB1 = 1;
            IOCBbits.IOCB1 = 1;
        break;
        case 2:
            TRISB = TRISB | 0b00000100;
            OPTION_REGbits.nRBPU = 0; //enable individual pullups
            WPUBbits.WPUB2 = 1;
            IOCBbits.IOCB2 = 1;
            break;
        case 3:
            TRISB = TRISB | 0b00001000;
            OPTION_REGbits.nRBPU = 0; //enable individual pullups
            WPUBbits.WPUB3 = 1;
            IOCBbits.IOCB3 = 1;
            break;
        case 4:
            TRISB = TRISB | 0b00010000;
            OPTION_REGbits.nRBPU = 0; //enable individual pullups
            WPUBbits.WPUB4 = 1;
            IOCBbits.IOCB4 = 1;
            break;
        case 5:
            TRISB = TRISB | 0b00100000;
            OPTION_REGbits.nRBPU = 0; //enable individual pullups
            WPUBbits.WPUB5 = 1;
            IOCBbits.IOCB5 = 1;
            break;
        case 6:
            TRISB = TRISB | 0b01000000;
            OPTION_REGbits.nRBPU = 0; //enable individual pullups
            WPUBbits.WPUB6 = 1;
            IOCBbits.IOCB6 = 1;
            break;
        case 7:
            TRISB = TRISB | 0b10000000;
            OPTION_REGbits.nRBPU = 0; //enable individual pullups
            WPUBbits.WPUB7 = 1;
            IOCBbits.IOCB7 = 1;
            break;
    }
    return;
}
