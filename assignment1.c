#include "assignment1.h"
#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <stdint.h>

void init_shared_variable(SharedVariable* sv) {
	// You can initialize the shared variable if needed.
	sv->bProgramExit = 0;
	sv->state = 0; //Initial State
	
	sv->buttonState = 1; //Button Status
	sv->tempState = 0;  //Dig. Temperature Sensor Status
	sv->buzzerState = 0; //Buzzer Status
	sv->aledState = 0;  //ALED Status
	sv->trackState = 1; //Tracking sensor status
	sv->magenta = 0; //Disable magenta initially
	sv->red = 0; //Disable red initially
	sv->warning = 0; //Disable warning initially
}

void init_sensors(SharedVariable* sv) {
  pinMode(PIN_BUTTON, INPUT);
  pinMode(PIN_TEMP, INPUT);
  pinMode(PIN_TRACK, INPUT);
  pinMode(PIN_SHOCK, INPUT);  

  pinMode(PIN_YELLOW, OUTPUT);
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  pinMode(PIN_ALED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  softPwmCreate(7, 0, 255);
  softPwmCreate(8, 0, 255);
  softPwmCreate(9, 0, 255);
}

void body_button(SharedVariable* sv) {
//sv->bProgramExit = 1;
int buttonValue = digitalRead(PIN_BUTTON);

//Update the state if necessary
if(buttonValue != (sv->buttonState)) {
//remove
printf("State Updated. Current state:");

//wait for the user to release
while(buttonValue != sv->buttonState) {
buttonValue = digitalRead(PIN_BUTTON);
}
sv->state = !sv->state;

//remove
if(sv->state) { printf("DRIVING\n"); }
else { 
	printf("INITIAL\n");
	sv->trackState = 1;
	sv->shockState = 0;
	sv->warning = 0;
	}
}

}

void body_twocolor(SharedVariable* sv) {
if(sv->state) { digitalWrite(PIN_YELLOW, HIGH); }
else { digitalWrite(PIN_YELLOW, LOW); }
}

void body_temp(SharedVariable* sv) {
//Check Driving state
if(sv->state) {
	int tempValue = digitalRead(PIN_TEMP);
	if(tempValue == HIGH) {
		//Set off Buzzer and ALED
		sv->buzzerState = 1;
		sv->aledState = 1;
	}
	else {
		//Turn them off
		sv->buzzerState = 0;
		sv->aledState = 0;
	}
}//end if for state

}

void body_track(SharedVariable* sv) {
int trackValue = digitalRead(PIN_TRACK);
if(sv->state && trackValue == LOW && sv->trackState && !sv->warning) {
//Trigger Magenta
printf("Tracked!\n");
sv->trackState = 0;
sv->magenta = 1;
sv->warning = 1;
}

}

void body_shock(SharedVariable* sv) {
int shockValue = digitalRead(PIN_SHOCK);
if(sv->state && shockValue == HIGH && sv->shockState == 0 && !sv->warning) {
printf("Shocked!\n");
sv->shockState = 1;
sv->red = 1;
sv->warning = 1;
}

}

void body_rgbcolor(SharedVariable* sv) {
//Initial - Blue (0,0,1)
if(sv->state == 0) {
sv->magenta = 0;
sv->red = 0;
softPwmWrite(PIN_RED, 0);
softPwmWrite(PIN_GREEN, 0);
softPwmWrite(PIN_BLUE, 255);
}

else {
//Driving - Green (0,1,0)
if(sv->magenta) {
	//printf("Magenta\n");
	softPwmWrite(PIN_RED, 118);
	softPwmWrite(PIN_GREEN, 0);
	softPwmWrite(PIN_BLUE, 238);
	}
else if(sv->red) {
	//printf("Red\n");
	softPwmWrite(PIN_RED, 255);
        softPwmWrite(PIN_GREEN, 0);
        softPwmWrite(PIN_BLUE, 0);
	}
else {
	//printf("Green\n");
	softPwmWrite(PIN_RED, 0);
	softPwmWrite(PIN_GREEN, 255);
	softPwmWrite(PIN_BLUE, 0);
	}
}

}

void body_aled(SharedVariable* sv) {
if(sv->aledState && sv->state) { digitalWrite(PIN_ALED, HIGH); }
else { digitalWrite(PIN_ALED, LOW); }
}

void body_buzzer(SharedVariable* sv) {
if(sv->buzzerState && sv->state) { digitalWrite(PIN_BUZZER, HIGH); }
else { digitalWrite(PIN_BUZZER, LOW); }
}

