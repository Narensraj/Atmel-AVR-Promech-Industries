/*
 * timer_test.c
 *
 * Created: 28-09-2020 3.14.22 PM
 * Author : Naren
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#define TOGGLEBIT(x,y) (x^=(1<<y))    // Toggle bit y in byte x
int data=0;

void timer1_init()
{
	cli(); // stop interrupts
	TCCR1A = 0; // set entire TCCR1A register to 0
	TCCR1B = 0; // same for TCCR1B
	TCNT1  = 0; // initialize counter value to 0
	// set compare match register for 1 Hz increments
	OCR1A = 31249; // = 8000000 / (256 * 1) - 1 (must be <65536)
	// turn on CTC mode
	TCCR1B |= (1 << WGM12);
	// Set CS12, CS11 and CS10 bits for 256 prescaler
	TCCR1B |= (1 << CS12) | (0 << CS11) | (0 << CS10);
	// enable timer compare interrupt
	TIMSK1 |= (1 << OCIE1A);
	sei(); // allow interrupts
}
ISR(TIMER1_COMPA_vect)
{
	 data=1;
}

int main(void)
{
	DDRB = 0b00000001;
    timer1_init();
    while (1) 
    {
		if(data==1)
		{
			PORTB = TOGGLEBIT(PINB0,1);
			data = 0;	
		}
			
    }
}

