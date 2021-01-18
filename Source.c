#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_MAX_NAME (1024)
#define MAX_LENGTH (11)

struct _TreeNode;
typedef struct _TreeNode * PositionT;
typedef struct _TreeNode {
	PositionT Lchild;
	PositionT Rchild;
	char data[MAX_LENGTH];
}TreeNode;

struct _Stack;
typedef struct _Stack* PositionS;
typedef struct _Stack {
	PositionS next;
	PositionT treeNode;
}Stack;

PositionT createNode(char* data);
int Push(PositionS head, PositionT Node);
PositionT Pop(PositionS head);
PositionT readFromFile(char* fileName);
int isNumber(char* str);
PositionT deleteAll(PositionT root);
int printInOrder(PositionT current, FILE* fileName);
int postfixToInfix(PositionT root, char* fileName);

int main(void) {

	PositionT root = NULL;
	char fileName[FILENAME_MAX] = { 0 };

	printf("Insert file name: ");
	scanf("%s", fileName);

	root = readFromFile(fileName);

	if (!root)
		return EXIT_FAILURE;
	else {
		printf("Enter the file name where you want to convert postfix to infix: ");
		scanf(" %s", fileName);

		postfixToInfix(root, fileName);
		deleteAll(root);


		return EXIT_SUCCESS;
	}
}

PositionT createNode(char* data) {
	PositionT K = NULL;
	K = (PositionT)malloc(sizeof(TreeNode));
	if (!K) {
		printf("Memory allocation failed!");
		return 0;
	}
	strcpy(K->data, data);
	K->Lchild = NULL;
	K->Rchild = NULL;
	
	return K;
}

PositionT readFromFile(char* fileName) {
	Stack head;
	head.next = NULL;
	PositionT Result = NULL;
	char data[MAX_LENGTH] = { 0 };

	FILE* File = NULL;
	File = fopen(fileName, "r");
	if (!File) {
		printf("File failed to open!");
		return 0;
	}

	while (!feof(File)) {

		PositionT Node = NULL;
		fscanf(File, "%s", data);
		Node = createNode(data);

		if (!Node) {
			fclose(File);
			return EXIT_SUCCESS;
		}

		if (isNumber(data))
			Push(&head, Node);
		else 
		{
			Node->Rchild = Pop(&head);

			if (!Node->Rchild) {
				printf("Postfix expression is incorrect!\n");
				break;
			}

			Node->Lchild = Pop(&head);

			if (!Node->Lchild) {
				printf("Postfix expression is incorrect!\n");
				break;
			}

			Push(&head, Node);
		}
	}

	fclose(File);

	Result = Pop(&head);
	if (!Result) {
		printf("File is empty or incorrect!\n");
		return EXIT_SUCCESS;
	}

	return Result;
}

int Push(PositionS head, PositionT Node) {
	
	PositionS First = NULL;
	
	First = (PositionS)malloc(sizeof(Stack));
	if (!First) {
		printf("Memory allocation failed!");
		return EXIT_SUCCESS;
	}
	
	First->treeNode = Node;
	First->next = head->next;
	head->next = First;

	return 0;
}

PositionT Pop(PositionS head) {
	PositionS Temp = head->next;
	PositionT Result = NULL;

	if (!Temp)
		return NULL;

	Result = Temp->treeNode;
	head->next = Temp->next;
	free(Temp);

	return Result;

}

int isNumber(char* str) {
	int number = 0;

	if (sscanf(str, "%d", &number) == 1) return 1;

	return 0;
}

PositionT deleteAll(PositionT current) {

	if (current == 0)
		return NULL;

	deleteAll(current->Lchild);
	deleteAll(current->Rchild);
	free(current);

	return NULL;
}

int printInOrder(PositionT current, FILE* fileName) {
	if (current == NULL)
		return EXIT_FAILURE;

	fprintf(fileName, "(");
	printInOrder(current->Lchild, fileName);
	fprintf(fileName,"%s", current->data);
	printInOrder(current->Rchild, fileName);
	fprintf(fileName, ")");

	return EXIT_SUCCESS;

}

int postfixToInfix(PositionT root, char* fileName) {
	FILE* File = NULL;
	File = fopen(fileName, "w");
	if (!File) {
		printf("File failed to open!");
		return EXIT_FAILURE;
	}
	printInOrder(root, File);
	fclose(File);

	return EXIT_SUCCESS;
}