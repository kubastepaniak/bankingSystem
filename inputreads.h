#ifndef INPUTREADS_H
#define INPUTREADS_H

#define STRING_LIMIT 20

extern int numberOfClients;
void readString(char string[]);
void readMoney(unsigned long* money);
void readNumber(unsigned long* number);
void readPesel(unsigned long* number);
void readAddress(char address[]);
int moneyIO(char operation, unsigned long accNumber, unsigned long amount);
char* balanceToStr(char* string, unsigned long number);

#endif
