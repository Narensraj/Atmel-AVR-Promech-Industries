/*
 * uart_testing.c
 *
 * Created: 21-09-2020 2.59.05 PM
 * Author : Naren
 */ 

#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1
volatile int adcValue;
void adc_init()
{
	// AREF = AVcc
	ADMUX = (1<<REFS0);
	
	// ADC Enable and prescaler of 128
	// 16000000/128 = 125000
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}
uint16_t adc_read(uint8_t ch)
{
	ch &= 0b00000001;  
	ADMUX = (ADMUX & 0xF8)|ch; 

	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC));
	
	return (ADC);
}
void USART_Init( unsigned int ubrr)
{
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	//Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}
void USART_Transmit (char* data)
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

uint16_t adc_result0;

int main(void)
{
	USART_Init(MYUBRR);
	adc_init();

    while (1) 
    {
		adc_result0 = adc_read(0); 
		USART_Transmit(adc_result0);
		   _delay_ms(50);
	}
}
