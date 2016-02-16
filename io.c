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
	wiringPiISR(BUTTON, INT_EDGE_FALLING, &authenticateWithSmartKey);
		
	while(1){
		//vibrate(500, 300);
		if(sv->state == 0) {
			sv->index = 0;
			sv->authenticatedDigits = 0;
		}
		
	}

	return 0;
}

//-----------------------------------------------------------
//ISR to attempt authentication
void authenticateWithSmartKey()
{
	//Base Cases
	//If free set the state
	if(sv->state == 0) { sv->state = 1; }
	if(sv->grantAccess == 1) { return; }
	
	int digit = sv->digits[sv->index];
	printf("Digit to authenticate: %d\n", digit);
	
	//Register number of counts from the user
	int count = 0;
	if(digitalRead(BUTTON) == LOW)
	{
		delay(20);
		if(digitalRead(BUTTON) == LOW) {
			printf("Button Pressed\n");

			while(digitalRead(BUTTON) == LOW) {
				vibrate(500,300);
			 	count++;	
			}
			
			printf("Counts Held: %d\n", count);
			if(count == digit) {
				printf("Digit %d authenticated!\n", digit);  
				sv->authenticatedDigits++;
				sv->index++;
			}
			else {
				//TODO: Track number of attempts
				printf("Digit %d failed authentication.\n");
				sv->authenticatedDigits = 0;
				return;
			}
		}
	}
	//End of count registration from the user

	if(sv->authenticatedDigits == sv->length) {
		sv->state = 0;
		printf("*********Welcome Home!!!********\n");
		printf("Access Granted.\n");
		sv->grantAccess = 1;
		delay(5000);
		printf("Access Locked.\n");
		sv->grantAccess = 0;
	}
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

	sv->authenticatedDigits = 0;
	sv->index = 0;

	sv->grantAccess = 0;	
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

