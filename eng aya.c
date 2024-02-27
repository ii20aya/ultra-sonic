#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <stdbool.h>
#include "macro.h"
#include "DIO.h"
#include "lcd.h"
#include "keypad.h"
#include "7seg.h"
#define cc //common cathod for 7 segment
#include <avr/interrupt.h>//to use bilt in functions in interrupt
#include <stdio.h>
#include <math.h>
#include <float.h>
#define NOTPRESSED 0xff
volatile int flag1=0;
int main(void)
{
	LCD_inti();
	unsigned short a,b,high,distance;
	DIO_SET_BINDIR('D',7,1);
	while(1)
	{
		TCCR1A = 0;
		TIFR = (1<<ICF1);  	/* Clear ICF (Input Capture flag)  */
		DIO_WRITE('D',7,1);
		_delay_us(50);
		DIO_WRITE('D',7,0);
		
		TCCR1B = 0xc1;  	/* Rising edge, no prescaler , noise canceler*/
		while ((TIFR&(1<<ICF1)) == 0);
		a = ICR1;  		/* Take value of capture register */
		TIFR = (1<<ICF1);  	/* Clear ICF flag */
		TCCR1B = 0x81;  	/* Falling edge, no prescaler ,noise canceler*/
		while ((TIFR&(1<<ICF1)) == 0);
		b = ICR1;  		/* Take value of capture register */
		TIFR = (1<<ICF1);  	/* Clear ICF flag */
		TCNT1=0;
		TCCR1B = 0;  		/* Stop the timer */
		high=b-a;
		distance=((high*34600)/(F_CPU*2)) ;
		if (distance>=80)
		{
			LCD_clear_screen();
			LCD_send_string("no object");
			_delay_ms(500);
		}
		else
		{
			LCD_clear_screen();
			LCD_send_string("distance=");
			LCD_send_data((distance/10)+48);
			LCD_send_data((distance%10)+48);
			LCD_send_string("cm");
			_delay_ms(500);
		}
	}
}
 


	
