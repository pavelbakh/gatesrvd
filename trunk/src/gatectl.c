//============================================================================
// Name        : gatectl.c
// Author      : Pavel Bakhmetiev
// Version     : 1.0
// Copyright   : Sportplex 2010 Copyright
// Description : GATE server program in C, Ansi-style
//============================================================================

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "adrport.h"
//#include "loger.h"

int init_gatectl(int nCom)
{
		char sCom[3];
		char sCmd[254];
		char sResult[254];
		char sMess[254];
		int iSpot;
	// Open port for control devices
		sprintf(sCom, "%d", nCom);
		if (OpenAdrPort(sCom) < 0)
		{
//			log_message(LOGFILE, "ERROR open com port for control devices");
			return -1;
		}
		strcpy(sCmd, "$KE,RST");
		iSpot = strlen(sCmd);
		sCmd[iSpot] = 0x0d; // stick a <CR> after the command
		sCmd[iSpot+1] = 0x0a; // stick a <LF> after the command
		sCmd[iSpot+2] = 0x00; // terminate the string properly
		if (WriteAdrPort(sCmd) < 0)
		{
//			log_message(LOGFILE, "ERROR write command to com port");
			return -1;
		}
		sleep(1); // give the ADR card some time to respond
		if (ReadAdrPort(sResult,254) > 0)
		{
			sprintf(sMess, "****Response is %s\n", sResult);
//			log_message(LOGFILE, sMess);
		} // end if

//		strcpy(sCmd, "$KE,WRA,111111111111111111111111");
		strcpy(sCmd, "$KE,WRA,000000000111100000000000");
		iSpot = strlen(sCmd);
		sCmd[iSpot] = 0x0d; // stick a <CR> after the command
		sCmd[iSpot+1] = 0x0a; // stick a <LF> after the command
		sCmd[iSpot+2] = 0x00; // terminate the string properly
		if (WriteAdrPort(sCmd) < 0)
		{
//			log_message(LOGFILE, "ERROR write command to com port");
			return -1;
		}

		return 0;
}

int open_gate(int nGate)
{
	char sCmd[254];
	int iSpot;

//	printf("Begin open gate...");
	sprintf(sCmd, "$KE,WR,%u,1", nGate);
	iSpot = strlen(sCmd);
	sCmd[iSpot] = 0x0d; // stick a <CR> after the command
	sCmd[iSpot+1] = 0x0a; // stick a <LF> after the command
	sCmd[iSpot+2] = 0x00; // terminate the string properly
	if (WriteAdrPort(sCmd) < 0)
	{
//		log_message(LOGFILE, "ERROR write command to com port");
		return -1;
	}

	sprintf(sCmd, "$KE,WR,%u,0", nGate);
	iSpot = strlen(sCmd);
	sCmd[iSpot] = 0x0d; // stick a <CR> after the command
	sCmd[iSpot+1] = 0x0a; // stick a <LF> after the command
	sCmd[iSpot+2] = 0x00; // terminate the string properly
	if (WriteAdrPort(sCmd) < 0)
	{
//		log_message(LOGFILE, "ERROR write command to com port");
		return -1;
	}

	sleep(1); // pause

	sprintf(sCmd, "$KE,WR,%u,1", nGate);
	iSpot = strlen(sCmd);
	sCmd[iSpot] = 0x0d; // stick a <CR> after the command
	sCmd[iSpot+1] = 0x0a; // stick a <LF> after the command
	sCmd[iSpot+2] = 0x00; // terminate the string properly
	if (WriteAdrPort(sCmd) < 0)
	{
//		log_message(LOGFILE, "ERROR write command to com port");
		return -1;
	}

//	printf("End open gate...\n");

	return 0;
}

int close_gatectl()
{
	char sCmd[254];
	int iSpot;

	strcpy(sCmd, "$KE,WRA,000000000000000000000000");
	iSpot = strlen(sCmd);
	sCmd[iSpot] = 0x0d; // stick a <CR> after the command
	sCmd[iSpot+1] = 0x0a; // stick a <LF> after the command
	sCmd[iSpot+2] = 0x00; // terminate the string properly
	if (WriteAdrPort(sCmd) < 0)
	{
//		log_message(LOGFILE, "ERROR write command to com port");
		return -1;
	}
	// Close com port
		CloseAdrPort();

		return 0;
}
