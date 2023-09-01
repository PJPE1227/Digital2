#include "Usart.h"

void UART_RX_config(uint16_t baudrate){
    
    BAUDCTLbits.BRG16 = 1;
    
    SPBRG = baudrate;
    SPBRGH = 0;
    
    RCSTAbits.SPEN = 1;
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;
    
    PIR1bits.RCIF = 0;
    PIE1bits.RCIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    return;
}
void UART_TX_config(uint16_t baudrate){
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    
    BAUDCTLbits.BRG16 = 1;
    
    SPBRG = baudrate;
    SPBRGH = 0;
    
    TXSTAbits.TXEN = 1;
    return;
}
void UART_write_char(char c){
    TXREG = c;
    return;
}
char UART_read_char(){
    return RCREG;
}