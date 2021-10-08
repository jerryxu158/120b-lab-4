/*	Author: Jerry Xu
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 4  Exercise 4
 *	Exercise Description: A household has a digital combination deadbolt lock system on the doorway. 
 *	The system has buttons on a keypad. Button 'X' connects to PA0, 'Y' to PA1, and '#' to PA2. 
 *	Pressing and releasing '#', then pressing 'Y', should unlock the door by setting PB0 to 1. Any other sequence fails to unlock.
 *	Pressing a button from inside the house (PA7) locks the door (PB0=0). 
 *	For debugging purposes, give each state a number, and always write the current state to PORTC (consider using the enum state variable). 
 *	Also, be sure to check that only one button is pressed at a time. 
 *	Extend the above door so that it can also be locked by entering the earlier code.
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {Start, standby, rightHash, rightStandby, rightAccept, lockInside, rightHashLock, rightLockStandby, rightAcceptLock} state;
	unsigned char tempA = 0x00; 
	unsigned char tempB = 0x00;
void tick(){
	switch(state){
		case Start:
			state = standby;
			tempB = 0x00;
			break;
		case standby:
			if(tempA >> 7 & 0x01){
				state = lockInside;
				tempB = 0;
			}
			else if(tempA == 0x04){state = rightHash;}
			else {
				state = standby;
			}
			break;
		case rightHash:
			if(tempA >> 7 & 0x01){
				state = lockInside;
				tempB = 0;
			}
			else if(!tempA){
				state = rightStandby;
			}
			else if(tempA == 0x04){state = rightHash;}
			else {state = standby;}
			break;
		case rightStandby:
			if(tempA >> 7 & 0x01){
				state = lockInside;
				tempB = 0;
			}
			else if(tempA == 0x02){
				tempB = 0x01;
				state = rightAccept;
			}
			else if(!tempA){state = rightStandby;}
			else {state = standby;}
			break;
		case rightAccept:
			if(tempA >> 7 & 0x01){
				state = lockInside;
				tempB = 0;
			}
			else if(tempA == 0x04){state = rightHashLock;}
			else {state = rightAccept;}
			break;
		case rightHashLock:
			if(tempA >> 7 & 0x01){
				state = lockInside;
				tempB = 0;
			}
			else if (tempA == 0x04){state = rightHashLock;}
			else if(!tempA){state = rightLockStandby;}
			else {state = standby;}
			break;
		case rightLockStandby:
			if(tempA >> 7 & 0x01){
				state = lockInside;
				tempB = 0;
			}
			else if(!tempA){state = rightLockStandby;}
			else if(tempA == 0x02){
				tempB = 0x00;
				state = rightAcceptLock;
			}
			else {state = standby;}
			break;
		case rightAcceptLock:
			if(tempA >> 7 & 0x01){
				state = lockInside;
				tempB = 0;
			}
			else if(tempA == 0x04){state = rightHash;}
			else {state = rightAcceptLock;}
			break;
		case lockInside:
			if(!(tempA >> 7 & 0x01)){
				state = standby;
			}
			else{
				state = lockInside;
			}
			break;
		
	}
	switch(state){
		case rightAcceptLock:
			tempB = 0x00;
		default: break;
	}
}		
int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFB; PORTB = 0x00;
	state = Start;
    while (1) {
	PORTA = PINA;
	tempA = PORTA;

	tick();
	PORTB = tempB;
    }
    return 1;
}
