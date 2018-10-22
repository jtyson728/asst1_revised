#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#include "scannerCSVsorter.h"

#ifndef MERGESORT_H
#define MERGESORT_H

void mergeSort(line**);
line* merge(line*, line*);
void split(line*, line**, line**);
int compareLines(char*, char*);
void printList(line*);

#endif
