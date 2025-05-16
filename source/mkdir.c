#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "main.h"


int makeDir(DirectoryTree* dirTree, char* dirName, char type) {

    // main.h에 있지만 .c로 구현해야함
    // 만약 똑같은 디렉토리 존재 시 알림
    if (dirExistence(dirTree, dirName, type) != NULL) {
        printf("Directory '%s' already exists.\n", dirName);
        return FAIL;
    }

    // 새로운 Node 생성 여기서 type은 'd'로 고정됨
    DirectoryNode* newNode = (DirectoryNode*)malloc(sizeof(DirectoryNode));

    // 새로운 Node의 이름, ID, 생성 시간 등. 정보추가
    strcpy(newNode->name, dirName);
    newNode->type = type;
    newNode->SIZE = 0;
    newNode->id.UID = dirTree->current->id.UID;
    newNode->id.GID = dirTree->current->id.GID;
    newNode->permission.mode = 755;


    // main.h에 있지만 .c로 구현해야함
    getToday(&(newNode->date));
    // main.h에 있지만 .c로 구현해야함
    modeToPermission(newNode);

    //현재 위치의 Node를 부모로 연결
    newNode->parent = dirTree->current;
    newNode->firstChild = NULL;
    newNode->nextSibling = NULL;

    //만약 기존 child 존재 없다면 firstchild 차지
    if (dirTree->current->firstChild == NULL) {
        dirTree->current->firstChild = newNode;
    }
    // 기존 child 있다면 마지막 sibling으로 연결됨
    else {
        DirectoryNode* sibling = dirTree->current->firstChild;
        while (sibling->nextSibling != NULL) {
            sibling = sibling->nextSibling;
        }
        sibling->nextSibling = newNode;
    }

    printf("Directory '%s' created successfully.\n", dirName);
    return SUCCESS;
}

 
    //멀티쓰레딩을 위함 쓰레드 생성 후 makeDirUsedThread 함수가 실행됨
void* makeDirUsedThread(void* arg) {
    ThreadTree* args = (ThreadTree*)arg;
    makeDir(args->threadTree, args->command, 'd'); 
    pthread_exit(NULL);
}

    // mkdir a b c가 입력되면 cmd에 저장됨
int ft_mkdir(DirectoryTree* dirTree, char* cmd) {
    char* token;
    char* delim = " ";
    int threadCount = 0;
    pthread_t tid[MAX_THREAD];
    // 여기있는 ThreadTree는 main.h에 정의되어있음
    ThreadTree* args[MAX_THREAD];

    token = strtok(cmd, delim);  // 첫 번째 토큰은 "mkdir" 이므로 skip
    token = strtok(NULL, delim); // 두 번째 토큰부터 실행 ex) cmd a에서 a가 할당됨

    // 토큰이 NULL이거나 쓰레드의 개수가 너무 커지면 종료
    while (token != NULL && threadCount < MAX_THREAD) {
        //ThreadTree의 정보 할당
        args[threadCount] = (ThreadTree*)malloc(sizeof(ThreadTree));
        args[threadCount]->threadTree = dirTree;
        args[threadCount]->command = strdup(token); // 디렉토리 이름
        // 쓰레드 생성 후 아까 선언한 makeDirUsedThread 실행
        pthread_create(&tid[threadCount], NULL, makeDirUsedThread, (void*)args[threadCount]);
        threadCount++;

        token = strtok(NULL, delim);
    }

    for (int i = 0; i < threadCount; i++) {
        pthread_join(tid[i], NULL);
        free(args[i]->command);
        free(args[i]);
    }

    return SUCCESS;
}

// 동작 방식
// mkdir a b c가 들어오면 
// ft_mkdir(DirectoryTree* dirTree, char* cmd) 이 함수가 먼저 실행됨
// while 문에서 각각 a, b, c에 대한 쓰레드가 순차적으로 생성되고 각 쓰레드는 makeDirUsedThread 함수를 통해 makeDir을 진행
// 이 경우 mkdir a 하나인 경우에도 쓰레드가 하나 생성되고 makeDir 진행 후 종료됨



