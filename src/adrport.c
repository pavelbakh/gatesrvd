//============================================================================
// Name        : gatesrv.c
// Author      : Pavel Bakhmetiev
// Version     : 1.0
// Copyright   : Sportplex 2010 Copyright
// Description : GATE server program in C, Ansi-style
//============================================================================

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <errno.h>
#include "adrport.h"
#include "loger.h"
static int fd = 0;

// opens the serial port
// return code:
//   > 0 = fd for the port
//   -1 = open failed
int OpenAdrPort(char* sPortNumber)
{
	char sMessage[254];
    char sPortName[64];
    sprintf(sPortName, "/dev/ttyACM%s", sPortNumber);
    sprintf(sMessage, "sPortName=%s", sPortName);
	log_message(LOGFILE, sMessage);

    // make sure port is closed
    CloseAdrPort(fd);

    fd = open(sPortName, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd < 0)
    {
        sprintf(sMessage, "open error %d %s", errno, strerror(errno));
		log_message(LOGFILE, sMessage);
    }
    else
    {
        struct termios my_termios;
        tcgetattr(fd, &my_termios);

        tcflush(fd, TCIFLUSH);

        my_termios.c_cflag = B9600 | CS8 |CREAD | CLOCAL | HUPCL;

        cfsetospeed(&my_termios, B9600);
        tcsetattr(fd, TCSANOW, &my_termios);

    } // end if
    return fd;
} // end OpenAdrPort

// writes zero terminated string to the serial port
// return code:
//   >= 0 = number of characters written
//   -1 = write failed
int WriteAdrPort(char* psOutput)
{
    int iOut;
	char sMessage[254];
    if (fd < 1)
    {
		log_message(LOGFILE, " port is not open");
        return -1;
    } // end if
    iOut = write(fd, psOutput, strlen(psOutput));
    if (iOut < 0)
    {
        sprintf(sMessage, "write error %d %s", errno, strerror(errno));
		log_message(LOGFILE, sMessage);
   }
 //   else
 //   {
 //       sprintf(sMessage, "wrote %d chars: %s", iOut, psOutput);
//		log_message(LOGFILE, sMessage);
//    } // end if
    return iOut;
} // end WriteAdrPort

// read string from the serial port
// return code:
//   >= 0 = number of characters read
//   -1 = read failed
int ReadAdrPort(char* psResponse, int iMax)
{
    int iIn;
	char sMessage[254];

    if (fd < 1)
    {
    	log_message(LOGFILE, " port is not open");
        return -1;
    } // end if

    iIn = read(fd, psResponse, iMax-1);
   if (iIn < 0)
    {
    	if (errno == EAGAIN)
    		return 0; // assume that command generated no response
		else
		{
		    sprintf(sMessage, "read error %d %s", errno, strerror(errno));
			log_message(LOGFILE, sMessage);
		} // end if
    }
    else
     	psResponse[iIn<iMax?iIn:iMax] = '\0';

    return iIn;
} // end ReadAdrPort

// closes the serial port
void CloseAdrPort()
{
	// you may want to restore the saved port attributes
    if (fd > 0)
        close(fd);

} // end CloseAdrPort
