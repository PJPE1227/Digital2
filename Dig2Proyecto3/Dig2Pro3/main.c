#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"
#include "driverlib/adc.h"
#include "driverlib/pin_map.h"
#include "utils/uartstdio.h"

uint32_t ui32Period;
uint32_t adc0[1];
uint32_t adc1[1];
uint32_t adc2[1];
uint32_t adc3[1];

char parking0;
char parking1;
char parking2;
char parking3;

uint32_t count = 0;

void InitConsole(void){
    // se inicia el puerto UART 7
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    GPIOPinConfigure(GPIO_PE0_U7RX);
    GPIOPinConfigure(GPIO_PE1_U7TX);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART7);

    GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTConfigSetExpClk(UART7_BASE, SysCtlClockGet(),115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
}

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ| SYSCTL_OSC_MAIN);//se inicial el reloj a 40Mhz

    InitConsole();

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);// se inicializan los puertos que se usaran para controlar los leds
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_6 | GPIO_PIN_7);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);//se inicializan los puertos que se ADC para leer las LDR

    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceConfigure(ADC0_BASE, 2, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);

    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH5|ADC_CTL_IE|ADC_CTL_END);
    ADCSequenceStepConfigure(ADC0_BASE, 2, 0, ADC_CTL_CH4|ADC_CTL_IE|ADC_CTL_END);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH6|ADC_CTL_IE|ADC_CTL_END);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH7|ADC_CTL_IE|ADC_CTL_END);

    ADCSequenceEnable(ADC0_BASE, 3);
    ADCSequenceEnable(ADC0_BASE, 2);
    ADCSequenceEnable(ADC0_BASE, 1);
    ADCSequenceEnable(ADC0_BASE, 0);

    ADCIntClear(ADC0_BASE, 3);
    ADCIntClear(ADC0_BASE, 2);
    ADCIntClear(ADC0_BASE, 1);
    ADCIntClear(ADC0_BASE, 0);


	while(1){
	    ADCProcessorTrigger(ADC0_BASE, 3);// se activa la lectura de los ADCs
	    ADCProcessorTrigger(ADC0_BASE, 2);
        ADCProcessorTrigger(ADC0_BASE, 1);
        ADCProcessorTrigger(ADC0_BASE, 0);

	    while(!ADCIntStatus(ADC0_BASE, 3, false) || !ADCIntStatus(ADC0_BASE, 2, false) || !ADCIntStatus(ADC0_BASE, 1, false) || !ADCIntStatus(ADC0_BASE, 0, false)){

	    }

	    ADCIntClear(ADC0_BASE, 3); // se limpian las banderas de interrupcion
	    ADCIntClear(ADC0_BASE, 2);
        ADCIntClear(ADC0_BASE, 1);
        ADCIntClear(ADC0_BASE, 0);
	    ADCSequenceDataGet(ADC0_BASE, 3, adc0); //se obtiene la lectura de los ADC
	    ADCSequenceDataGet(ADC0_BASE, 2, adc1);
        ADCSequenceDataGet(ADC0_BASE, 1, adc2);
        ADCSequenceDataGet(ADC0_BASE, 0, adc3);

	    if(adc0[0] < 500){
	        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6 | GPIO_PIN_7, 64);//se determina si hay un objeto obstruyendo la luz y se prender la luz respectiva
	        parking0 = '0';
	    }else{
	        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6 | GPIO_PIN_7, 128);
	        parking0 = '1';
	    }

	    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6 | GPIO_PIN_7, 0);

        if(adc1[0] < 500){
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7, 64);
            parking1 = '0';
        }else{
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7, 128);
            parking1 = '1';
        }

        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7, 0);

        if(adc2[0] < 500){
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5, 32);
            parking2 = '0';
        }else{
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5, 16);
            parking2 = '1';
        }

        GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5, 0);

        if(adc3[0] < 500){
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3, 4);
            parking3 = '0';
        }else{
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3, 8);
            parking3 = '1';
        }

        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3, 0);
        count++;
        if(count == 60000){//se envian las lecturas por uart al esp32 cada cierto tiempo
            UARTCharPut(UART7_BASE, 'x');
            UARTCharPut(UART7_BASE, parking3);
            UARTCharPut(UART7_BASE, parking2);
            UARTCharPut(UART7_BASE, parking0);
            UARTCharPut(UART7_BASE, parking1);
            UARTCharPut(UART7_BASE, 10);
            UARTCharPut(UART7_BASE, 13);
            count = 0;
        }
	}
}


