#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

// 스택이 비었는지 확인
boolean isEmpty(Stack* stack) {
    return stack->cnt == 0;
}

// 스택 초기화 (메모리 할당 + 초기값 설정)
Stack* initStack() {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (!stack) {
        fprintf(stderr, "Failed to allocate memory for Stack\n");
        return NULL;
    }
    stack->topNode = NULL;
    stack->cnt = 0;
    return stack;
}

// 디렉토리 이름을 스택에 push
int pushStack(Stack* stack, char* dirName) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    if (!newNode) {
        fprintf(stderr, "Failed to allocate memory for StackNode\n");
        return FAIL;
    }
    strcpy(newNode->name, dirName);
    newNode->nextNode = stack->topNode;
    stack->topNode = newNode;
    stack->cnt++;
    return SUCCESS;
}

// 스택에서 디렉토리 이름을 pop하고 반환 (복사된 문자열 반환)
char* popStack(Stack* stack) {
    if (isEmpty(stack)) return NULL;

    StackNode* temp = stack->topNode;
    char* poppedName = strdup(temp->name);  // 문자열 복사
    stack->topNode = temp->nextNode;
    free(temp);
    stack->cnt--;

    return poppedName;
}
