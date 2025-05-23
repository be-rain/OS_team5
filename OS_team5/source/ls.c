#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/main.h"

#define MAX_FILES 1024

// 옵션 플래그 정의
#define OPT_A  (1 << 0)
#define OPT_H  (1 << 1)
#define OPT_L  (1 << 2)
#define OPT_R  (1 << 3)
#define OPT_S  (1 << 4)
#define OPT_T  (1 << 5)
#define OPT_CAP_A (1 << 6)
#define OPT_B  (1 << 7)
#define OPT_I  (1 << 8)
#define OPT_CAP_I (1 << 9)
#define OPT_CAP_L (1 << 10)
#define OPT_M  (1 << 11)
#define OPT_N  (1 << 12)
#define OPT_CAP_R (1 << 13)

// 옵션 파싱 함수
static unsigned int parse_ls_options(const char *optStr) {
    unsigned int flags = 0;
    for (int i = 1; optStr[i]; i++) {
        switch (optStr[i]) {
            case 'a': flags |= OPT_A; break;
            case 'h': flags |= OPT_H; break;
            case 'l': flags |= OPT_L; break;
            case 'r': flags |= OPT_R; break;
            case 'S': flags |= OPT_S; break;
            case 't': flags |= OPT_T; break;
            case 'A': flags |= OPT_CAP_A; break;
            case 'b': flags |= OPT_B; break;
            case 'i': flags |= OPT_I; break;
            case 'I': flags |= OPT_CAP_I; break;
            case 'L': flags |= OPT_CAP_L; break;
            case 'm': flags |= OPT_M; break;
            case 'n': flags |= OPT_N; break;
            case 'R': flags |= OPT_CAP_R; break;
            default:
                printf("ls: invalid option -- '%c'\n", optStr[i]);
                // 오류 처리 시 리턴값 조정 가능
                break;
        }
    }
    return flags;
}

// 절대경로 및 상대경로 모두 처리하는 경로 탐색 함수
DirectoryNode* findDirectoryByPath(DirectoryTree *dirTree, const char *path) {
    if (path == NULL || strlen(path) == 0) {
        return dirTree->current;
    }

    DirectoryNode *startNode;

    if (path[0] == '/') {
        // 절대경로 시작
        startNode = dirTree->root;
        path++; // '/' 건너뛰기
    } else {
        startNode = dirTree->current;
    }

    char tempPath[MAX_BUFFER];
    strncpy(tempPath, path, MAX_BUFFER);
    tempPath[MAX_BUFFER - 1] = '\0';

    char *token = strtok(tempPath, "/");
    DirectoryNode *curr = startNode;

    while (token && curr) {
        if (strcmp(token, ".") == 0) {
            // 현재 디렉토리 그대로
        } else if (strcmp(token, "..") == 0) {
            if (curr->parent) curr = curr->parent;
        } else {
            DirectoryNode *child = curr->firstChild;
            int found = 0;
            while (child) {
                if (strcmp(child->name, token) == 0 && child->type == 'd') {
                    curr = child;
                    found = 1;
                    break;
                }
                child = child->nextSibling;
            }
            if (!found) return NULL;
        }
        token = strtok(NULL, "/");
    }

    return curr;
}

// 정렬 비교 함수들
static int cmpByName(const void *a, const void *b) {
    DirectoryNode *d1 = *(DirectoryNode **)a;
    DirectoryNode *d2 = *(DirectoryNode **)b;
    return strcmp(d1->name, d2->name);
}

static int cmpBySize(const void *a, const void *b) {
    DirectoryNode *d1 = *(DirectoryNode **)a;
    DirectoryNode *d2 = *(DirectoryNode **)b;
    return d2->SIZE - d1->SIZE; // 큰게 앞에
}

static int cmpByTime(const void *a, const void *b) {
    DirectoryNode *d1 = *(DirectoryNode **)a;
    DirectoryNode *d2 = *(DirectoryNode **)b;

    // 년월일시분초 순으로 비교 (최신 순)
    if (d2->date.year != d1->date.year) return d2->date.year - d1->date.year;
    if (d2->date.month != d1->date.month) return d2->date.month - d1->date.month;
    if (d2->date.day != d1->date.day) return d2->date.day - d1->date.day;
    if (d2->date.hour != d1->date.hour) return d2->date.hour - d1->date.hour;
    if (d2->date.minute != d1->date.minute) return d2->date.minute - d1->date.minute;
    return d2->date.second - d1->date.second;
}

static int reverseCmp(const void *a, const void *b, int (*cmp)(const void*, const void*)) {
    return -cmp(a,b);
}

// 파일 이름 출력 (옵션 b 등 고려해서 필요 시 수정)
static void printFileName(DirectoryNode *file, unsigned int flags) {
    // 특수문자 이스케이프(b), inode(i), 숫자(n) 등 출력은 필요하면 구현
    printf("%s", file->name);
}

// 권한 문자열 출력 (main.h에 선언된 printPermission 함수 활용 가능)
static void printLongFormat(DirectoryNode *file, unsigned int flags) {
    printPermission(file);

    if (flags & OPT_I) {
        // inode 출력 (UID 대신 ID로 임시 표시)
        printf(" %d", file->id.UID);
    }

    if (flags & OPT_N) {
        // 숫자 UID GID 출력
        printf(" %d %d", file->id.UID, file->id.GID);
    } else {
        // 이름 출력 (추가로 userName, groupName 출력 함수 필요)
        // 예: printf(" %s %s", getUserName(file->id.UID), getGroupName(file->id.GID));
        printf(" %d %d", file->id.UID, file->id.GID); // 임시
    }

    printf(" %5d ", file->SIZE);

    printf("%04d-%02d-%02d %02d:%02d ", 
        file->date.year, file->date.month, file->date.day, file->date.hour, file->date.minute);

    printFileName(file, flags);
    printf("\n");
}

// 디렉토리 내용 출력 함수, 옵션에 따라 처리, 재귀 지원
int listDir(DirectoryNode *targetDir, unsigned int flags) {
    pthread_mutex_lock(&dirTreeMutex); // entry section

    if (!targetDir) {
        printf("listDir: target directory is NULL\n");
        return FAIL;
    }

    DirectoryNode *files[MAX_FILES];
    int count = 0;
    DirectoryNode *curr = targetDir->firstChild;

    while (curr && count < MAX_FILES) {
        int skip = 0;

        // 숨김파일 처리
        if (!(flags & OPT_A)) {
            if (curr->name[0] == '.') {
                if ((flags & OPT_CAP_A) == 0) skip = 1; // -A는 . .. 제외한 숨김파일 포함
                else if (strcmp(curr->name, ".") == 0 || strcmp(curr->name, "..") == 0) skip = 1;
            }
        }

        if (!skip) files[count++] = curr;
        curr = curr->nextSibling;
    }

    // 정렬 기준 결정
    int (*cmpFunc)(const void*, const void*) = cmpByName;
    if (flags & OPT_S) cmpFunc = cmpBySize;
    else if (flags & OPT_T) cmpFunc = cmpByTime;

    // 정렬 실행
    qsort(files, count, sizeof(DirectoryNode*), cmpFunc);

    // 역순 옵션 처리
    if (flags & OPT_R) {
        // 역순 출력
        for (int i = count - 1; i >= 0; i--) {
            if (flags & OPT_L)
                printLongFormat(files[i], flags);
            else {
                printFileName(files[i], flags);
                if (!(flags & OPT_M)) printf("\n");
                else printf(", ");
            }
        }
        if (!(flags & OPT_L) && !(flags & OPT_M)) printf("\n");
        else if (flags & OPT_M) printf("\n");
    } else {
        // 정상 순서 출력
        for (int i = 0; i < count; i++) {
            if (flags & OPT_L)
                printLongFormat(files[i], flags);
            else {
                printFileName(files[i], flags);
                if (!(flags & OPT_M)) printf("\n");
                else printf(", ");
            }
        }
        if (!(flags & OPT_L) && !(flags & OPT_M)) printf("\n");
        else if (flags & OPT_M) printf("\n");
    }

    // 재귀 출력 -R 옵션
    if (flags & OPT_CAP_R) {
        for (int i = 0; i < count; i++) {
            if (files[i]->type == 'd' && 
                strcmp(files[i]->name, ".") != 0 && strcmp(files[i]->name, "..") != 0) {
                printf("\n%s:\n", files[i]->name);
                listDir(files[i], flags);
            }
        }
    }

    pthread_mutex_unlock(&dirTreeMutex); // exit section
    return SUCCESS;
}

int ft_ls(DirectoryTree *dirTree, char *cmd) {
    if (!dirTree || !cmd) return FAIL;

    unsigned int flags = 0;
    char targetPath[MAX_BUFFER] = "";

    // 명령어 파싱
    char cmdCopy[MAX_BUFFER];
    strncpy(cmdCopy, cmd, MAX_BUFFER);
    cmdCopy[MAX_BUFFER - 1] = '\0';

    char *token = strtok(cmdCopy, " ");
    token = strtok(NULL, " "); // "ls" 건너뛰기

    while (token) {
        if (token[0] == '-') {
            flags |= parse_ls_options(token);
        } else {
            strncpy(targetPath, token, MAX_BUFFER);
            targetPath[MAX_BUFFER - 1] = '\0';
        }
        token = strtok(NULL, " ");
    }

    DirectoryNode *targetDir = NULL;
    if (strlen(targetPath) == 0) {
        targetDir = dirTree->current;
    } else {
        targetDir = findDirectoryByPath(dirTree, targetPath);
        if (!targetDir) {
            printf("ls: cannot access '%s': No such directory\n", targetPath);
            return FAIL;
        }
    }

    return listDir(targetDir, flags);
}
