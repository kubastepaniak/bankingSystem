#include <stdlib.h>
#include <stdio.h>
#include <stdio_ext.h> // fpurge header
#include <string.h>
#include <math.h>
#include <stdarg.h>
#include <ctype.h>
#include "dboperations.h"

int numberOfClients;

int main(int argc, char const *argv[])
{
	numberOfClients = getNumberOfClients();
	while(systemInteface());
	return 0;
}