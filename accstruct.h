#ifndef ACCSTRUCT_H
#define ACCSTRUCT_H

#define STRING_LIMIT 20

typedef struct {
	unsigned long long accountNumber;
	unsigned long long pesel;
	unsigned long long balance;
	char name[STRING_LIMIT + 1];
	char lastName[STRING_LIMIT + 1];
	char address[2 * STRING_LIMIT + 1];
} account;

#endif
