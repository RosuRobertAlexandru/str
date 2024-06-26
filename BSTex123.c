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

struct NodeBST
{
	struct BankAccount info;
	struct NodeBST* left, * right;
};

typedef struct BankAccount BankAccount;
typedef struct NodeBST NodeBST;

NodeBST* insert_ba_tree(NodeBST* r, BankAccount tAccount, unsigned char* insert_flag)
{
	if (r != NULL)
	{
		// the search of the NULL place in the tree is continuing
		if (strcmp(r->info.iban, tAccount.iban) > 0)
		{
			// go on the below level on the left
			r->left = insert_ba_tree(r->left, tAccount, insert_flag);
		}
		else
		{
			if (strcmp(r->info.iban, tAccount.iban) < 0)
			{
				// go to the right subtree
				r->right = insert_ba_tree(r->right, tAccount, insert_flag);
			}
			else
			{
				// there is the key already stored within the BST
				*insert_flag = 0; // the insertion is cancelled
			}
		}
	}
	else
	{
		// this is the place where the new node must be allocated and initialized
		NodeBST* new_node = (NodeBST*)malloc(sizeof(NodeBST));
		new_node->info = tAccount;
		new_node->left = NULL;
		new_node->right = NULL;

		*insert_flag = 1; // the insertion is done
		r = new_node;
	}

	return r;
}

void parse_tree(NodeBST* r)
{
	if (r != NULL)
	{
		parse_tree(r->left);
		printf("%s %s\n", r->info.iban, r->info.owner_name);
		parse_tree(r->right);
	}
}
void currencyCounter(NodeBST* r, char* toFind, int* counter) {
	if (r != NULL)
	{
		currencyCounter(r->left, toFind, counter);
		if (strcmp(r->info.currency, toFind) == 0)
			(* counter)++;
		currencyCounter(r->right, toFind, counter);
	}
}
void leafCounter(NodeBST* r, int* counter)
{
	if (r != NULL)
	{
			leafCounter(r->left, counter);
			if (r->left == NULL && r->right == NULL)
			{
				(*counter)++;
			}
			leafCounter(r->right, counter);	
	}

}
void dealocBST(NodeBST* r)
{
	if (r != NULL)
	{
		dealocBST(r->left);
		dealocBST(r->right);
		free(r->info.owner_name);
		free(r);
	}
}
int main()
{
	FILE* f = fopen("Accounts.txt", "r");
	NodeBST* root = NULL; // an empty binary search tree
	char buffer[256];

	while (fgets(buffer, sizeof(buffer), f))
	{
		BankAccount tAccount;
		buffer[strlen(buffer) - 1] = 0; // overwrites the byte 0x0a ('\n') with byte 0x00 ('\n' added to buffer by fgets)
		strcpy(tAccount.iban, buffer);

		fgets(buffer, sizeof(buffer), f); // for the owner's name
		buffer[strlen(buffer) - 1] = 0;
		tAccount.owner_name = (char*)malloc(strlen(buffer) + 1);
		strcpy(tAccount.owner_name, buffer);

		fgets(buffer, sizeof(buffer), f); // for the bank account balance
		buffer[strlen(buffer) - 1] = 0;
		tAccount.balance = (float)atof(buffer);

		fgets(buffer, sizeof(buffer), f); // for the bank account currency
		buffer[strlen(buffer) - 1] = 0;
		strcpy(tAccount.currency, buffer);

		// insert BankAccount data (tAccount) into binary search tree
		unsigned char insert_flag = 0;
		root = insert_ba_tree(root, tAccount, &insert_flag);

		if (insert_flag != 0)
		{
			printf("Bank account %s has been sucessfully inserted!\n", tAccount.iban);
		}
		else
		{
			printf("Bank account %s has not been inserted!\n", tAccount.iban);
		}
	}

	printf("\nThe BST after creation:\n");
	parse_tree(root);

	// count BAs with the same currency within the BST
	int ronCounter = 0; 
	currencyCounter(root, "RON",&ronCounter);
	int euroCounter = 0; 
	currencyCounter(root, "EUR",&euroCounter);
	printf("EX1:=================================\n");
	printf("%d ron\n", ronCounter);
	printf("%d eur\n", euroCounter);
	// count BAs as leafs within the BST
	int numberLEafes = 0;
	leafCounter(root,&numberLEafes);
	printf("EX2:=================================\n");
	printf("%d leafes\n", numberLEafes);
	// BST deallocation
	printf("EX3:=================================\n");
	dealocBST(root);
	root = NULL;
	fclose(f);
	return 0;
}