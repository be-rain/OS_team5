#include "../include/main.h"

int changeOwner(DirectoryTree *dirTree, char *userName, char *dirName, int flag) {
    DirectoryNode *dirNode = NULL;
    DirectoryNode *fileNode = NULL;
    UserNode *tmpUser = NULL;

    dirNode = dirExistence(dirTree, dirName, 'd');
    fileNode = dirExistence(dirTree, dirName, 'f');

    if (dirNode) {
        if (checkPermission(dirNode, 'w')) {
            printf("chown: Can not modify file '%s': Permission denied\n", dirName);
            return FAIL;
        }
        tmpUser = userExistence(usrList, userName);
        if (tmpUser) {
            if (!flag) dirNode->id.UID = tmpUser->id.UID;
            else dirNode->id.GID = tmpUser->id.GID;
        } else {
            printf("chown: Invalid user: '%s'\n", userName);
            printf("Try 'chown --help' for more information.\n");
            return FAIL;
        }
    } else if (fileNode) {
        if (checkPermission(fileNode, 'w')) {
            printf("chown: Can not modify file '%s': Permission denied\n", dirName);
            return FAIL;
        }
        tmpUser = userExistence(usrList, userName);
        if (tmpUser) {
            if (!flag) fileNode->id.UID = tmpUser->id.UID;
            else fileNode->id.GID = tmpUser->id.GID;
        } else {
            printf("chown: Invalid user: '%s'\n", userName);
            printf("Try 'chown --help' for more information.\n");
            return FAIL;
        }
    } else {
        printf("chown: cannot access '%s': No such file or directory\n", dirName);
        return FAIL;
    }
    return SUCCESS;
}

int ft_chown(DirectoryTree* dirTree, char* command) {
    DirectoryNode* tmpNode = NULL;
    ThreadTree threadTree[MAX_THREAD];
    pthread_t threadPool[MAX_THREAD];
    char tmp[MAX_NAME];
    int threadCount = 0;
    UserNode* tmpUser = NULL;
    char *str;

    if (!command) {
        printf("chown: Invalid option\n");
        printf("Try 'chown --help' for more information.\n");
        return -1;
    }
    if (command[0] == '-') {
        if(!strcmp(command, "--help")) {
            printf("Usage: chown [OPTION]... [OWNER] FILE...\n");
            printf("Change the owner of each FILE to OWNER.\n\n");
            printf("  Options:\n");
            printf("  -R, --recursive        operate on files and directories recursively\n");
            printf("      --help     display this help and exit\n");
            return FAIL;
        } else {
            str = strtok(command, "-");
            if (!str) {
                printf("chown: Invalid option\n");
                printf("Try 'chown --help' for more information.\n");
                return FAIL;
            } else {
                printf("chown: Unrecognized option -- '%s'\n", str);
                printf("Try 'chown --help' for more information.\n");
                return FAIL;
            }
        }
    } else {
        strncpy(tmp, command, MAX_NAME);
        str = strtok(NULL, " ");
        if (!str) {
            printf("chown: Invalid option\n");
            printf("Try 'chown --help' for more information.\n");
            return FAIL;
        } else {
            threadTree[threadCount].threadTree = dirTree;
            threadTree[threadCount].usrName = tmp;
            threadTree[threadCount++].command = str;
            while (str) {
                threadTree[threadCount].threadTree = dirTree;
                threadTree[threadCount].usrName = tmp;
                threadTree[threadCount++].command = str;
                str = strtok(NULL, " ");
            }
        }
    }
    for (int i = 0; i < threadCount; i++) {
        pthread_create(&threadPool[i], NULL, chownUsedThread, (void*)&threadTree[i]);
        pthread_join(threadPool[i], NULL);
    }

    return SUCCESS;
}