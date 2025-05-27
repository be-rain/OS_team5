#include "../include/main.h"

void destroyNode(DirectoryNode *dirNode) {
    free(dirNode);
}

void destroyDir(DirectoryNode *dirNode) {
    if (dirNode->nextSibling) {
        destroyDir(dirNode->nextSibling);
    }
    if (dirNode->firstChild) {
        destroyDir(dirNode->firstChild);
    }
    destroyNode(dirNode);
}

DirectoryNode *findAndDestroyDir(DirectoryTree *dirTree, char *dirName) {
    DirectoryNode *delNode = NULL;
    DirectoryNode *tmpNode = dirTree->current->firstChild;
    DirectoryNode *prevNode = NULL;

    while (tmpNode && strcmp(tmpNode->name, dirName)) {
        prevNode = tmpNode;
        tmpNode = tmpNode->nextSibling;
    }

    if (tmpNode) {
        delNode = tmpNode;
        if (prevNode) {
            prevNode->nextSibling = delNode->nextSibling;
        } else {
            dirTree->current->firstChild = delNode->nextSibling;
        }
        if (delNode->firstChild) destroyDir(delNode->firstChild);
        destroyNode(delNode);
    } else {
        printf("rm: Can not remove '%s': No such file or directory.\n", dirName);
        return NULL;
    }

    return delNode;
}

int removeDir(DirectoryTree *dirTree, char *dirName) {
    if (!findAndDestroyDir(dirTree, dirName)) {
        return FAIL;
    }
    return SUCCESS;
}

void executeThread(DirectoryTree *dirTree, ThreadTree *threadTree, int option, char *command) {
    threadTree->threadTree = dirTree;
    threadTree->command = command;
    threadTree->option = option;
}

int ft_rm(DirectoryTree *dirTree, char *command) {
    DirectoryNode *currentNode = NULL;
    DirectoryNode *tmpNode = NULL;
    DirectoryNode *tmpNode2 = NULL;
    char *str;
    char tmp[MAX_DIR];
    char tmp2[MAX_DIR];
    char tmp3[MAX_DIR];
    int val;

    int threadCount = 0;
    pthread_t threadPool[MAX_THREAD];
    ThreadTree threadTree[MAX_THREAD];

    // not command
    if (!command) {
        printf("rm: rm: Invalid option\n");
        printf("Try 'rm --help' for more information.\n");
        return FAIL;
    }
    currentNode = dirTree->current;
    if (command[0] == '-'){
        // command is rm -r
        if (!strcmp(command, "-r")) {
            str = strtok(NULL, " ");
            if (!str) {
                printf("rm: Invalid option\n");
                printf("Try 'rm --help' for more information.\n");
                return FAIL;
            }
            while (str) {
                executeThread(dirTree, &threadTree[threadCount++], 1, str);
                str = strtok(NULL, " ");
            }
        } else if(!strcmp(command, "-f")) {
            str = strtok(NULL, " ");
            if (!str) {
                printf("rm: Invalid option\n");
                printf("Try 'rm --help' for more information.\n");
                return FAIL;
            }
            while (str) {
                executeThread(dirTree, &threadTree[threadCount++], 2, str);
                str = strtok(NULL, " ");
            }
        } else if (!strcmp(command, "-rf") || !strcmp(command, "-fr")) {
            str = strtok(NULL, " ");
            if (!str) {
                return FAIL;
            }
            while (str) {
                executeThread(dirTree, &threadTree[threadCount++], 3, str);
                str = strtok(NULL, " ");
            }
        } else if (!strcmp(command, "--help")) {
            printf("Manual: rm [Option]... [File]...\n");
            printf("  Remove the FILE(s).\n\n");
            printf("  Options:\n");
            printf("    -f, --force    \t ignore nonexistent files and arguments, never prompt\n");
            printf("    -r, --recursive\t remove directories and their contents recursively\n");
            printf("        --help\t Display this help and exit\n");
            return FAIL;
        } else {
            str = strtok(command, "-");
            if (!str) {
                printf("rm: Invalid option\n");
                printf("Try 'rm --help' for more information.\n");
                return FAIL;
            } else {
                printf("rm: Unrecognized option -- '%s'\n", str);
                printf("Try 'rm --help' for more information.\n");
                return FAIL;
            }
        }
    } else {
        str = strtok(NULL, " ");
        executeThread(dirTree, &threadTree[threadCount++], 0, command);
        if (!command) {
            printf("rm: Invalid option\n");
            printf("Try 'rm --help' for more information.\n");
            return FAIL;
        }
        while (str) {
            executeThread(dirTree, &threadTree[threadCount++], 0, str);
            str = strtok(NULL, " ");
        }
    }
    for (int i = 0; i < threadCount; i++) {
        pthread_create(&threadPool[i], NULL, removeDirUsedThread, (void *)&threadTree[i]);
        pthread_join(threadPool[i], NULL);
    }
    return SUCCESS;
}