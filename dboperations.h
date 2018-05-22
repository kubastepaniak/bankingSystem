#ifndef DBOPERATIONS_H
#define DBOPERATIONS_H

#include "accstruct.h"
#define STRING_LIMIT 20

extern int numberOfClients;
int getNumberOfClients();
int systemInteface();
void addNewClient();
void deposit();
void withdraw();
void transfer();
void showInformationAll();
void showInformation(account current);
void searchBy();
account* searchFor(char type[], ...);
int searchingStrings(char searchWhat[]);
int searchingNumbers(char searchWhat[]);
int fsearch(FILE* data, unsigned long accNumber);
int confirm();

#endif