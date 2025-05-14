#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"  // 구조체, Stack 함수, DirectoryTree 등 선언 포함

// 현재 경로를 출력 (루트부터 현재 디렉토리까지)
int printPath(DirectoryTree* dirTree, Stack* dirStack) {
    DirectoryNode* current = dirTree->current;

    // 스택 비우기
    while (!isEmpty(dirStack)) {
        popStack(dirStack);
    }

    // 현재 디렉토리부터 루트까지 스택에 push
    while (current != NULL) {
        pushStack(dirStack, current->name);
        current = current->parent;
    }

    // 스택을 pop하면서 정방향으로 경로 출력
    printf("/");
    StackNode* node = dirStack->topNode;
    while (node != NULL) {
        if (strcmp(node->name, "/") != 0) {
            printf("%s", node->name);
            if (node->nextNode != NULL) {
                printf("/");
            }
        }
        node = node->nextNode;
    }
    printf("\n");

    return SUCCESS;
}

// pwd 명령어 처리 함수
int ft_pwd(DirectoryTree* dirTree, Stack* dirStack, char* cmd) {
    
    if (strcmp(cmd, "pwd") != 0) {
        fprintf(stderr, "Invalid command for pwd: %s\n", cmd);
        return FAIL;
    }

    return printPath(dirTree, dirStack);
}
