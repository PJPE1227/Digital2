/* 
 * File:   adcInterrupt.h
 * Author: PJPE
 *
 * Created on 18 de julio de 2023, 05:00 PM
 */

#ifndef ADCINTERRUPT_H
#define	ADCINTERRUPT_H

#include <xc.h>

void adc_init(int channel);
int adc_read();
void adc_change_channel(int channel);
int adc_get_channel();

#endif	ADCINTERRUPT_H 

