#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

// 상대경로/절대경로 포함하여 실제 경로로 이동 처리
int changeCurrentDirectory(DirectoryTree* dirTree, char* dirPath) {

    //복사본 생성
    DirectoryNode* temp;
    char path[MAX_BUFFER];

    // path 사본 만들기 (strtok은 문자열을 파괴하니까)
    strcpy(path, dirPath);

    // 1. cd /경우  2. cd /home/user/osteam의 경우 3. pwd=/home/user 일 때 cd osteam을 하는 경우

    if (strcmp(path, "/") == 0) { // dirpath=="/"면 root로 이동
        dirTree->current = dirTree->root;
        return SUCCESS;
    }
    else if (path[0] == '/') {
        // 절대경로 → 루트부터 시작
        temp = dirTree->root;
        // strtok이 제대로 작동하려면 /를 제거해야 함 → "/etc/test" → "etc/test"
        memmove(path, path + 1, strlen(path)); // 슬래시 한 칸 밀기
    }
    else {
        // 상대경로 → 현재 위치에서 시작
        temp = dirTree->current;
    }

    // path에서 /제거한 요소 하나 얻기(ex /home/ro 였다면 token은 home)
    char* token = strtok(path, "/");
    while (token != NULL) {
        if (strcmp(token, "..") == 0) { // cd ..이라면 부모 노드로 변경
            if (temp->parent != NULL)
                temp = temp->parent;
        }
        else {                                   // cd /bin 이었다면 현재 dirTree는 root이므로 여러 자식들 탐색
            DirectoryNode* child = temp->firstChild;
            boolean found = FALSE;
            while (child != NULL) {
                if (strcmp(child->name, token) == 0 && child->type == 'd') {
                    temp = child;
                    found = TRUE;
                    break;
                }
                child = child->nextSibling;     // child 노드의 형제 탐색
            }
            if (!found) {
                printf("cd: no such directory: %s\n", token);
                return FAIL;
            }
        }
        token = strtok(NULL, "/");
    }

    dirTree->current = temp;
    return SUCCESS;
}



// cd 명령어 시 처리 과정
int ft_cd(DirectoryTree* dirTree, char* cmd) {
    char* token = strtok(cmd, " ");  // cd 추출 
    token = strtok(NULL, " ");       // 이동 경로(ex /home

    if (token == NULL) {
        char path[MAX_BUFFER] = "/home/";
        strcat(path,usrList->current->name);
        return changeCurrentDirectory(dirTree, path);
    }
    return changeCurrentDirectory(dirTree, token);
}
