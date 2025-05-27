#include "../include/main.h"

int ft_touch(DirectoryTree *dirTree, char *command) {
    DirectoryNode *currentNode = dirTree->current;
    ThreadTree threadTree[MAX_THREAD];
    pthread_t threadPool[MAX_THREAD];
    int threadCount = 0;
    char *str;
    int value;
    int mode;

    if (!command) {
        printf("touch: touch: Invalid option\n");
        printf("Try 'touch --help' for more information.\n");
        return FAIL;
    }
    if (command[0] == '-') {
        if (!strcmp(command, "--help")) {
            printf("Usage: touch [OPTION]... FILE...\n");
            printf("Update the access and modification times of each FILE to the current time.\n\n\n");
            printf("A FILE argument string of - is handled specially and causes touch to\n");
            printf("change the times of the file associated with standard output.\n\n");
            printf("Mandatory arguments to long options are mandatory for short options too.\n");
            printf("      --help     display this help and exit");
        } else {
            str = strtok(command, "-");
            if (!str) {
                printf("touch: Invalid option\n");
                printf("Try 'touch --help' for more information.\n");
                return FAIL;
            } else {
                printf("touch: invalid option -- '%s'\n", str);
                printf("Try 'touch --help' for more information.\n");
                return FAIL;
            }
        }
    } else {
        if (!command) {
            printf("touch: missing file operand\n");
            printf("Try 'touch --help' for more information.\n");
            return FAIL;
        }
        threadTree[threadCount].threadTree = dirTree;
        threadTree[threadCount++].command = command;
        str = strtok(NULL, " ");
        while (str) {
            threadTree[threadCount].threadTree = dirTree;
            threadTree[threadCount++].command = str;
            str = strtok(NULL, " ");
        }
    }
    for (int i = 0; i < threadCount; i++) {
        pthread_create(&threadPool[i], NULL, touchUsedThread, (void *)&threadTree[i]);
        pthread_join(threadPool[i], NULL);
    }
    return SUCCESS;
}