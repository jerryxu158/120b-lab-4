/*	Author: Jerry Xu
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 4  Exercise 2
 *	Exercise Description: Buttons are connected to PA0 and PA1. Output for PORTC is initially 7. 
 *	Pressing PA0 increments PORTC once (stopping at 9). Pressing PA1 decrements PORTC once (stopping at 0). 
 *	If both buttons are depressed (even if not initially simultaneously), PORTC resets to 0.
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {Start, Inc, Dec, Reset, Standby} state;
	unsigned char tempA = 0x00; 
	unsigned char tempC = 0x00;
void tick(){
	switch(state){
		case Start:
			state = Standby;
			tempC = 0x07;
			break;
		case Standby:
			if(tempA == 0x00){state = Standby;}
			else if(tempA == 0x01){
				if(tempC < 9){
					tempC++;
				}
				 state = Inc;
			}
			else if (tempA == 0x02) {
				if(tempC > 0){
					tempC--;
				}			
				state = Dec;
			}
			else if(tempA == 0x03){tempC = 0; state = Reset;}
			break;
		case Inc:
			if(tempA == 0x00){state = Standby;}
			else if(tempA == 0x01) {state = Inc;}
			else if (tempA == 0x02) {
				if(tempC > 0){
					tempC--;
				}			
				state = Dec;
			}
			else if(tempA == 0x03) {tempC = 0; state = Reset;}
			break;
		case Dec:
			if(tempA == 0x00){state = Standby;}
			else if(tempA == 0x01){
				if(tempC < 9){
					tempC++;
				}
				 state = Inc;
			}
			else if(tempA == 0x02) {state = Dec;}

			else if(tempA == 0x03) {tempC = 0; state = Reset;}
			break;
		case Reset:
			if(tempA == 0x03){state = Reset;}
			else if (tempA == 0x02) {
				if(tempC > 0){
					tempC--;
				}			
				state = Dec;
			}
			else if(tempA == 0x01){
				if(tempC < 9){
					tempC++;
				}
				 state = Inc;
			}
			else if(!tempA){state = Standby;}
			break;
	}
	switch(state){//no actions in states, only on transitions
		default: break;
	}
}		
int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x00; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	state = Start;
    while (1) {
	PORTA = PINA;
	tempA = PORTA;

	tick();
	PORTC = tempC;
    }
    return 1;
}
