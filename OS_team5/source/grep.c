#include "../include/main.h"

#define OPT_I (1 << 0) // -i: 대소문자 구분 안함
#define OPT_V (1 << 1) // -v: 반전 매칭
#define OPT_C (1 << 2) // -c: 매칭 라인 수만 출력
#define OPT_L (1 << 3) // -l: 매칭된 파일명만 출력
#define OPT_N (1 << 4) // -n: 라인 번호 출력
#define OPT_H (1 << 5) // -h: 파일명 출력 안함 (멀티 파일 시 기본 출력됨)
#define OPT_S (1 << 6) // -s: 에러 메시지 억제

// 대소문자 구분 없이 문자열 검색 (strcasestr 간단 구현)
static char *strcasestr_simple(const char *haystack, const char *needle) {
    if (!*needle) return (char *)haystack;
    for (; *haystack; haystack++) {
        if (tolower((unsigned char)*haystack) == tolower((unsigned char)*needle)) {
            const char *h = haystack + 1;
            const char *n = needle + 1;
            while (*n && *h && tolower((unsigned char)*h) == tolower((unsigned char)*n)) {
                h++; n++;
            }
            if (!*n) return (char *)haystack;
        }
    }
    return NULL;
}

// 파일 내용에서 패턴 검색 및 출력
int printContent(DirectoryTree *dirtree, char *pattern, char *fileName, unsigned int options) {
    DirectoryNode *node = dirExistence(dirtree, fileName, 'f');
    if (!node) {
        if (!(options & OPT_S))
            printf("grep: %s: No such file or directory\n", fileName);
        return FAIL;
    }

    char *content = node->data;
    if (!content || strlen(content) == 0) return SUCCESS;

    char buf[LENGTH_SIZE];
    strncpy(buf, content, LENGTH_SIZE);
    buf[LENGTH_SIZE - 1] = '\0';

    char *line = strtok(buf, "\n");
    int lineNo = 1;
    int matchCount = 0;
    int matched = FALSE;

    while (line) {
        char *found = (options & OPT_I) ? strcasestr_simple(line, pattern) : strstr(line, pattern);
        int isMatch = (found != NULL);
        if (options & OPT_V) isMatch = !isMatch;

        if (isMatch) {
            matchCount++;
            matched = TRUE;
            if (!(options & OPT_C) && !(options & OPT_L)) {
                if (!(options & OPT_H)) printf("%s:", fileName);
                if (options & OPT_N) printf("%d:", lineNo);
                printf("%s\n", line);
            }
        }
        line = strtok(NULL, "\n");
        lineNo++;
    }

    if (options & OPT_C) {
        if (!(options & OPT_H)) printf("%s:", fileName);
        printf("%d\n", matchCount);
    }

    if ((options & OPT_L) && matched) {
        printf("%s\n", fileName);
    }

    return SUCCESS;
}

// 옵션 문자열 파싱 함수 (지원 옵션만 처리)
static unsigned int parse_grep_options(const char *optStr) {
    unsigned int flags = 0;
    for (int i = 1; optStr[i]; i++) {
        switch (optStr[i]) {
            case 'i': flags |= OPT_I; break;
            case 'v': flags |= OPT_V; break;
            case 'c': flags |= OPT_C; break;
            case 'l': flags |= OPT_L; break;
            case 'n': flags |= OPT_N; break;
            case 'h': flags |= OPT_H; break;
            case 's': flags |= OPT_S; break;
            default:
                printf("grep: invalid option -- '%c'\n", optStr[i]);
                break;
        }
    }
    return flags;
}

// 명령어 파싱 및 grep 실행
int ft_grep(DirectoryTree *dirTree, char *cmd) {
    unsigned int options = 0;
    char *pattern = NULL;
    char *fileName = NULL;

    // 앞 공백 제거
    while (*cmd == ' ' || *cmd == '\t') cmd++;

    // 옵션 처리
    while (*cmd == '-') {
        char *nextSpace = strchr(cmd, ' ');
        int len = nextSpace ? (int)(nextSpace - cmd) : (int)strlen(cmd);

        char tempOpt[20];
        if (len >= 20) len = 19;
        strncpy(tempOpt, cmd, len);
        tempOpt[len] = '\0';

        options |= parse_grep_options(tempOpt);
        cmd += len;

        while (*cmd == ' ' || *cmd == '\t') cmd++;
    }

    // 패턴 추출
    if (!*cmd) {
        printf("grep: missing pattern\n");
        return FAIL;
    }

    pattern = cmd;

    // 패턴과 파일명 분리
    char *space = strchr(cmd, ' ');
    if (space) {
        *space = '\0';
        fileName = space + 1;
        while (*fileName == ' ' || *fileName == '\t') fileName++;
        if (*fileName == '\0') fileName = NULL;
    }

    if (!fileName) {
        printf("grep: missing file operand\n");
        return FAIL;
    }

    DirectoryNode *dirNode = findDirectoryByPath(dirTree, fileName);
    if (dirNode && dirNode->type == 'd') {
        printf("grep: %s: Is a directory\n", fileName);
        return FAIL;
    }

    return printContent(dirTree, pattern, fileName, options);
}
