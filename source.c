#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

struct BankAccount
{
	char iban[21]; // 20 for iban itself + 1 for end of the string (byte 0x00)
	char* owner_name;
	float balance;
	char currency[4];  // 3 for currency code + 1 for end of the string (byte 0x00)
};

void my_printer(struct BankAccount** account_array, unsigned int count)
{
	for (unsigned int i = 0; i < count; i++)
	{
		printf("Iban %s", account_array[i]->iban);	
		printf("Name %s", account_array[i]->owner_name);
		printf("Balance %.2f", account_array[i]->balance);
		printf("Currency %s", account_array[i]->currency);
		printf("\n");
	}
}
void empty_me(struct BankAccount** account_array, unsigned int count)
{
	for (unsigned int i = 0; i < count; i++)
	{
		free(account_array[i]->owner_name);
	}
	free(account_array);
}


int main()
{
	FILE* f = fopen("Accounts.txt", "r");
	unsigned short int line_counter = 0;
	char buffer[256];

	while (fgets(buffer, sizeof(buffer), f))
	{
		line_counter += 1;
	}

	unsigned short int account_count = line_counter / 4; // 4 is the number of fields in BankAccount
	struct BankAccount** account_array = NULL;
	account_array = malloc(sizeof(struct BankAccount) * account_count);
	unsigned int account_index = 0;
	fseek(f, 0, SEEK_SET);
	while (fgets(buffer, sizeof(buffer), f))
	{
		account_array[account_index] = malloc(sizeof(struct BankAccount));
		strcpy(account_array[account_index]->iban, buffer);
		fgets(buffer, sizeof(buffer), f);
		account_array[account_index]->owner_name = malloc(strlen(buffer) + 1);
		strcpy(account_array[account_index]->owner_name, buffer);
		fgets(buffer, sizeof(buffer), f);
		account_array[account_index]->balance = atof(buffer);
		fgets(buffer, sizeof(buffer), f);
		strcpy(account_array[account_index]->currency, buffer);
		account_index++;
	}
	my_printer(account_array, account_count);
	
	empty_me(account_array, account_count);
	fclose(f);
	return 0;
}