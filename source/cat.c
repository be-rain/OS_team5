#include "../include/main.h"

int concatenate(DirectoryTree *dirTree, char *fileName, int type) {
    if (type == 4) {
        UserNode *user = usrList->head;
        while (user) {
            printf("%s:x:%d:%d:%s:%s\n", user->name, user->id.UID, user->id.GID, user->name, user->dir);
            user = user->nextNode;
        }
        return SUCCESS;
    }

    DirectoryNode *dirNode = dirExistence(dirTree, fileName, 'f');
    if (!dirNode)
        return FAIL;

    FILE *file = fopen(fileName, "r");
    char buffer[MAX_BUFFER];
    int cnt = 1;

    while (fgets(buffer, sizeof(buffer), file)) {
        if (type == 2) {
            if (buffer[strlen(buffer) - 1] == '\n') {
                printf("     %d ", cnt);
                cnt++;
            }
        } else if (type == 3) {
            if (buffer[strlen(buffer) - 1] == '\n' && buffer[0] != '\n') {
                printf("     %d ", cnt);
                cnt++;
            }
        }
        fputs(buffer, stdout);
    }

    fclose(file);
    return SUCCESS;
}

int createFile(DirectoryTree *dirTree, char *fileName, char *filePath) {
    FILE *file = fopen(fileName, "w");
    char buffer[MAX_BUFFER];
    int tmpSize = 0;

    while (fgets(buffer, sizeof(buffer), stdin)) {
        fputs(buffer, file);
        tmpSize += strlen(buffer) - 1;
    }

    rewind(stdin);
    fclose(file);

    DirectoryNode *dirNode = dirExistence(dirTree, fileName, 'f');
    if (dirNode) {
        time(&ltime);
        today = localtime(&ltime);

        dirNode->date.month = today->tm_mon + 1;
        dirNode->date.day = today->tm_mday;
        dirNode->date.hour = today->tm_hour;
        dirNode->date.minute = today->tm_min;
    } else {
        makeDir(dirTree, fileName, 'f');
    }

    dirNode = dirExistence(dirTree, fileName, 'f');
    dirNode->SIZE = tmpSize;

    return SUCCESS;
}

int ft_cat(DirectoryTree *dirTree, char *command) {
    DirectoryNode *currentNode = dirTree->current;
    DirectoryNode *dirNode = NULL;
    DirectoryNode *dirNode2 = NULL;
    ThreadTree threadTree[MAX_THREAD];
    pthread_t threadPool[MAX_THREAD];
    int threadCount = 0;
    char *str;
    char tmp[MAX_DIR];
    char tmp2[MAX_DIR];
    char tmp3[MAX_DIR];
    int value;

    if (!command) {
        long size;
        unsigned char c;
        size = read(0, &c, 1);
        while (size) {
            write(1, &c, size);
            size = read(0, &c, 1);
        }
        rewind(stdin);
        return SUCCESS;
    }

    if (strcmp(command, ">") == 0) {
        str = strtok(NULL, " ");
        if (str == NULL) {
            printf("cat: Invalid option\n");
            printf("Try 'cat --help' for more information.\n");
            return FAIL;
        }
        strncpy(tmp, str, MAX_DIR);
        if (!strstr(str, "/")) {
            if (checkPermission(dirTree->current, 'w')) {
                printf("cat: Can not create file '%s': No permission.\n", dirTree->current->name);
                return FAIL;
            }
            dirNode = dirExistence(dirTree, str, 'd');
            if (dirNode) {
                printf("cat: '%s': Is a directory\n", str);
                return FAIL;
            } else {
                createFile(dirTree, str, NULL);
            }
        } else {
            strncpy(tmp2, getDir(str), MAX_DIR);
            value = changePath(dirTree, tmp2);
            if (value) {
                printf("cat: '%s': No such file or directory\n", tmp2);
                return FAIL;
            }
            str = strtok(tmp, "/");
            while (str) {
                strncpy(tmp3, str, MAX_NAME);
                str = strtok(NULL, "/");
            }
            if (checkPermission(dirTree->current, 'w')) {
                printf("cat: Can not create file '%s': No permission.\n", dirTree->current->name);
                dirTree->current = currentNode;
                return FAIL;
            }
            dirNode = dirExistence(dirTree, tmp3, 'd');
            if (dirNode) {
                printf("cat: '%s': Is a directory\n", tmp3);
                dirTree->current = currentNode;
                return FAIL;
            } else {
                createFile(dirTree, tmp3, NULL);
            }
            dirTree->current = currentNode;
        }
        return 0;
    } else if (command[0] == '-') {
        if (!strcmp(command, "-n")) {
            str = strtok(NULL, " ");
            if (!str) {
                printf("cat: Invalid option\n");
                printf("Try 'cat --help' for more information.\n");
                return FAIL;
            }
            while (str) {
                threadTree[threadCount].threadTree = dirTree;
                threadTree[threadCount].option = 1;
                threadTree[threadCount++].command = str;
                str = strtok(NULL, " ");
            }
        } else if (strcmp(command, "--help") == 0) {
            printf("Usage: cat [OPTION]... [FILE]...\n");
            printf("Concatenate FILE(s) to standard output.\n\n");
            printf("With no FILE, or when FILE is -, read standard input.\n\n");
            printf("  -n, --number             number all output lines\n");
            printf("  -b, --number-nonblank    number nonempty output lines, overrides -n\n");
            printf("      --help     display this help and exit\n");
            return FAIL;
        } else {
            str = strtok(command, "-");
            if (!str) {
                printf("cat: Invalid option\n");
                printf("Try 'cat --help' for more information.\n");
                return FAIL;
            } else {
                printf("cat: Unrecognized option -- '%s'\n", str);
                printf("Try 'cat --help' for more information.\n");
                return FAIL;
            }
        }
    } else {
        if (!strcmp(command, "/etc/password")) {
            concatenate(dirTree, "password", 4);
            return SUCCESS;
        }
        if (!command) {
            printf("cat: Invalid option\n");
            printf("Try 'rm --help' for more information.\n");
            return FAIL;
        }
        str = strtok(NULL, " ");
        threadTree[threadCount].threadTree = dirTree;
        threadTree[threadCount].option = 0;
        threadTree[threadCount++].command = command;
        while (str) {
            threadTree[threadCount].threadTree = dirTree;
            threadTree[threadCount].option = 0;
            threadTree[threadCount++].command = str;
            str = strtok(NULL, " ");
        }
    }

    for (int i = 0; i < threadCount; i++) {
        pthread_create(&threadPool[i], NULL, catUsedThread, (void *)&threadTree[i]);
        pthread_join(threadPool[i], NULL);
    }
    return SUCCESS;
}
