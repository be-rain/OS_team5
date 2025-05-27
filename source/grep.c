#include "../include/main.h"

int printContent(DirectoryTree *dirtree, char *search, char *fileName, int option) {
    DirectoryNode *tmpNode = NULL;
    FILE *fp;
    char buf[MAX_BUFFER];
    int count = 1;

    tmpNode = dirExistence(dirtree, fileName, 'f');
    if (!tmpNode) return FAIL;
    fp = fopen(fileName, "r");
    while (!feof(fp)) {
        fgets(buf, sizeof(buf), fp);
        if (feof(fp)) break;
        else if (option == 0) {
            if (strstr(buf, search)) printf("%s", buf);
        } else if (option == 1) {
            if (strstr(buf, search)) printf("%d:%s", count, buf);
        } else if (option == 2) {
            if (!strstr(buf, search)) printf("%s", buf);
        } else if (option == 3) {
            if (strcasestr(buf, search)) printf("%s", buf);
        } else if (option == 4) {
            if (!strstr(buf, search)) printf("%d:%s", count, buf);
        } else if (option == 5) {
            if (strcasestr(buf, search)) printf("%d:%s", count, buf);
        } else if (option == 6) {
            if (!strcasestr(buf, search)) printf("%s", buf);
        } else if (option == 7) {
            if (!strcasestr(buf, search)) printf("%d:%s", count, buf);
        }
        count++;
    }
    fclose(fp);
    return SUCCESS;
}

int ft_grep(DirectoryTree *dirTree, char *command) {
    DirectoryNode *currentNode = dirTree->current;
    DirectoryNode *tmpNode = NULL;
    DirectoryNode *tmpNode2 = NULL;
    ThreadTree threadTree[MAX_THREAD];
    pthread_t threadPool[MAX_THREAD];
    int threadCount = 0;
    char *str;
    char content[MAX_BUFFER];
    char tmp[MAX_DIR];
    char tmp2[MAX_DIR];
    char tmp3[MAX_DIR];
    int value;

    if (!command) {
        printf("Try 'grep --help' for more information.\n");
        return FAIL;
    }
    if (command[0] == '-') {
        if (!strcmp(command, "-n")) {
            str = strtok(NULL, " ");
            if (!str) return FAIL;
            strncpy(content, str, MAX_BUFFER);
            str = strtok(NULL, " ");
            if (!str) {
                printf("Try 'grep --help' for more information.\n");
                return FAIL;
            }
            while (str) {
                threadTree[threadCount].threadTree = dirTree;
                threadTree[threadCount].option = 1;
                threadTree[threadCount].content = content;
                threadTree[threadCount++].command = str;
                str = strtok(NULL, " ");
            }
        } else if (!strcmp(command, "-v")) {
            str = strtok(NULL, " ");
            if (!str) return FAIL;
            strncpy(content, str, MAX_BUFFER);
            str = strtok(NULL, " ");
            if (!str) {
                printf("Try 'grep --help' for more information.\n");
                return FAIL;
            }
            while (str) {
                threadTree[threadCount].threadTree = dirTree;
                threadTree[threadCount].option = 2;
                threadTree[threadCount].content = content;
                threadTree[threadCount++].command = str;
                str = strtok(NULL, " ");
            }
        } else if (!strcmp(command, "-i")) {
            str = strtok(NULL, " ");
            if (!str) return FAIL;
            strncpy(content, str, MAX_BUFFER);
            str = strtok(NULL, " ");
            if (!str) {
                printf("Try 'grep --help' for more information.\n");
                return FAIL;
            }
            while (str) {
                threadTree[threadCount].threadTree = dirTree;
                threadTree[threadCount].option = 3;
                threadTree[threadCount].content = content;
                threadTree[threadCount++].command = str;
                str = strtok(NULL, " ");
            }
        } else if (!strcmp(command, "-nv") || !strcmp(command, "-vn")) {
            str = strtok(NULL, " ");
            if (!str) return FAIL;
            strncpy(content, str, MAX_BUFFER);
            str = strtok(NULL, " ");
            if (!str) {
                printf("Try 'grep --help' for more information.\n");
                return FAIL;
            }
            while (str) {
                threadTree[threadCount].threadTree = dirTree;
                threadTree[threadCount].option = 4;
                threadTree[threadCount].content = content;
                threadTree[threadCount++].command = str;
                str = strtok(NULL, " ");
            }
        } else if (!strcmp(command, "-ni") || !strcmp(command, "-in")) {
            str = strtok(NULL, " ");
            if (!str) return FAIL;
            strncpy(content, str, MAX_BUFFER);
            str = strtok(NULL, " ");
            if (!str) {
                printf("Try 'grep --help' for more information.\n");
                return FAIL;
            }
            while (str) {
                threadTree[threadCount].threadTree = dirTree;
                threadTree[threadCount].option = 5;
                threadTree[threadCount].content = content;
                threadTree[threadCount++].command = str;
                str = strtok(NULL, " ");
            }
        } else if(!strcmp(command, "-vi") || !strcmp(command, "-iv")) {
            str = strtok(NULL, " ");
            if (!str) return FAIL;
            strncpy(content, str, MAX_BUFFER);
            str = strtok(NULL, " ");
            if (!str) {
                printf("Try 'grep --help' for more information.\n");
                return FAIL;
            }
            while (str) {
                threadTree[threadCount].threadTree = dirTree;
                threadTree[threadCount].option = 6;
                threadTree[threadCount].content = content;
                threadTree[threadCount++].command = str;
                str = strtok(NULL, " ");
            }
        } else if (!strcmp(command, "-inv") || !strcmp(command, "-ivn") || !strcmp(command, "-niv") || !strcmp(command, "-nvi") || !strcmp(command, "-vin") || !strcmp(command, "-vni")) {
            str = strtok(NULL, " ");
            if (!str) return FAIL;
            strncpy(content, str, MAX_BUFFER);
            str = strtok(NULL, " ");
            if (!str) {
                printf("Try 'grep --help' for more information.\n");
                return FAIL;
            }
            while (str) {
                threadTree[threadCount].threadTree = dirTree;
                threadTree[threadCount].option = 7;
                threadTree[threadCount].content = content;
                threadTree[threadCount++].command = str;
                str = strtok(NULL, " ");
            }
        } else if (!strcmp(command, "--help")) {
            printf("Usage: grep [<option>]... [<file>]...\n");
            printf("  Search for PATTERNS in each FILE.\n\n");
            printf("  Options:\n");
            printf("    -n, —number         \t number all output line\n");
            printf("    -i, --ignore-case         \t ignore case distinctions in patterns and data\n");
            printf("    -v, --invert-match         \t select non-matching lines\n");
            printf("      --help                display this help text and exit\n");
            return FAIL;
        } else {
            printf("Try 'grep --help' for more information.\n");
            return FAIL;
        }
    } else {
        strncpy(content, command, MAX_BUFFER);
        str = strtok(NULL, " ");
        if (!str) {
            printf("Try 'grep --help' for more information.\n");
            return FAIL;
        }
        while (str) {
            threadTree[threadCount].threadTree = dirTree;
            threadTree[threadCount].option = 0;
            threadTree[threadCount].content = content;
            threadTree[threadCount++].command = str;
            str = strtok(NULL, " ");
        }
    }
    for (int i = 0; i < threadCount; i++) {
        pthread_create(&threadPool[i], NULL, grepUsedThread, (void *)&threadTree[i]);
        pthread_join(threadPool[i], NULL);
    }
    return SUCCESS;
} 