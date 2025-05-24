/* chown.h */
#ifndef CHOWN_H
#define CHOWN_H

#pragma once
#pragma warning(disable:4996)

#include "main.h"

/**
 * @brief 파일 또는 디렉토리 소유자 변경
 * @param dirTree        현재 디렉토리 트리 포인터
 * @param userName       변경할 사용자 이름
 * @param nodeName       대상 파일/디렉토리 이름
 * @param recursiveFlag  하위 디렉토리 재귀 적용 여부 (0: 단일, 1: 재귀)
 * @return SUCCESS(0) 또는 FAIL(-1)
 */
int changeOwner(DirectoryTree* dirTree,
    char* userName,
    char* nodeName,
    int recursiveFlag);

/**
 * @brief 디렉토리 노드 하위 항목 전체 소유자 재귀 변경
 * @param node      시작 디렉토리 노드
 * @param userName  변경할 사용자 이름
 */
void changeOwnerAll(DirectoryNode* node,
    char* userName);

/**
 * @brief chown 명령어 파싱 및 실행
 * @param dirTree  현재 디렉토리 트리 포인터
 * @param cmd      전체 명령어 문자열 (예: "chown -R alice /path")
 * @return SUCCESS(0) 또는 FAIL(-1)
 */
int ft_chown(DirectoryTree* dirTree,
    char* cmd);

#endif // CHOWN_H
