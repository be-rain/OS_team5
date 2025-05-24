/* chown.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chown.h"    // 반드시 자신의 헤더를 include

/**
 * @note flag == 0: 단일 변경, flag == 1: 재귀 변경
 */
int changeOwner(DirectoryTree* dirTree,
    char* userName,
    char* dirName,
    int                 flag)
{
    // 1) 사용자 조회
    UserNode* usr = userExistence(usrList, userName);
    if (!usr) {
        printf("chown: Invalid user '%s'\n", userName);
        printf("Try 'chown --help' for more information.\n");
        return FAIL;
    }

    // 2) 디렉터리 또는 파일 노드 조회
    DirectoryNode* dirNode = dirExistence(dirTree, dirName, 'd');
    if (!dirNode) {
        dirNode = dirExistence(dirTree, dirName, 'f');
        if (!dirNode) {
            printf("chown: %s: No such file or directory\n", dirName);
            return FAIL;
        }
    }

    // 3) 해당 노드의 UID/GID 변경
    dirNode->id.UID = usr->id.UID;
    dirNode->id.GID = usr->id.GID;

    // 4) -R(flag==1) 옵션이면 하위 트리 전체에 적용
    if (flag == 1 && dirNode->firstChild) {
        changeOwnerAll(dirNode->firstChild, usr);
    }

    return SUCCESS;
}

/**
 * @brief 디렉토리 하위 전체 노드에 대해 소유자 재귀 변경
 */
void changeOwnerAll(DirectoryNode* node, UserNode* usr)
{
    // 현재 노드 UID/GID 설정
    node->id.UID = usr->id.UID;
    node->id.GID = usr->id.GID;

    // 자식 노드 재귀
    if (node->firstChild) {
        changeOwnerAll(node->firstChild, usr);
    }
    // 형제 노드 재귀
    if (node->nextSibling) {
        changeOwnerAll(node->nextSibling, usr);
    }
}

/**
 * @brief 입력된 명령어 문자열을 파싱해 changeOwner 호출
 */
int ft_chown(DirectoryTree* dirTree,
    char* cmd)
{
    char* token = strtok(cmd, " ");
    int recursive = 0;
    char userName[MAX_NAME];
    char targetName[MAX_DIR];

    // 첫 토큰은 "chown"
    if (!token) {
        printf("Usage: chown [-R] user file\n");
        return FAIL;
    }

    // 옵션 처리
    token = strtok(NULL, " ");
    if (token && strcmp(token, "-R") == 0) {
        recursive = 1;
        token = strtok(NULL, " ");
    }

    // 사용자 이름
    if (!token) {
        printf("chown: missing user operand\n");
        return FAIL;
    }
    strncpy(userName, token, MAX_NAME - 1);
    userName[MAX_NAME - 1] = '\0';

    // 대상 파일/디렉토리
    token = strtok(NULL, " ");
    if (!token) {
        printf("chown: missing file operand\n");
        return FAIL;
    }
    strncpy(targetName, token, MAX_DIR - 1);
    targetName[MAX_DIR - 1] = '\0';

    // 실제 소유자 변경 수행
    return changeOwner(dirTree, userName, targetName, recursive);
}
