#include <stdlib.h>
#include <stdio.h>
#include <stdio_ext.h> // fpurge header
#include <string.h>
#include <math.h>
#include <stdarg.h>
#include <ctype.h>
#include "inputreads.h"
#include "dboperations.h"
#include "accstruct.h"

#define LONG_BUFFER 21
#define LONG_LIMIT 19

void readString(char string[])
{
	char buffer[STRING_LIMIT + 2];
	int check;
	size_t newSize;
	
	do
	{
		check = 0;
		newSize = 0;
		__fpurge(stdin);
		scanf("%22s", buffer);
		
		for(int i = 0; ; i++)
		{	
			if(newSize > STRING_LIMIT)
			{
				printf("Invalid input. ONLY UP TO 20 letters allowed.\nType again: ");
				check = 1;
				break;
			}
			if(buffer[i] == '\0')
				break;
			newSize++;
		}
		if(newSize > STRING_LIMIT) continue;

		for(int i = 0; i < newSize; i++)
		{
			if(!(isalpha(buffer[i])))
			{
				printf("Invalid input. ONLY UP TO 20 letters allowed.\nType again: ");
				check = 1;
				break;
			}
		}
	}while(check);

	strcpy(string, buffer);
}

void readMoney(unsigned long long* balance)
{
	char balBuffer[LONG_BUFFER];
	size_t size;
	int check;
	int dot = 0;
	int dotAt = 0;

	do
	{
		check = 0;
		size = 0;
		__fpurge(stdin);
		scanf("%21s", balBuffer);
		for(int i = 0; ; i++)
		{
			if(size > LONG_LIMIT)
			{
				printf("Invalid input. Number out of bound.\n");
				check = 1;
				break;
			}
			if(balBuffer[i] == '\0')
				break;
			size++;
		}
		if(check) continue;

		dot = 0;
		dotAt = 0;
		for(int i = 0; i < size; i++)
		{	
			if(balBuffer[i] == '.' && !(dot)) 
			{
				dot = 1;
				dotAt = i;
				continue;
			}
			else if(balBuffer[i] == '.' && dot)
			{
				printf("Wrong input. No more than one dot allowed in number.\n");
				check = 1;
				break;
			}

			if(!(isdigit(balBuffer[i])))
			{
				printf("Wrong input. Only numbers are allowed.\n");
				check = 1;
				break;
			}
		}
	}while(check);

	int dottedOffset = 0;
	if(dot)
	{
		while(balBuffer[dotAt + 1] != '\0' && dottedOffset < 2)
		{
			balBuffer[dotAt] = balBuffer[dotAt + 1];
			dotAt++;
			dottedOffset++;
			balBuffer[dotAt] = '.';
		}
		if(dottedOffset == 0)
			*balance = strtol(balBuffer, NULL, 10) * 100;
		else if(dottedOffset == 1)
			*balance = strtol(balBuffer, NULL, 10) * 10;
		else
			*balance = strtol(balBuffer, NULL, 10);
	}
	else
	{
		*balance = strtol(balBuffer, NULL, 10) * 100;
	}
}

void readNumber(unsigned long long* number)
{
	unsigned long long buffer;
	do
	{
		__fpurge(stdin);
		if(scanf("%llu", &buffer)) 
			break;
		else
			printf("Invalid input. Enter positive number.\nType again: ");
	}while(1);

	*number = buffer;
}

void readPesel(unsigned long long* pesel)
{
	unsigned long long buffer;
	int check;

	do
	{
		check = 0;
		readNumber(&buffer);
		int digit = (int)(buffer / 10000000000);
		if(digit > 0 && digit < 10)
			break;
		else
		{
			printf("Invalid number format. PESEL is a number consisting of 11 digits only.\nType again: ");
			check = 1;
		}
	}while(check);

	*pesel = buffer;
}

void readAddress(char address[])
{
	char buffer[2 * STRING_LIMIT + 1];
	int check;
	size_t size;

	do
	{
		check = 0;
		size = 0;
		__fpurge(stdin);
		fgets(buffer, 2 * STRING_LIMIT + 1, stdin);

		for(int i = 0; ; i++)
		{	
			if(size > (2 * STRING_LIMIT))
			{
				printf("Invalid input. ONLY UP TO 40 signs allowed.\nType again: ");
				check = 1;
				break;
			}
			if(buffer[i] == '\0')
				break;
			size++;
		}
		if(size > (2 * STRING_LIMIT)) continue;

		/* trimming leading whitespaces  */
		int i = 0;
		int spaces = 0;
		while(isspace(buffer[i]))
		{
			i++;
			spaces++;
		}
		i = size - spaces - 1;
		char bufferTrimmed[2 * STRING_LIMIT + 1];
		strcpy(bufferTrimmed, buffer);
		strncpy(buffer, &bufferTrimmed[spaces], i);
		buffer[i] = '\0';
		
		int number = 1;
		int numberChecked = 0;
		while(i > 0)
		{
			i--;

			if(number && buffer[i] == ' ' && numberChecked) number = 0;

			if(!(isdigit(buffer[i])) && (number))
			{
				printf("Invalid NUMBER input. ONLY digits allowed.\nType again: ");
				check = 1;
				break;
			}
			else if(isdigit(buffer[i]) && (number)) numberChecked = 1;

			if((!(isalpha(buffer[i])) && (number == 0)) || ((i == 0) && (number)))
			{
				if(buffer[i] == ' ') continue;
				printf("Invalid STREET input. ONLY letters allowed.\nType again: ");
				check = 1;
				break;
			}
		}
	}while(check);

	strcpy(address, buffer);
}

int moneyIO(char operation, unsigned long long accNumber, unsigned long long amount)
{
	account current;
	FILE *data = fopen("data.dat", "rb+");
	int offset = (fsearch(data, accNumber) - 1) * sizeof(account);

	fseek(data, offset, 0);
	fread(&current, sizeof(account), 1, data);
	
	if(operation == 'd')
	{
		current.balance += amount;
	}
	else if(operation == 'w')
	{
		if(current.balance >= amount)
		{
			current.balance -= amount;
		}
		else
		{
			printf("Operation forbidden. Not enough money on account.\n");
			return 0;
		}
	}

	fseek(data, offset, 0);
	fwrite(&current, sizeof(account), 1, data);

	fclose(data);
	return 1;
}

char* balanceToStr(char* string, unsigned long long number)
{
	sprintf(string, "%llu.%llu", number / 100, number % 100);
	return string;
}
