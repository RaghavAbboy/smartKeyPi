#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>

int main ()
{
	int fd,i=0;
	fd= serialOpen("/dev/ttyAMA0",115200);
	if(fd < 0)  { printf("Opening serial failed.\n"); return 0; }

	while(i<10)
	{
		delay(500);
		serialPutchar(fd,i);
		i++;

		//printf("serialDataAvail: %d\n", serialDataAvail(fd));

		if(serialDataAvail(fd) >= 1)
		{
       			printf ("->%d\n", serialGetchar(fd));
       			fflush (stdout);
		}
	}

	printf("Serial port closing.\n");
	serialClose(fd);
	return 0;
}



