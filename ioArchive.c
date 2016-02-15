#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <stdint.h>

#define pin 0
#define pin_button 1
#define LOW 0
#define HIGH 1

int b,c=1;

void pressed();
void cell_vibration_motor();

int main()
{
	int i;
	if(wiringPiSetup() == -1) { printf("error\n"); return 1; }

	pinMode(pin,OUTPUT);
	//b=0;
	b=wiringPiISR(pin_button,INT_EDGE_BOTH,&pressed);
	//for(i=0;i<8;i++) {
	while(1){
		cell_vibration_motor();	
	}
	
	return 0;	
}
void pressed()
{
	int check;
	if(c==1)
	{
	//	delay(5);
		if(digitalRead(pin_button)==LOW)
		{
			printf("pressed\n");
			c=0;
		}
	}
	else
	{
	//	delay(5);
		if(digitalRead(pin_button)==HIGH)
		{
			printf("released\n");
			c=1;
		}
	}
}

void cell_vibration_motor()
{
	printf("hi\n");
	digitalWrite(pin,HIGH);
	delay(150);
	digitalWrite(pin,LOW);
	delay(200);
}


