//============================================================================
// Name        : gatesrv.c
// Author      : Pavel Bakhmetiev
// Version     : 1.0
// Copyright   : Sportplex 2010 Copyright
// Description : GATE server program in C, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void log_message(char *filename, char *message)
{
 	time_t t;
//	FILE *logfile;
//	logfile = fopen(filename, "a");
//	if(!logfile)
//		return;
	/* Timestamp the event as we get notified */
	t = time(NULL);

	char sMess[254];
	sprintf(sMess, "%s %s\n", ctime(&t), message);
	printf(sMess);
//	fprintf(logfile, "%s %s\n", ctime(&t), message);
//	fclose(logfile);
}
