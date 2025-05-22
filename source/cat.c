#include "main.h"
#include "cat.h"

/*
 * concatenate: 가상 파일 시스템(DirectoryTree) 내에서 파일 읽기/쓰기 수행
 * @param dirTree 포인터 to 디렉터리 트리
 * @param fName   파일 이름/경로
 * @param type    0=쓰기, 1=전체 읽기, 2=줄 번호 출력, 3=빈 줄 건너뛰기
 */
int concatenate(DirectoryTree* dirTree, char* fName, int type) {
    DirectoryNode* saved = dirTree->current;
    DirectoryNode* node = NULL;
    char pathBuf[MAX_DIR];
    char* dirPath;
    char* fname;
    int status = SUCCESS;

    /* 1) 경로/파일명 분리 */
    strncpy(pathBuf, fName, MAX_DIR);
    pathBuf[MAX_DIR - 1] = '\0';
    dirPath = getDir(pathBuf);
    fname = pathBuf + strlen(dirPath);
    while (*fname == '/' || *fname == '\\') fname++;

    /* 2) 디렉터리 이동 */
    if (changePath(dirTree, dirPath) != SUCCESS) {
        printf("cat: cannot change directory to '%s'\n", dirPath);
        status = FAIL;
        goto restore;
    }

    /* 3) 파일 존재 여부 */
    node = dirExistence(dirTree, fname, 'f');
    if (!node) {
        if (type == 0) {
            /* 쓰기 모드에서만 파일 생성 */
            if (makeDir(dirTree, fname, 'f') != SUCCESS) {
                printf("cat: failed to create file '%s'\n", fname);
                status = FAIL;
                goto restore;
            }
            node = dirExistence(dirTree, fname, 'f');
            if (!node) {
                printf("cat: file '%s' not found after creation\n", fname);
                status = FAIL;
                goto restore;
            }
        }
        else {
            printf("cat: '%s' does not exist\n", fname);
            status = FAIL;
            goto restore;
        }
    }

    /* 4) 쓰기 모드 */
    if (type == 0) {
        /* 파일 내용 버퍼 초기화 */
        node->data[0] = '\0';
        char lineBuf[MAX_BUFFER];
        printf("Enter file content. End with a line containing only 'end'.\n");
        while (fgets(lineBuf, MAX_BUFFER, stdin)) {
            if (strcmp(lineBuf, "end\n") == 0 || strcmp(lineBuf, "end\r\n") == 0)
                break;
            /* 버퍼 오버플로우 방지 */
            if ((int)(strlen(node->data) + strlen(lineBuf)) < LENGTH_SIZE) {
                strcat(node->data, lineBuf);
            }
        }
    }
    /* 5) 읽기 모드 */
    else {
        char buf[LENGTH_SIZE];
        strncpy(buf, node->data, LENGTH_SIZE);
        buf[LENGTH_SIZE - 1] = '\0';
        char* tok = strtok(buf, "\n");
        int lineNo = 1;
        while (tok) {
            int isBlank = (*tok == '\0');
            if (type == 1 || (type == 2) || (type == 3 && !isBlank)) {
                if (type == 2) {
                    printf("%6d  %s\n", lineNo++, tok);
                }
                else {
                    printf("%s\n", tok);
                }
            }
            tok = strtok(NULL, "\n");
        }
    }

restore:
    /* 6) 원위치 복원 */
    dirTree->current = saved;
    return status;
}

/*
 * ft_cat: 쉘에서 넘어온 커맨드(cmd) 파싱 후 concatenate 호출
 */
int ft_cat(DirectoryTree* dirTree, char* cmd) {
    char* arg = cmd;
    int mode = 1; /* 기본 읽기 모드 */

    /* 공백 제거 */
    while (*arg == ' ' || *arg == '\t') arg++;

    /* 쓰기 리디렉션('>') 처리 */
    if (*arg == '>') {
        mode = 0;
        arg++;
    }
    /* 옵션 처리 */
    else if (*arg == '-') {
        if (strncmp(arg, "-n", 2) == 0) {
            mode = 2;
            arg += 2;
        }
        else if (strncmp(arg, "-b", 2) == 0) {
            mode = 3;
            arg += 2;
        }
        else {
            printf("cat: invalid option '%s'\n", arg);
            printf("Try 'cat --help' for more information.\n");
            return FAIL;
        }
    }

    /* 파일명 앞 공백 제거 */
    while (*arg == ' ' || *arg == '\t') arg++;
    if (!*arg) {
        printf("cat: missing file operand\n");
        return FAIL;
    }

    return concatenate(dirTree, arg, mode);
}
