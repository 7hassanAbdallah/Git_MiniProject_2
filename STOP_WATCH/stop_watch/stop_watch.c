/*
 * stop_watch.c
 *
 *  Created on: Apr 10, 2023
 *      Author: Hassan
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned char i[7]={0};


void Display_7segmant(unsigned char x,unsigned char y);
void TIMER_1(void);
void INT_0(void);
void INT_1(void);
void INT_2(void);



int main(){
	DDRC|=0x0f;
	DDRA = 0x3f;
	DDRD &=~ (0x0c);

	INT_0();
	INT_1();
	INT_2();
	TIMER_1();

	for(;;){
		if (i[0]>5){i[0]=1;}
		else i[0]++;
		Display_7segmant(i[0],i[i[0]]);
		_delay_ms(2);
	}
}



void Display_7segmant(unsigned char x,unsigned char y){
	PORTA &=~ (0x3f);
	PORTA |= (1<<(x-1));
	PORTC &=~ (0x0f);
	PORTC |= (y)&(0x0f);


}


void TIMER_1(void){
	SREG |= (1<<7);
	TCNT1 = 0;
	OCR1A = 977;
	ICR1=978;
	TIMSK |= (1<<OCIE1A);
	TCCR1A = (1<<FOC1A);
	TCCR1B = (1<<WGM12) | (1<<WGM13) | (1<<CS10) | (1<<CS12);
}




ISR(TIMER1_COMPA_vect){
	i[6]++;

	if (i[6]>9){
			i[6]=0;i[5]++;
		}
	if (i[5]>5){
			i[5]=0;i[4]++;
		}
	if (i[4]>9){
				i[4]=0;i[3]++;
			}
	if (i[3]>5){
				i[3]=0;i[2]++;
			}
	if (i[2]>9){
				i[2]=0;i[1]++;
			}
	if((i[1]>=2)&(i[2]>=4)){
		i[1]=0;i[2]=0;
	}
}

void INT_0(void){
	SREG |= (1<<7);
	DDRD &=~ (1<<2);
	GICR |=(1<<INT0);
	MCUCR |=(1<<ISC01) ;
	MCUCR &=~ (1<<ISC00);
}

ISR(INT0_vect){
	unsigned char j;
	for(j=0;j<7;j++){
		i[j]=0;
	}
}

void INT_1(void){
	SREG |= (1<<7);
	DDRD &=~ (1<<3);
	GICR |=(1<<INT1);
	MCUCR |=(1<<ISC11)|(1<<ISC10);

}

ISR(INT1_vect){
	TIMSK &=~ (1<<OCIE1A);
}


void INT_2(void)
{
	SREG   |= (1<<7);
	DDRB   &= (~(1<<PB2));
	GICR   |= (1<<INT2);
	MCUCSR &=~ (1<<ISC2);
}

ISR(INT2_vect){
	TIMSK |= (1<<OCIE1A);
}
