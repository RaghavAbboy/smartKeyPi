#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <stdint.h>
#include <math.h>
#include "io.h"

int main()
{
	int i;
	init_sharedvariable(sv);
	
	//remove	
	printf("Main function begins.\n");
	if(wiringPiSetup() == -1) { printf("error\n"); return 1; }

	pinMode(VIB,OUTPUT);
	
	//wiringPiISR(BUTTON, INT_EDGE_BOTH, &pressed);
	wiringPiISR(BUTTON, INT_EDGE_FALLING, &authenticateDigit);
		
	while(1){
		//vibrate(500, 300);
	}

	return 0;
}

//ISR to detect Smart Key button press
void pressed()
{
	if(sv->pressed == 0)
	{
		if(digitalRead(BUTTON) == LOW)
		{
			delay(20);
			if(digitalRead(BUTTON) == LOW) {
				printf("Button Pressed\n");
				sv->pressed = 1;
			}
		}
	}
	else
	{
		if(digitalRead(BUTTON) == HIGH)
		{
			delay(20);
			if(digitalRead(BUTTON) == HIGH) {
				printf("Button Released\n");
				sv->pressed = 0;
			}
		}
	}
}

//-----------------------------------------------------------
//ISR to attempt authentication
void authenticateDigit()
{
	//if(sv->pressed == 0)
	//{

	//remove
	sv->currentDigit = sv->digits[2];
	
	int digit = sv->currentDigit;
	printf("Digit to authenticate: %d\n", digit);
	int count = 0;

	if(digitalRead(BUTTON) == LOW)
	{
		delay(20);
		if(digitalRead(BUTTON) == LOW) {
			printf("Button Pressed\n");
			sv->pressed = 1;

			while(digitalRead(BUTTON) == LOW) {
				vibrate(500,300);
				count++;	
			}
			
			printf("Counts Held: %d\n", count);
			if(count == digit) { printf("Digit %d authenticated!\n");  return; }
			else { printf("Digit %d failed authentication.\n"); return; }
		}
	}

	//}
	/*
	else
	{
		if(digitalRead(BUTTON) == HIGH)
		{
			delay(20);
			if(digitalRead(BUTTON) == HIGH) {
				printf("Button Released\n");
				sv->pressed = 0;
			}
		}
	} */
}

//Vibrate the motor for onTime
void vibrate(int onTime, int offTime)
{
	printf("Vibrating for %d ms\n", onTime);
	digitalWrite(VIB,HIGH);
	delay(onTime);
	digitalWrite(VIB,LOW);
	delay(offTime);
}

//------------------------------------------------------------
//Initialization Functions
//Initialize the shared variable
void init_sharedvariable()
{
	sv->b=0;
	sv->pressed=0;
	
	sv->password = 123;
	sv->length = 3;
	init_setDigits(sv->password, sv->length);
}


//Set a password
int init_setPassword(int password) {
	//If password length is atleast 3, accept with a 1, else return -1
	if(password <= 99) { return -1; }
	
	sv->password = password;
	//Determine length
	int length = 0;
	while(password != 0) {
		length++;
		password/=10;
	}	
	sv->length = length;
	init_setDigits(sv->password, sv->length);
	printf("Password set: %d, length: %d\n", sv->password, sv->length);
	return 1;
}


//Set the digits array with the password digits
void init_setDigits(int password, int length) {
	int i;
	int power = length - 1;
	
	//remove
	printf("init_setDigits says: Password: %d, length: %d, power: %d\n", password, length, power);
	
	for(i=0; i<length; i++) {
		int digit = (password/(int)pow(10,power))%10;
		printf("Digit:%d\n", digit);
		sv->digits[i] = digit;
		power--;		
	}
}

