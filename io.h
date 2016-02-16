#ifndef _IO_BODY_
#define _IO_BODY_

#define VIB 0
#define BUTTON 1

#define LOW 0
#define HIGH 1

typedef struct shared_variable{
	int b;
//Key Debouncing Helper
	int pressed;
	
//helpers to enable normal operation
	int state;
	int currentDigit;
	int authenticatedDigits;
	int index;	
	int grantAccess;	

	int password;
	int length;
	int digits[100];	
	
	int attempts;
} sharedvariable;

sharedvariable gv;
sharedvariable* sv = &gv;

void pressed();
void vibrate(int, int);
void authenticateWithSmartKey();

//Initialization functions
void init_sharedvariable();
void init_setDigits(int password, int digits);

#endif
