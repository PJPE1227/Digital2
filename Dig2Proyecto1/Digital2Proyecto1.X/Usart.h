/* 
 * File:   Usart.h
 * Author: PJPE
 *
 * Created on 26 de julio de 2023, 09:14 PM
 */

#ifndef USART_H
#define	USART_H

#include <xc.h>

void UART_RX_config(uint16_t baudrate);
void UART_TX_config(uint16_t baudrate);
void UART_write_char(char c);
char UART_read_char();

#endif	/* USART_H */

