/*	Author: Jerry Xu
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 4  Exercise 1
 *	Exercise Description: [PB0 and PB1 each connect to an LED, and PB0's LED is initially on. 
 *	Pressing a button connected to PA0 turns off PB0's LED and turns on PB1's LED, staying that way after button release.
 *	Pressing the button again turns off PB1's LED and turns on PB0's LED. 
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {Start, On1, On2, Off1, Off2} state;
	unsigned char tempA = 0x00; 
	unsigned char tempB = 0x00;
void tick(){
	switch(state){
		case Start:
			state = On1;
			tempB = 0x01;
			break;
		case On1:
			if(tempA){tempB = 0x02; state = Off1;}
			else {state = On1;}
			break;
		case Off1:
			if(tempA){state = Off1;}
			else {state = Off2;}
			break;
		case Off2:
			if(tempA){tempB = 0x01; state = On2;}
			else{state = Off2;}
			break;
		case On2:
			if(!tempA){state = On1;}
			else {state = On2;}
			break;
	}
	switch(state){//no actions in states, only on transitions
		default: break;
	}
}		
int main(void) {
	DDRA = 0x00; PORTA = 0x00;
	DDRB = 0x00; PORTB = 0x00;
	state = Start;
    while (1) {
	PORTA = PINA;
	tempA = PORTA;
	PORTB = PINB;

	tick();
	PORTB = tempB;
    }
    return 1;
}
