#include "../include/main.h"

int changeFileMode(DirectoryNode *node, int mode) {
    if (checkPermission(node, 'w')) {
        printf("chmod: Can not modify file '%s': Permission denied\n", node->name);
        return FAIL;
    }
    node->permission.mode = mode;
    modeToPermission(node);
    return SUCCESS;
}

int changeDirectoryMode(DirectoryNode *node, int mode) {
    if (checkPermission(node, 'w')) {
        printf("chmod: Can not modify directory '%s': Permission denied\n", node->name);
        return FAIL;
    }
    node->permission.mode = mode;
    modeToPermission(node);
    return SUCCESS;
}

int ft_chmod(DirectoryTree *dirTree, char *command) {
    ThreadTree threadTree[MAX_THREAD];
    pthread_t threadPool[MAX_THREAD];
    int threadCount = 0;
    char *str;
    int mode;

    if (!command) {
        printf("chmod: Invalid option\n");
        printf("Try 'chmod --help' for more information.\n");
        return FAIL;
    }
    if (command[0] == '-') {
        if (!strcmp(command, "--help")) {
            printf("Usage: chmod [OPTION]... OCTAL-MODE FILE...\n");
            printf("Change the mode of each FILE to MODE.\n\n");
            printf("  Options:\n");
            printf("  -R, --recursive        change files and directories recursively\n");
            printf("      --help     Display this help and exit\n");
            return FAIL;
        } else {
            str = strtok(command, "-");
            if (!str) {
                printf("chmod: Invalid option\n");
                printf("Try 'chmod --help' for more information.\n");
                return FAIL;
            } else {
                printf("chmod: Unrecognized option -- '%s'\n", str);
                printf("Try 'chmod --help' for more information.\n");
                return FAIL;
            }
        }
    } else {
        if (command[0] - '0' < 8 && command[1] - '0' < 8 && command[2] - '0' < 8 && strlen(command) == 3) {
            mode = atoi(command);
            str = strtok(NULL, " ");
            if (!str) {
                printf("chmod: Invalid option\n");
                printf("Try 'chmod --help' for more information.\n");
                return FAIL;
            }
            threadTree[threadCount].threadTree = dirTree;
            threadTree[threadCount].command = str;
            threadTree[threadCount++].mode = mode;
            while (str) {
                threadTree[threadCount].threadTree = dirTree;
                threadTree[threadCount].command = str;
                threadTree[threadCount++].mode = mode;
                str = strtok(NULL, " ");
            }
        } else {
            printf("chmod: Invalid Mode: '%s'\n", command);
            printf("Try 'chmod --help' for more information.\n");
            return FAIL;
        }
    }

    for (int i = 0; i < threadCount; i++) {
        pthread_create(&threadPool[i], NULL, chmodUsedThread, (void *)&threadTree[i]);
        pthread_join(threadPool[i], NULL);
    }

    return SUCCESS;
}
