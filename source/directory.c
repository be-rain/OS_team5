#include "../include/main.h"

DirectoryNode *dirExistence(DirectoryTree *dirTree, char *dirName, char type) {
    for (DirectoryNode *current = dirTree->current->firstChild; current; current = current->nextSibling) {
        if (!strcmp(current->name, dirName) && current->type == type) {
            return current;
        }
    }
    return NULL;
}

char *getDir(char *dirPath) {
    char *str, *tmpPath = (char *)malloc(MAX_DIR), tmp[MAX_DIR], tmp2[MAX_DIR];
    strncpy(tmp, dirPath, MAX_DIR);

    for (str = strtok(dirPath, "/"); str; str = strtok(NULL, "/")) {
        strncpy(tmp2, str, MAX_DIR);
    }

    strncpy(tmpPath, tmp, strlen(tmp) - strlen(tmp2) - 1);
    tmpPath[strlen(tmp) - strlen(tmp2) - 1] = '\0';
    return tmpPath;
}

void getPath(DirectoryTree *dirTree, DirectoryNode *dirNode, Stack *dirStack, char *tmp) {
    DirectoryNode *tmpNode = dirNode->parent;

    if (tmpNode == dirTree->root) {
        strcpy(tmp, "/");
    } else {
        while (tmpNode->parent) {
            pushStack(dirStack, tmpNode->name);
            tmpNode = tmpNode->parent;
        }
        while (isEmpty(dirStack) == FALSE) {
            strcat(tmp, "/");
            strcat(tmp, popStack(dirStack));
        }
    }
    fprintf(Dir, " %s\n", tmp);
}

void writeDirNode(DirectoryTree *dirTree, DirectoryNode *dirNode, Stack *dirStack) {
    char tmp[MAX_DIR] = "";

    fprintf(Dir, "%s %c %d ", dirNode->name, dirNode->type, dirNode->permission.mode);
    fprintf(Dir, "%d %d %d %d %d %d %d", dirNode->SIZE, dirNode->id.UID, dirNode->id.GID, dirNode->date.month, dirNode->date.day, dirNode->date.hour, dirNode->date.minute);

    if (dirNode != dirTree->root) {
        getPath(dirTree, dirNode, dirStack, tmp);
    } else {
        fprintf(Dir, "\n");
    }

    if (dirNode->nextSibling) {
        writeDirNode(dirTree, dirNode->nextSibling, dirStack);
    }
    if (dirNode->firstChild) {
        writeDirNode(dirTree, dirNode->firstChild, dirStack);
    }
}

void createAndAttachNode(DirectoryTree *dirTree, char *str, DirectoryNode *newNode, DirectoryNode *tmpNode) {
    if (str) {
        str[strlen(str) - 1] = '\0';
        changePath(dirTree, str);
        newNode->parent = dirTree->current;
        if (!dirTree->current->firstChild) {
            dirTree->current->firstChild = newNode;
        } else {
            tmpNode = dirTree->current->firstChild;
            while (tmpNode->nextSibling) {
                tmpNode = tmpNode->nextSibling;
            }
            tmpNode->nextSibling = newNode;
        }
    } else {
        dirTree->root = newNode;
        dirTree->current = dirTree->root;
    }
}

int readDirNode(DirectoryTree *dirTree, char *tmp) {
    DirectoryNode *newNode = (DirectoryNode *)malloc(sizeof(DirectoryNode)), *tmpNode = NULL;
    char *str;

    newNode->firstChild = NULL;
    newNode->nextSibling = NULL;
    newNode->parent = NULL;

    str = strtok(tmp, " ");
    strncpy(newNode->name, str, MAX_NAME);
    str = strtok(NULL, " ");
    newNode->type = str[0];
    str = strtok(NULL, " ");
    newNode->permission.mode = atoi(str);
    modeToPermission(newNode);
    str = strtok(NULL, " ");
    newNode->SIZE = atoi(str);
    str = strtok(NULL, " ");
    newNode->id.UID = atoi(str);
    str = strtok(NULL, " ");
    newNode->id.GID = atoi(str);
    str = strtok(NULL, " ");
    newNode->date.month = atoi(str);
    str = strtok(NULL, " ");
    newNode->date.day = atoi(str);
    str = strtok(NULL, " ");
    newNode->date.hour = atoi(str);
    str = strtok(NULL, " ");
    newNode->date.minute = atoi(str);
    str = strtok(NULL, " ");

    createAndAttachNode(dirTree, str, newNode, tmpNode);
    return 0;
}
