#ifndef INPUTREADS_H
#define INPUTREADS_H

#define STRING_LIMIT 20

extern int numberOfClients;
void readString(char string[]);
void readMoney(unsigned long long* money);
void readNumber(unsigned long long* number);
void readPesel(unsigned long long* number);
void readAddress(char address[]);
int moneyIO(char operation, unsigned long long accNumber, unsigned long long amount);
char* balanceToStr(char* string, unsigned long long number);

#endif
