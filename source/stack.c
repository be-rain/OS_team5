#include "../include/main.h"

boolean isEmpty(Stack* stack) {
    if(stack->topNode == NULL){
        return TRUE;
    }
    return FALSE;
}

Stack *initStack() {
	Stack *newStack = (Stack *)malloc(sizeof(Stack));

	if (!newStack) {
        return NULL;
	}
    newStack->topNode = NULL;
	return newStack;
}

int pushStack(Stack *dirStack, char *dirName) {
	StackNode *dirNode = (StackNode *)malloc(sizeof(StackNode));

	if (!dirStack || !dirNode) {
        return FALSE;
	}
	strncpy(dirNode->name, dirName, MAX_NAME);
	dirNode->nextNode = dirStack->topNode;
    dirStack->topNode = dirNode;
    return TRUE;
}

char *popStack(Stack *dirStack) {
	StackNode *returnNode = NULL;
	if (!dirStack || (isEmpty(dirStack) == TRUE)) {
        return NULL;
	}
	returnNode = dirStack->topNode;
	dirStack->topNode = returnNode->nextNode;
	return returnNode->name;
}
