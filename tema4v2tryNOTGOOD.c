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

struct NodeD
{
	struct BankAccount* info;
	struct NodeD* next, * prev;
};

typedef struct BankAccount BankAccount;
typedef struct NodeD NodeD;

struct DoubleList
{
	NodeD* head, * tail;
};

typedef struct DoubleList DoubleList;

DoubleList insert_start(DoubleList list, BankAccount* pba)
{
	NodeD* newNode = (NodeD*)malloc(sizeof(NodeD));
	newNode->info = pba;
	newNode->next = list.head;
	newNode->prev = NULL;

	if (list.head == NULL)
	{
		// the double list is empty
		list.head = list.tail = newNode;
	}
	else
	{
		list.head->prev = newNode;
		list.head = newNode;
	}

	return list;
}

DoubleList deallocate_list(DoubleList list)
{
	while (list.head != NULL)
	{
		// there is one node (at least) in the double list
		NodeD* temp = list.head; // save the heap address of the 1st node into a temporary local variable
		list.head = list.head->next; // list stores the address of the 2nd node in the list

		free(temp->info->owner_name); // release the 3rd level heap allocated location (string for owner's name)
		free(temp->info);			  // release the BankAccount structure (2nd allocation level in heap)
		free(temp);					 // release the 1st level heap allocated location (NodeD)
	}

	list.tail = NULL;

	return list;
}

DoubleList swap(DoubleList list, unsigned char position_first)
{
	if (list.head == NULL || list.head->next == NULL)
		return list;

	NodeD* temp = list.head;
	NodeD* left, * position, * right, * rightNext;
	left = NULL; position = NULL; right = NULL; rightNext = NULL;
	if (position_first == 1)
	{
		position = temp;
		right = temp->next;
		rightNext = temp->next->next;

		list.head = right;
		right->next = position;
		position->next = rightNext;

		right->prev = NULL;
		position->prev = right;
		rightNext->prev = position;
		return list;
	}

	unsigned char counter = 0;
	while (temp != list.tail && counter <= position_first)
	{

		if (counter == position_first - 1 && temp->next->next != NULL)
		{
			left = temp->prev;
			position = temp;
			right = temp->next;
			rightNext = temp->next->next;

			left->next = right;
			position->next = rightNext;
			right->next = position;

			right->prev = left;
			position->prev = right;
			rightNext->prev = position;

		}

		counter++;
		temp = temp->next;
	}

	return list;
}
DoubleList swap2(DoubleList list, unsigned char p1, unsigned char p2)
{
	if (p1 > p2) {// i want p1<p2
		p1 = p1 + p2;
		p2 = p1 - p2;
		p1 = p1 - p2;
	}
	if (list.head == NULL || list.head->next == NULL)
		return list;
	if (p1 == p2) {
		return list;// no change needed 
	}
	NodeD* temp = list.head;
	NodeD* L1, * PP1, * R1, * L2, * PP2, * R2;
	L1 = NULL; R1 = NULL; L2 = NULL; R2 = NULL; PP1 = NULL; PP2 = NULL;
	unsigned char counter = 0,max=0;
	while(temp!=list.tail)
	{
		max++;
		temp = temp->next;
	}
	temp = list.head;
	//p1 is head
	// p2 is tail
	//p1 head and p2 tail

	//general case
	while (temp != list.tail && counter <= p2)
	{ if(p1!=0&&p2!=max)
	{ 
		if (counter == p1)
		{
			L1 = temp->prev;
			PP1 = temp;
			R1 = temp->next;
		}
		if (counter == p2)
		{
			L2 = temp->prev;
			PP2 = temp;
			R2 = temp->next;

			// after we found the 2nd node we can change and return 

			L1->next = PP2;
			PP1->next = R2;
			L2->next = PP1;
			PP2->next = R1;

			PP1->prev = L2;
			R1->prev = PP2;
			PP2->prev = L1;
			R2->prev = PP1;
			return list;
		}
	}
	//p1 head p2 not tail
	if (p1 == 0 && p2 < max)
	{
		if (counter == p1)
		{
			L1 = temp->prev;
			PP1 = temp;
			R1 = temp->next;
		}
		if (counter == p2)
		{
			L2 = temp->prev;
			PP2 = temp;
			R2 = temp->next;
		
		list.head = PP2;
		PP1->next = R2;
		L2->next = PP1;
		PP2->next = R1;

		PP1->prev = L2;
		R1->prev = PP2;
		PP2->prev = NULL;
		R2->prev = PP1;
		return list;
		}
	}
	// p1 not heade and p2 is tail
	if (p1 > 0 && p2 == max)
	{
		if (counter == p1)
		{
			L1 = temp->prev;
			PP1 = temp;
			R1 = temp->next;
		}
		if (counter == p2)
		{
			L2 = temp->prev;
			PP2 = temp;
			R2 = temp->next;
		
		list.tail = PP2;
		L1->next = PP2;
		PP1->next = NULL;
		L2->next = PP1;
		PP2->next = R1;

		PP1->prev = L2;
		R1->prev = PP2;
		PP2->prev = L1;
		

		return list;
		}
	}
	//p1 head and p2 tail
	if (p1 == 0 && p2 == max)
	{
		if (counter == p1)
		{
			L1 = temp->prev;
			PP1 = temp;
			R1 = temp->next;
		}
		if (counter == p2)
		{
			L2 = temp->prev;
			PP2 = temp;
			R2 = temp->next;
		
		list.head = PP1;
		list.tail = PP2;

		PP1->next = NULL;
		L2->next = PP1;
		PP2->next = R1;

		PP1->prev = L2;
		R1->prev = PP2;
		PP2->prev = NULL;
		
		return list;
		}
	}

		counter++;
		temp = temp->next;
	}

	return list;
}
int main()
{
	FILE* f = fopen("Accounts.txt", "r");
	DoubleList DList;
	DList.head = DList.tail = NULL; // the double list is initially empty
	char buffer[256];

	while (fgets(buffer, sizeof(buffer), f))
	{
		BankAccount tAccount, * pAccount;
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

		// allocate heap memory for BankAccount
		pAccount = (BankAccount*)malloc(sizeof(BankAccount));
		// copy data from tAccount (stack seg) to *pAccount (heap seg)
		*pAccount = tAccount;

		// call insert double node at the begining of the double list
		DList = insert_start(DList, pAccount);
	}

	printf("Double list after creation:\n");
	NodeD* temp = DList.tail;
	while (temp != NULL)
	{
		printf("%s %s\n", temp->info->iban, temp->info->owner_name);
		temp = temp->prev;
	}

	printf("Double list after swap i with i+1:\n");

	//DList = swap(DList, 3);
	temp = DList.tail;
	while (temp != NULL)
	{
		printf("%s %s\n", temp->info->iban, temp->info->owner_name);
		temp = temp->prev;
	}

	printf("Double list after swap i with j:\n");

	DList = swap2(DList, 2, 4);
	temp = DList.tail;
	while (temp != NULL)
	{
		printf("%s %s\n", temp->info->iban, temp->info->owner_name);
		temp = temp->prev;
	}


	DList = deallocate_list(DList);

	printf("Double list after deallocation:\n");
	temp = DList.head;
	while (temp != NULL)
	{
		printf("%s %s\n", temp->info->iban, temp->info->owner_name);
		temp = temp->next;
	}


	fclose(f);
	return 0;
}