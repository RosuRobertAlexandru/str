#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>


#pragma region Struct info
struct Fotografie
{
	int id;
	char* oras;
	char* data;
	float rezolutie;
};


typedef struct Fotografie Fotografie;

void printFoto(Fotografie* foto)
{
	printf("%d %s %s %.2f \n", foto->id,foto->oras,foto->data,foto->rezolutie);
}
#pragma endregion

#pragma region SimpleList
typedef struct NodeSL NodeSL;
struct NodeSL {
	Fotografie* info;
	struct NodeSL* next;
};
NodeSL* insertSL(NodeSL* list,Fotografie* f)
{
	NodeSL* temp = malloc(sizeof(NodeSL));
	temp->info = f;
	temp->next = list;

	return temp;
}
void printerSL(NodeSL* list)
{
	NodeSL* temp = list;
	while (temp)
	{
		printFoto(temp->info);
		temp = temp->next;
	}
}
NodeSL* dealocateSL(NodeSL* list)
{
	while(list){
	NodeSL* temp = list;
	list = list->next;
	free(temp->info->data);
	free(temp->info->oras);
	free(temp->info);
	free(temp);
	}
	return list;
}
#pragma endregion

#pragma region DoubleList
typedef struct NodeDL NodeDL;
typedef struct DoubleList DoubleList;
struct NodeDL {
	Fotografie* info;
	NodeDL* next;
	NodeDL* prev;
};
struct DoubleList {
	NodeDL* head;
	NodeDL* tail;
};

DoubleList* insertDL(DoubleList* list, Fotografie* f)
{
	NodeDL* temp = (NodeDL*)malloc(sizeof(NodeDL));
	temp->next = list->head;
	temp->info = f;
	temp->prev = NULL;
	if (list->head == NULL)
	{
		list->head = list->tail = temp;
	}else{
		list->head->prev = temp;
		list->head = temp;
	}
	return list;
}
void printerDL(DoubleList* list)
{
	NodeDL* temp = list->head;
	while (temp)
	{
		printFoto(temp->info);
		temp = temp->next;
	}
}

DoubleList* dealocateDL(DoubleList* list) {
	
	while (list->head)
	{
		NodeDL* temp = list->head;
		list->head = list->head->next;
		free(temp->info->data);
		free(temp->info->oras);
		free(temp->info);
		free(temp);

	}

	return list;
}
#pragma endregion

#pragma region HashTable

#define HASHTABLE_SIZE 100

typedef struct HashNode HashNode;
struct HashNode {
	Fotografie* info;
	HashNode* next;
};

unsigned char fhash(char* key)
{

	return (strlen(key) * 13 / 100)%HASHTABLE_SIZE;
}
HashNode* createHashNode(HashNode* l, Fotografie* f)
{
	HashNode* temp = malloc(sizeof(HashNode));
	temp->next = l;
	temp->info = f;
	return temp;
}
HashNode* insertHT(HashNode** ht, Fotografie* f)
{
	unsigned char off = fhash(f->oras);
	ht[off] = createHashNode(ht[off], f);
	return ht;
}
void printHT(HashNode** ht) {
	if(ht!=NULL)
	for (int i = 0; i < HASHTABLE_SIZE; i++)
	{
		if (ht[i])
		{
			HashNode* temp = ht[i];
			while (temp)
			{
				printFoto(temp->info);
				temp = temp->next;
			}
		}
	}
}

HashNode* dealocateHT(HashNode** ht)
{
	for (int i = 0; i < HASHTABLE_SIZE; i++)
	{
		while (ht[i])
		{
			HashNode* temp = ht[i];
			ht[i] = ht[i]->next;
			free(temp->info->data);
			free(temp->info->oras);
			free(temp->info);
			free(temp);

		}
	}
	return ht;
}
#pragma endregion

#pragma region BST
typedef struct BST BST;
struct BST {
	Fotografie* info;
	BST* left;
	BST* right;
};

BST* insert(BST* r, Fotografie* f)
{
	if (r == NULL)
	{
		BST* temp = (BST*)malloc(sizeof(BST));
		temp->info = f;
		temp->left = NULL;
		temp->right = NULL;
		r = temp;
	}
	else
	{
		if (r->info->id > f->id) {
			r->left = insert(r->left, f);
		}
		else {
			r->right = insert(r->right, f);
		}

	}

	return r;
}

void printerBST(BST* r)//in ORDER	
{
	if (r != NULL)
	{
		printerBST(r->left);
		printFoto(r->info);
		printerBST(r->right);
	}
	
}
void printerBST2(BST* r)// Preorder
{
	if (r != NULL)
	{
		printFoto(r->info);

		printerBST2(r->left);
		
		printerBST2(r->right);
		
	}

}
void printerBST3(BST* r)//postorder
{
	if (r != NULL)
	{
		
		printerBST3(r->left);
		printerBST3(r->right);
		printFoto(r->info);
	}

}
BST* dealocateBST(BST* r)
{
	if (r == NULL)
		return r;
	r->left = dealocateBST(r->left);
	r->right = dealocateBST(r->right);

	free(r->info->data);
	free(r->info->oras);
	free(r->info);
	free(r);
	
	return NULL;
}
#pragma endregion

#pragma region AVL
typedef struct AVL AVL;
struct AVL {
	Fotografie* info;
	char balance;
	AVL* left;
	AVL* right;
};

int maxim(int a, int b) {
	return a > b ? a: b;
}
int height(AVL* r)
{
	if (r)
		return 1 + maxim(height(r->left), height(r->right));
	else
		return 0;
}
void computeBALANCE(AVL* r)
{
	if (r) {
		r->balance = height(r->right) - height(r->left);
	}
}


AVL* rotireSimplaDr(AVL* pivot) {
	AVL* fiuSt = pivot->left;
	pivot->left = fiuSt->right;
	fiuSt->right = pivot;
	computeBALANCE(pivot);
	computeBALANCE(fiuSt);
	return fiuSt;
}

AVL* rotireSimplaSt(AVL* pivot) {
	AVL* fiuDr = pivot->right;
	pivot->right = fiuDr->left;
	fiuDr->left = pivot;
	computeBALANCE(pivot);
	computeBALANCE(fiuDr);
	return fiuDr;
}

AVL* rotireDblStDr(AVL* pivot) {
	pivot->left = rotireSimplaSt(pivot->left);
	return rotireSimplaDr(pivot);
}

AVL* rotireDblDrSt(AVL* pivot) {
	pivot->right = rotireSimplaDr(pivot->right);
	return rotireSimplaSt(pivot);
}

AVL* insertAVL(AVL* r, Fotografie* f) {
	if (r == NULL) {
		AVL* temp = (AVL*)malloc(sizeof(AVL));
		temp->info = f;
		temp->left = NULL;
		temp->right = NULL;
		temp->balance = 0;
		return temp;
	}

	//if (strcmp(r->info->oras, f->oras) > 0) {
	if (r->info->id>f->id) {
		r->left = insertAVL(r->left, f);
	}
	else {
		r->right = insertAVL(r->right, f);
	}

	computeBALANCE(r);

	if (r->balance == 2) {
		if (r->right->balance < 0) {
			r = rotireDblDrSt(r);
		}
		else {
			r = rotireSimplaSt(r);
		}
	}
	else if (r->balance == -2) {
		if (r->left->balance > 0) {
			r = rotireDblStDr(r);
		}
		else {
			r = rotireSimplaDr(r);
		}
	}

	return r;
}

void printerAVL(AVL* r) {
	if (r) {
		printerAVL(r->left);
		printFoto(r->info);
		printerAVL(r->right);
	}
}
AVL* dealocateAVL(AVL* r) {
	if (r) {
		dealocateAVL(r->left);
		dealocateAVL(r->right);
		
		free(r->info->data);
		free(r->info->oras);
		free(r->info);
		free(r);
		
	}
	return NULL;
}
#pragma endregion
int main()
{
	FILE* f = fopen("Text.txt", "r");
	char buffer[128];
	

	NodeSL *simpleList = NULL;

	DoubleList* doubleList=malloc(sizeof(DoubleList));
	doubleList->head = NULL;
	doubleList->tail = NULL;


	HashNode** HT = NULL;
	HT = (HashNode**)malloc(sizeof(HashNode*) * HASHTABLE_SIZE);
	for (unsigned i = 0; i < HASHTABLE_SIZE; i++)
		HT[i] = NULL;

	BST* rootBST = NULL;

	AVL* rootAVL = NULL;

	while (fgets(buffer, sizeof(buffer), f))
	{
		Fotografie* foto = malloc(sizeof(Fotografie));
		buffer[strlen(buffer ) - 1] = 0;
		foto->id = atoi(buffer);

		fgets(buffer, sizeof(buffer), f);
		buffer[strlen(buffer)-1] = 0;
		foto->oras=(char*)malloc(strlen(buffer)+1);
		strcpy(foto->oras, buffer);

		fgets(buffer, sizeof(buffer), f);
		buffer[strlen(buffer) - 1] = 0;
		foto->data= (char*)malloc(strlen(buffer)+1);
		strcpy(foto->data, buffer);

		fgets(buffer, sizeof(buffer), f);
		buffer[strlen(buffer ) - 1] = 0;
		foto->rezolutie = atof(buffer);
		
		//printFoto(foto);

		//simpleList = insertSL(simpleList, foto);
		//doubleList = insertDL(doubleList, foto);
		//HT = insertHT(HT, foto);
		//rootBST = insert(rootBST, foto);
		//rootAVL= insertAVL(rootAVL, foto);
	}
	
	
#pragma region SimpleList_Test
	/*printerSL(simpleList);
	simpleList = dealocateSL(simpleList);
	printerSL(simpleList);*/
#pragma endregion

#pragma region DoubleList_Test
	/*printerDL(doubleList);
	dealocateDL(doubleList);
	printerDL(doubleList);*/
#pragma endregion

#pragma region HashTable_Test
	/*printHT(HT);
	dealocateHT(HT);
	printHT(HT);*/
#pragma endregion

#pragma region BST_Test
	/*printf("==========in order\n");
	printerBST(rootBST);
	printf("==========pre order\n");
	printerBST2(rootBST);
	printf("==========post order\n");
	printerBST3(rootBST);


	rootBST = dealocateBST(rootBST);
	printf("==========in order\n");
	printerBST(rootBST);*/
#pragma endregion

#pragma region AVL_Test
	/*printerAVL(rootAVL);
	rootAVL=dealocateAVL(rootAVL);
	printerAVL(rootAVL);*/
#pragma endregion


	fclose(f);
	return 0;
}