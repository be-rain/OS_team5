#include "../include/main.h"

void *catUsedThread(void *arg) {
    ThreadTree *threadTree = (ThreadTree *)arg;
    DirectoryTree *dirTree = threadTree->threadTree;
    DirectoryTree *tmpTree;
    char *command = threadTree->command;
    DirectoryNode *currentNode = dirTree->current;
    DirectoryNode *tmpNode = NULL;
    DirectoryNode *tmpNode2 = NULL;
    char tmp[MAX_DIR];
    char tmp2[MAX_DIR];
    char tmp3[MAX_DIR];
    char *str;
    int option = threadTree->option;
    int value;

    strncpy(tmp, command, MAX_DIR);

    if (option == 1) {
        if (!strstr(tmp, "/")) {
            if (checkPermission(dirTree->current, 'w')) {
                printf("cat: Can not create file '%s': Permission denied\n", dirTree->current->name);
                return NULL;
            }
            tmpNode = dirExistence(dirTree, command, 'd');
            tmpNode2 = dirExistence(dirTree, command, 'f');
            if (!tmpNode && !tmpNode2) {
                printf("cat: '%s': No such file or directory.\n", command);
                return NULL;
            } else if (tmpNode && !tmpNode2) {
                printf("cat: '%s': Is a directory\n", command);
                return NULL;
            } else if (tmpNode2 && checkPermission(tmpNode2, 'r')) {
                printf("cat: Can not open file '%s': Permission denied\n", tmpNode2->name);
                return NULL;
            } else concatenate(dirTree, command, 2);
        } else {
            strncpy(tmp2, getDir(tmp), MAX_DIR);
            value = changePath(dirTree, tmp2);
            if (value) {
                printf("cat: '%s': No such file or directory.\n", tmp2);
                return NULL;
            }
            str = strtok(tmp, "/");
            while (str) {
                strncpy(tmp3, str, MAX_NAME);
                str = strtok(NULL, "/");
            }
            tmpNode = dirExistence(dirTree, tmp3, 'd');
            tmpNode2 = dirExistence(dirTree, tmp3, 'f');
            if(!tmpNode && !tmpNode2) {
                printf("cat: '%s': No such file or directory.\n", tmp3);
                dirTree->current = currentNode;
                return NULL;
            } else if (tmpNode && !tmpNode2) {
                printf("cat: '%s': Is a directory\n", tmp3);
                dirTree->current = currentNode;
                return NULL;
            } else if (tmpNode2 && checkPermission(tmpNode2, 'r')) {
                printf("cat: Can not open file '%s': Permission denied\n", tmpNode2->name);
                dirTree->current = currentNode;
                return NULL;
            } else concatenate(dirTree, tmp3, 2);
            dirTree->current = currentNode;
        }
    } else {
        if (!strstr(tmp, "/")) {
            if (checkPermission(dirTree->current, 'w')) {
                printf("cat: Can not create file '%s': Permission denied\n", dirTree->current->name);
                return NULL;
            }
            tmpNode = dirExistence(dirTree, command, 'd');
            tmpNode2 = dirExistence(dirTree, command, 'f');
            if (!tmpNode && !tmpNode2) {
                printf("cat: '%s': No such file or directory.\n", command);
                return NULL;
            } else if (tmpNode && !tmpNode2) {
                printf("cat: '%s': Is a directory\n", command);
                return NULL;
            } else if (tmpNode2 && checkPermission(tmpNode2, 'r')) {
                printf("cat: Can not open file '%s': Permission denied\n", tmpNode2->name);
                return NULL;
            } else concatenate(dirTree, command, 1);
        } else {
            strncpy(tmp2, getDir(tmp), MAX_DIR);
            value = changePath(dirTree, tmp2);
            if (value) {
                printf("cat: '%s': No such file or directory.\n", tmp2);
                return NULL;
            }
            str = strtok(tmp, "/");
            while (str) {
                strncpy(tmp3, str, MAX_NAME);
                str = strtok(NULL, "/");
            }
            tmpNode = dirExistence(dirTree, tmp3, 'd');
            tmpNode2 = dirExistence(dirTree, tmp3, 'f');
            if(!tmpNode && !tmpNode2) {
                printf("cat: '%s': No such file or directory.\n", tmp3);
                dirTree->current = currentNode;
                return NULL;
            } else if (tmpNode && !tmpNode2) {
                printf("cat: '%s': Is a directory\n", tmp3);
                dirTree->current = currentNode;
                return NULL;
            } else if (tmpNode2 && checkPermission(tmpNode2, 'r')) {
                printf("cat: Can not open file '%s': Permission denied\n", tmpNode2->name);
                dirTree->current = currentNode;
                return NULL;
            } else concatenate(dirTree, tmp3, 1);
            dirTree->current = currentNode;
        }
    }
    pthread_exit(NULL);
}

void *touchUsedThread(void *arg) {
    ThreadTree *threadTree = ((ThreadTree *)arg);
    DirectoryTree *dirTree = threadTree->threadTree;
    char *command = threadTree->command;
    DirectoryNode *currentNode = dirTree->current;
    char tmp[MAX_DIR];
    int value;

    strncpy(tmp, command, MAX_DIR);
    if (!strstr(command, "/")) {
        makeDir(dirTree, command, 'f');
    } else {
        char *getdirectory = getDir(command);
        value = changePath(dirTree, getdirectory);
        if (value) {
            printf("touch: '%s': No such file or directory.\n", getdirectory);
        } else {
            char *str = strtok(tmp, "/");
            char *directoryName;
            while (str) {
                directoryName = str;
                str = strtok(NULL, "/");
            }
            makeDir(dirTree, directoryName, 'f');
            dirTree->current = currentNode;
        }
    }
    pthread_exit(NULL);
}

void *makeDirUsedThread(void *arg) {
    ThreadTree *threadTree = ((ThreadTree *)arg);
    DirectoryTree *dirTree = threadTree->threadTree;
    DirectoryTree *p_preTree;
    char *command = threadTree->command;

    DirectoryNode *tmpNode = dirTree->current;
    char tmp[MAX_DIR];
    char pStr[MAX_DIR];
    char tmpStr[MAX_DIR];
    char directoryName[MAX_DIR];
    int directoryNameLength = 0;
    int isDirectoryExist;

    strncpy(tmp, command, MAX_DIR);

    if (strstr(command, "/") == NULL) {
        makeDir(dirTree, command, 'd');
    } else if (threadTree->option == 1) {
        int tmpLen = strlen(tmp), i = 0;
        if (tmp[0] == '/') {
            dirTree->current = dirTree->root;
            i = 1;
        }
        if (tmp[tmpLen - 1] == '/') {
            tmpLen -= 1;
        }
        for (; i < tmpLen; i++) {
            pStr[i] = tmp[i];
            pStr[i + 1] = 0;
            directoryName[directoryNameLength++] = tmp[i];
            if (tmp[i] == '/') {
                directoryName[--directoryNameLength] = 0;
                strncpy(tmpStr, pStr, i - 1);
                isDirectoryExist = changeCurrentDirectory(dirTree, directoryName);
                if (isDirectoryExist == -1) {
                    makeDir(dirTree, directoryName, 'd');
                    isDirectoryExist = changeCurrentDirectory(dirTree, directoryName);
                }
                directoryNameLength = 0;
            }
        }
        directoryName[directoryNameLength] = 0;
        makeDir(dirTree, directoryName, 'd');
        dirTree->current = tmpNode;
    } else {
        char *p_get_directory = getDir(command);
        isDirectoryExist = changePath(dirTree, p_get_directory);
        if (isDirectoryExist != 0) {
            printf("mkdir: '%s': No such file or directory.\n", p_get_directory);
        } else {
            char *str = strtok(tmp, "/");
            char *p_directory_name;
            while (str != NULL) {
                p_directory_name = str;
                str = strtok(NULL, "/");
            }
            makeDir(dirTree, p_directory_name, 'd');
            dirTree->current = tmpNode;
        }
    }
    pthread_exit(NULL);
}

void *removeDirUsedThread(void *arg) {
    ThreadTree *threadTree = (ThreadTree *)arg;
    DirectoryTree *dirTree = threadTree->threadTree;

    DirectoryNode *tmpNode = NULL;
    DirectoryNode *tmpNode2 = NULL;

    char *command = threadTree->command;
    char tmp[MAX_DIR];
    char tmp2[MAX_DIR];
    char tmp3[MAX_DIR];
    DirectoryNode *currentNode = dirTree->current;

    strncpy(tmp, command, MAX_DIR);
    int Option = threadTree->option;

    if (Option == 0) {
        if (!strstr(tmp, "/")) {
            tmpNode = dirExistence(dirTree, tmp, 'f');
            tmpNode2 = dirExistence(dirTree, tmp, 'd');
            if (!tmpNode && !tmpNode2) {
                printf("rm: Can not remove '%s': No such file or directory.\n", tmp);
                return NULL;
            }
            if (!tmpNode) {
                printf("rm: Can not remove '%s': No such file or directory.\n", command);
                return NULL;
            } else {
                if (checkPermission(dirTree->current, 'w') || checkPermission(tmpNode, 'w')) {
                    printf("rm: Can not remove '%s': Permission denied.\n", command);
                    return NULL;
                }
                removeDir(dirTree, tmp);
            }
        } else {
            strncpy(tmp2, getDir(tmp), MAX_DIR);
            int value = changePath(dirTree, tmp2);
            if (value) {
                printf("rm: Can not remove '%s': No such file or directory.\n", tmp2);
                return NULL;
            }
            char *str = strtok(command, "/");
            while (str) {
                strncpy(tmp3, str, MAX_NAME);
                str = strtok(NULL, "/");
            }
            tmpNode = dirExistence(dirTree, tmp3, 'f');
            tmpNode2 = dirExistence(dirTree, tmp3, 'd');
            if (tmpNode2) {
                printf("rm: Can not remove '%s': Is a directory.\n", tmp3);
                dirTree->current = currentNode;
                return NULL;
            }
            if (!tmpNode) {
                printf("rm: Can not remove '%s' No such file or directory.\n", tmp3);
                dirTree->current = currentNode;
                return NULL;
            } else {
                if (checkPermission(dirTree->current, 'w') || checkPermission(tmpNode, 'w')) {
                    printf("rm: Can not remove '%s' Permission denied.\n", tmp3);
                    dirTree->current = currentNode;
                    return NULL;
                }
                removeDir(dirTree, tmp3);
            }
            dirTree->current = currentNode;
        }
    } else if (Option == 1) {
        if (!strstr(tmp, "/")) {
            tmpNode = dirExistence(dirTree, tmp, 'f');
            tmpNode = dirExistence(dirTree, tmp, 'd') == NULL ? tmpNode : dirExistence(dirTree, tmp, 'd');
            if (!tmpNode) {
                printf("rm: Can not remove '%s': No such file or directory.\n", tmp);
                return NULL;
            } else {
                if (checkPermission(dirTree->current, 'w') || checkPermission(tmpNode, 'w')) {
                    printf("rm: failed to remove '%s'Can not remove directory or file: Permission denied.", tmp);
                    return NULL;
                }
                removeDir(dirTree, tmp);
            }
        } else {
            strncpy(tmp2, getDir(tmp), MAX_DIR);
            int value = changePath(dirTree, tmp2);
            if (value) {
                printf("rm: '%s': No such file or directory.\n", tmp2);
                return NULL;
            }
            char *str = strtok(command, "/");
            while (str) {
                strncpy(tmp3, str, MAX_NAME);
                str = strtok(NULL, "/");
            }
            tmpNode = dirExistence(dirTree, tmp3, 'f');
            tmpNode = dirExistence(dirTree, tmp3, 'd') == NULL ? tmpNode : dirExistence(dirTree, tmp3, 'd');
            if (tmpNode) {
                printf("rm: Can not remove '%s': No such file or directory.\n", tmp3);
                dirTree->current = currentNode;
                return NULL;
            } else {
                if (checkPermission(dirTree->current, 'w') || checkPermission(tmpNode, 'w')) {
                    printf("rm: failed to remove '%s' Can not remove directory or file: Permission denied.\n", tmp3);
                    dirTree->current = currentNode;
                    return NULL;
                }
                removeDir(dirTree, tmp3);
            }
            dirTree->current = currentNode;
        }
    } else if (Option == 2) {
        if (!strstr(tmp, "/")) {
            tmpNode = dirExistence(dirTree, tmp, 'f');
            tmpNode = dirExistence(dirTree, tmp, 'd') == NULL ? tmpNode : dirExistence(dirTree, tmp, 'd');
            if (!tmpNode) {
                return NULL;
            } else {
                if (checkPermission(dirTree->current, 'w') || checkPermission(tmpNode, 'w')) {
                    return NULL;
                }
                removeDir(dirTree, tmp);
            }
        } else {
            strncpy(tmp2, getDir(tmp), MAX_DIR);
            int value = changePath(dirTree, tmp2);
            if (value) return NULL;
            char *str = strtok(command, "/");
            while (str) {
                strncpy(tmp3, str, MAX_NAME);
                str = strtok(NULL, "/");
            }
            tmpNode = dirExistence(dirTree, tmp3, 'f');
            if (!tmpNode) {
                dirTree->current = currentNode;
                return NULL;
            } else {
                if (checkPermission(dirTree->current, 'w') || checkPermission(tmpNode, 'w')) {
                    dirTree->current = currentNode;
                    return NULL;
                }
                removeDir(dirTree, tmp3);
            }
            dirTree->current = currentNode;
        }
    } else if (Option == 3) {
        if (!strstr(tmp, "/")) {
            tmpNode = dirExistence(dirTree, tmp, 'f');
            tmpNode = dirExistence(dirTree, tmp, 'd') == NULL ? tmpNode : dirExistence(dirTree, tmp, 'd');
            if (!tmpNode) {
                return NULL;
            } else {
                if (checkPermission(dirTree->current, 'w') || checkPermission(tmpNode, 'w')) {
                    return NULL;
                }
                removeDir(dirTree, tmp);
            }
        } else {
            strncpy(tmp2, getDir(tmp), MAX_DIR);
            int value = changePath(dirTree, tmp2);
            if (value) return NULL;
            char *str = strtok(command, "/");
            while (str) {
                strncpy(tmp3, str, MAX_NAME);
                str = strtok(NULL, "/");
            }
            tmpNode = dirExistence(dirTree, tmp3, 'f');
            tmpNode = dirExistence(dirTree, tmp3, 'd') == NULL ? tmpNode : dirExistence(dirTree, tmp3, 'd');
            if (!tmpNode) {
                dirTree->current = currentNode;
                return NULL;
            } else {
                if (checkPermission(dirTree->current, 'w') || checkPermission(tmpNode, 'w')) {
                    dirTree->current = currentNode;
                    return NULL;
                }
                removeDir(dirTree, tmp3);
            }
            dirTree->current = currentNode;
        }
    }
    pthread_exit(NULL);
}

void *chmodUsedThread(void *arg) {
    ThreadTree *threadTree = (ThreadTree *)arg;
    DirectoryTree *dirTree = threadTree->threadTree;
    int mode = threadTree->mode;
    char *command = threadTree->command;
    DirectoryNode *node = dirExistence(dirTree, command, 'f');
    
    if (node) {
        changeFileMode(node, mode);
    } else {
        node = dirExistence(dirTree, command, 'd');
        if (node) {
            changeDirectoryMode(node, mode);
        } else {
            printf("chmod: Can not access '%s': No such file or directory\n", command);
        }
    }
    
    pthread_exit(NULL);
}

void *chownUsedThread(void *arg) {
    ThreadTree *threadTree = (ThreadTree *)arg;
    DirectoryTree *dirTree = threadTree->threadTree;
    char *command = threadTree->command;
    char *tmp = threadTree->usrName;
    char *str;
    char tmp3[MAX_NAME];
    
    strcpy(tmp3, tmp);

    if (!strstr(tmp, ":")) changeOwner(dirTree, tmp, command, 0);
    else {
        char tmp2[MAX_NAME];
        strncpy(tmp2, tmp3, MAX_NAME);
        char *str2 = strtok(tmp3, ":");
        if (str2 && strcmp(tmp3, tmp2)) {
            changeOwner(dirTree, str2, command, 0);
            str2 = strtok(NULL, " ");
            if (str2) changeOwner(dirTree, str2, command, 1);
        }
        else if (str2 && !strcmp(tmp3, tmp2)) changeOwner(dirTree, str2, command, 1);
    }
    pthread_exit(NULL);
}

void *grepUsedThread(void *arg) {
    ThreadTree *threadTree = (ThreadTree *)arg;
    DirectoryTree *dirTree = threadTree->threadTree;
    char *command = threadTree->command;
    char *content = threadTree->content;
    DirectoryNode *currentNode = dirTree->current;
    DirectoryNode *tmpNode = NULL;
    DirectoryNode *tmpNode2 = NULL;
    char *str;
    char tmp[MAX_DIR];
    char tmp2[MAX_DIR];
    char tmp3[MAX_DIR];
    int option = threadTree->option;
    int value;

    strncpy(tmp, command, MAX_DIR);
    if (!strstr(tmp, "/")) {
        tmpNode = dirExistence(dirTree, tmp, 'd');
        tmpNode2 = dirExistence(dirTree, tmp, 'f');
        if (!tmpNode && !tmpNode2) {
            printf("grep: '%s': No such file or directory.\n", command);
            return NULL;
        } else if (tmpNode && !tmpNode2) {
            printf("grep: '%s': Is a directory.\n", command);
            return NULL;
        } else if (tmpNode2 && checkPermission(tmpNode2, 'r')) {
            printf("grep: Can not open file '%s': Permission denied.\n", command);
            return NULL;
        } else printContent(dirTree, content, command, option);
    } else {
        strncpy(tmp2, getDir(command), MAX_DIR);
        value = changePath(dirTree, tmp2);
        if (value) {
            printf("grep: '%s': No such file or directory.\n", command);
            return NULL;
        }
        str = strtok(tmp, "/");
        while (str) {
            strncpy(tmp3, str, MAX_NAME);
            str = strtok(NULL, "/");
        }
        tmpNode = dirExistence(dirTree, tmp3, 'd');
        tmpNode2 = dirExistence(dirTree, tmp3, 'f');
        if (!tmpNode && !tmpNode2) {
            printf("grep: '%s': No such file or directory.\n", command);
            return NULL;
        } else if (tmpNode && !tmpNode2) {
            printf("grep: '%s': Is a directory.\n", command);
            return NULL;
        } else if (tmpNode2 && checkPermission(tmpNode2, 'r')) {
            printf("grep: Can not open file '%s': Permission denied.\n", command);
            return NULL;
        } else printContent(dirTree, content, tmp3, option);
        dirTree->current = currentNode;
    }
    pthread_exit(NULL);
}