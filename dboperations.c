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

#define accNumberOffset 1000
#define STRING_LIMIT 20
#define LONG_BUFFER 21
#define LONG_LIMIT 19

int getNumberOfClients()
{
	FILE *data = fopen("data.dat", "rb");
	account current;
	int number = 0;

	while(fread(&current, sizeof(account), 1, data) == 1) number++;
	
	fclose(data);
	return number;
}

int systemInteface()
{
	int operation;
	FILE *data;
	char interface[] = "\nBanking System Interface"
					   "\n1. Add Client"
					   "\n2. Show information about the Clients"
					   "\n3. Find client by data"
					   "\n4. Transfer money"
					   "\n5. Make withdrawal"
					   "\n6. Make deposit"
					   "\n9. ***Data Wipe***"
					   "\n0. Exit";
	printf("%s\n", interface);

	do
	{
		printf("Enter operation: ");
		if(scanf("%d", &operation)) break;
		else __fpurge(stdin);
	}while(1);

	switch(operation)
	{
		case 1:
			addNewClient();
			break;
		case 2:
			showInformationAll();
			break;
		case 3:
			searchBy();
			break;
		case 4:
			transfer();
			break;
		case 5:
			withdraw();
			break;
		case 6:
			deposit();
			break;
		case 9:
			data = fopen("data.dat", "wb");
			fclose(data);
			return 0;
		case 0:
			return 0;
		default:
			break;
	}
	return 1;
}

void addNewClient()
{
	FILE *data = fopen("data.dat", "ab");
	account newAccount;
	
	printf("\nEnter name: ");
	readString(newAccount.name);
	
	printf("Enter last name: ");
	readString(newAccount.lastName);

	printf("Enter PESEL number: ");
	readPesel(&newAccount.pesel);

	printf("Enter address in format \"STREET NUMBER\": ");
	readAddress(newAccount.address);

	newAccount.balance = 0;
	newAccount.accountNumber = accNumberOffset + (++numberOfClients);

	printf("Do you want to open an account?\n");
	if(confirm())
		fwrite(&newAccount, sizeof(account), 1, data);

	fclose(data);
}

void showInformationAll()
{
	FILE *data = fopen("data.dat", "rb");
	account current;

	while(fread(&current, sizeof(account), 1, data) == 1)
		showInformation(current);
	fclose(data);
}

void showInformation(account current)
{
	char balBuffer[LONG_BUFFER];
	printf("\nAccount Number: %ld", current.accountNumber);
	printf("\nName: %s", current.name);
	printf("\nLast name: %s", current.lastName);
	printf("\nPESEL: %ld", current.pesel);
	printf("\nAddress: %s", current.address);
	printf("\nBalance: %s\n", balanceToStr(balBuffer, current.balance));
}

void deposit()
{
	unsigned long amount;
	unsigned long accNumber;

	FILE *data = fopen("data.dat", "rb");
	while(1)
	{
		rewind(data);
		printf("Type account number: ");
		readNumber(&accNumber);
		if(fsearch(data, accNumber))
			break;
		else
			printf("Account not found.\n");
	}
	fclose(data);

	printf("How much do you want to deposit?: ");
	readMoney(&amount);

	if(confirm())
		moneyIO('d', accNumber, amount);
}

void withdraw()
{
	unsigned long amount;
	unsigned long accNumber;

	FILE *data = fopen("data.dat", "rb");
	while(1)
	{
		rewind(data);
		printf("Type account number: ");
		readNumber(&accNumber);
		if(fsearch(data, accNumber))
			break;
		else
			printf("Account not found.\n");
	}
	fclose(data);

	printf("How much do you want to withdraw?: ");
	readMoney(&amount);

	if(confirm())
		moneyIO('w', accNumber, amount);
}

void transfer()
{
	unsigned long amount;
	unsigned long accNumber1, accNumber2;

	FILE *data = fopen("data.dat", "rb");
	while(1)
	{
		rewind(data);
		printf("Type sender's account number: ");
		readNumber(&accNumber1);
		if(fsearch(data, accNumber1))
			break;
		else
			printf("Account not found.\n");
	}

	while(1)
	{
		rewind(data);
		printf("Type receiver's account number: ");
		readNumber(&accNumber2);
		if(accNumber1 == accNumber2)
		{
			printf("Cannot use same account twice.\n");
			continue;
		}
		if(fsearch(data, accNumber2))
			break;
		else
			printf("Account not found.\n");
	}
	fclose(data);

	printf("How much do you want to transfer?: ");
	readMoney(&amount);

	if(confirm())
	{
		if(moneyIO('w', accNumber1, amount))
			moneyIO('d', accNumber2, amount);
	}
}

int confirm()
{
	char confirmation;
	do
	{
		printf("Confirm? (y or n)\n");
		__fpurge(stdin);
		if(scanf("%c", &confirmation))
		{
			switch(confirmation)
			{
				case 'y':
					return 1;
				case 'n':
					return 0;
				default:
					printf("Answer with 'y' or 'n' only.\n");
					break;
			}
		}
	}while(1);
}

void searchBy()
{
	int choice;
	char prompt[] = "\nWhat do you want to look for?"
					"\n1. Name"
					"\n2. Last name"
					"\n3. Full name"
					"\n4. Account Number"
					"\n5. PESEL number"
					"\n6. Address"
					"\n7. Balance\n";
	printf("%s", prompt);

	while(1)
	{
		readNumber((unsigned long*)&choice);
		if(choice > 0 && choice < 8)
			break;
		else
			printf("Wrong choice. Choose number from 1 to 7.%s", prompt);
	}

	switch(choice)
	{
		case 1:
			searchingStrings("name");
			break;
		case 2:
			searchingStrings("lastname");
			break;
		case 3:
			searchingStrings("fullname");
			break;
		case 4:
			searchingNumbers("accountNumber");
			break;
		case 5:
			searchingNumbers("pesel");
			break;
		case 6:
			searchingStrings("address");
			break;
		case 7:
			searchingNumbers("balance");
			break;
	}
}

int searchingNumbers(char searchWhat[])
{
	unsigned long number;
	char balBuffer[LONG_BUFFER];
	account* currents;

	printf("Search for %s: \n", searchWhat);
	if(strcmp(searchWhat, "pesel") == 0)
		readPesel(&number);
	else if(strcmp(searchWhat, "balance") == 0)
		readMoney(&number);
	else
		readNumber(&number);

	currents = searchFor("number", searchWhat, number);
	if(currents == NULL)
	{
		if(strcmp(searchWhat, "balance") == 0)
			printf("No person with %s %s found.\n", searchWhat, balanceToStr(balBuffer, number));
		else
			printf("No person with %s %ld found.\n", searchWhat, number);
		free(currents);
		return 0;
	}
	else
	{
		for(int i = 1; i < currents[0].accountNumber; i++)
			showInformation(currents[i]);
		free(currents);
	}
	return 1;
}

int searchingStrings(char searchWhat[])
{
	char string[2 * STRING_LIMIT + 3];
	account* currents;

	printf("Search for %s: \n", searchWhat);
	if(strcmp(searchWhat, "address") == 0)
	{
		readAddress(string);
	}
	else if(strcmp(searchWhat, "fullname") == 0)
	{
		char lastnamestring[STRING_LIMIT + 1];

		printf("Name: ");
		readString(string);
		printf("Last name: ");
		readString(lastnamestring);

		strcat(string, " ");
		strcat(string, lastnamestring);
	}
	else
	{
		readString(string);
	}

	currents = searchFor("string", searchWhat, string);
	if(currents == NULL)
	{
		printf("No person with %s %s found.\n", searchWhat, string);
		free(currents);
		return 0;
	}
	else
	{
		for(int i = 1; i < currents[0].accountNumber; i++)
			showInformation(currents[i]);
		free(currents);
	}
	return 1;
}

account* searchFor(char type[], ...)
{
	FILE *data = fopen("data.dat", "rb");
	account current;
	account* currents = NULL;
	int size = 1;

	va_list ap;
	while(fread(&current, sizeof(account), 1, data) == 1)
	{
		va_start(ap, type);
		if(strcmp(type, "number") == 0)
		{
			char* searchType = va_arg(ap, char*);
			if(strcmp(searchType, "accountNumber") == 0)
			{
				if(current.accountNumber == va_arg(ap, int))
				{
					size++;
					currents = (account*)realloc(currents, sizeof(account)*size);
					currents[0].accountNumber = (unsigned long)size;
					currents[size - 1] = current;
				}
			}
			else if(strcmp(searchType, "pesel") == 0)
			{	
				if(current.pesel == va_arg(ap, int))
				{
					size++;
					currents = (account*)realloc(currents, sizeof(account)*size);
					currents[0].accountNumber = (unsigned long)size;
					currents[size - 1] = current;
				}
			}
			else if(strcmp(searchType, "balance") == 0)
			{
				if(current.balance == va_arg(ap, int))
				{
					size++;
					currents = (account*)realloc(currents, sizeof(account)*size);
					currents[0].accountNumber = (unsigned long)size;
					currents[size - 1] = current;
				}
			}
		}
		else if(strcmp(type, "string") == 0)
		{
			char* searchType = va_arg(ap, char*);
			if(strcmp(searchType, "name") == 0)
			{
				if(strcmp(current.name, va_arg(ap, char*)) == 0)
				{
					size++;
					currents = (account*)realloc(currents, sizeof(account)*size);
					currents[0].accountNumber = (unsigned long)size;
					currents[size - 1] = current;
				}
			}
			else if(strcmp(searchType, "lastname") == 0)
			{
				if(strcmp(current.lastName, va_arg(ap, char*)) == 0)
				{
					size++;
					currents = (account*)realloc(currents, sizeof(account)*size);
					currents[0].accountNumber = (unsigned long)size;
					currents[size - 1] = current;
				}
			}
			else if(strcmp(searchType, "fullname") == 0)
			{
				char fullname[2 * STRING_LIMIT + 3];
				strcpy(fullname, current.name) ;
				strcat(fullname, " ");
				strcat(fullname, current.lastName);
				if(strcmp(fullname, va_arg(ap, char*)) == 0)
				{
					size++;
					currents = (account*)realloc(currents, sizeof(account)*size);
					currents[0].accountNumber = (unsigned long)size;
					currents[size - 1] = current;
				}
			}
			else if(strcmp(searchType, "address") == 0)
			{
				if(strcmp(current.address, va_arg(ap, char*)) == 0)
				{
					size++;
					currents = (account*)realloc(currents, sizeof(account)*size);
					currents[0].accountNumber = (unsigned long)size;
					currents[size - 1] = current;
				}
			}
		}
		va_end(ap);
	}
	fclose(data);
	if(size == 0)
		return NULL;
	else
		return currents;
}

int fsearch(FILE* data, unsigned long accNumber)
{
	rewind(data);
	account current;
	int offset = 0;

	while(fread(&current, sizeof(account), 1, data) == 1)
	{
		offset++;
		if(current.accountNumber == accNumber) return offset;
	}
	return 0;
}