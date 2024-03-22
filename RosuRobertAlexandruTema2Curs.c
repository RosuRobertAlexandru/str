#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "memory.h"
#include "string.h"
#include "stdlib.h"
#include "ctype.h"

struct Stack {
	char letter;
	struct Stack* next;
};

struct Stack* Push(struct Stack* thisStack,char letter)
{
	struct Stack* temp = (struct Stack*)malloc(sizeof(struct Stack));
	temp->letter = letter;
	temp->next = thisStack;
	return temp;
}
struct Stack* Pop(struct Stack* thisStack)
{
	struct Stack* temp = thisStack;
	thisStack = thisStack->next;
	free(temp);
	return thisStack;
}
void myPrinter(struct Stack* thisStack)
{
	struct Stack* temp = thisStack;
	while (temp!=NULL)
	{
		printf("%c", temp->letter);
		temp = temp->next;
	}
}

struct Stack* fillMe(struct Stack* thisStack, char ThoseLetters[])
{
	for (unsigned int i = 0; i < strlen(ThoseLetters); i++)
	{
		thisStack = Push(thisStack, ThoseLetters[i]);
	}
	return thisStack;
}

struct Stack* reverseMe(struct Stack* thisIsGonnaBeReversed)
{
	struct Stack* reversedStack = NULL;
	struct Stack* temp = thisIsGonnaBeReversed;

	while (temp != NULL)
	{
		reversedStack = Push(reversedStack, temp->letter);
		temp = temp->next;
	}

	return reversedStack;
}
int Palindrom(struct Stack* left, struct Stack* right) {
	int presumption = 1;
	struct Stack* tempL = left;
	struct Stack* tempR = right;
	while (tempL != NULL) {
		if (tempL->letter!=tempR->letter) {
			presumption = 0;
		}
		tempL = tempL->next;
		tempR = tempR->next;
	}
	
	return presumption;
}
void freeMe(struct Stack* thisStack)
{
	while (thisStack != NULL)
	{
		struct Stack* temp = thisStack;
		thisStack = thisStack->next;
		free(temp);
	}
}

int main()
{
	char Pali[] = "Able was I ere I saw Elba";
	struct Stack* stack1 = NULL;
	struct Stack* stack1R = NULL;
	stack1 = fillMe(stack1, Pali);
	stack1R = reverseMe(stack1);
	
	myPrinter(stack1); printf("\n");
	myPrinter(stack1R); printf("\n");
	printf("We consider lower and upper letter different\n");
	if (Palindrom(stack1, stack1R) == 1)
	{
		printf("it is\n");
	}
	else
	{
		printf("it is not\n");
	}
	printf("=============================================\n");

	struct Stack* stack2 = NULL;
	struct Stack* stack2R = NULL;
	
	for (unsigned int i = 0; i < strlen(Pali); i++)
	{
		Pali[i] = tolower(Pali[i]);
	}
	stack2 = fillMe(stack2, Pali);
	stack2R = reverseMe(stack2);


	myPrinter(stack2); printf("\n");
	myPrinter(stack2R); printf("\n");
	printf("We consider lower and upper letter the same\n");
	if (Palindrom(stack2, stack2R) == 1)
	{
		printf("it is\n");
	}
	else
	{
		printf("it is not\n");
	}
	
	freeMe(stack1);
	freeMe(stack1R);
	freeMe(stack2);
	freeMe(stack2R);
	
	return 0;
}