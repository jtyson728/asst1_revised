#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <sys/wait.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>

#ifndef SCANNERCSVSORTER_H
#define SCANNERCSVSORTER_H

typedef struct rowLine{
	char* wholeLine;
	char* compareCat;
	struct rowLine *next;
}line;

int commaCounter(char*, char*, int);
void sorter(char*, char*, char*, char*,char*,int);



#endif
