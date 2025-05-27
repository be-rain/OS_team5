#include "../include/main.h"

void printDirDetails(DirectoryNode *dirNode, int cnt) {
    printf("%c", dirNode->type);
    printPermission(dirNode);
    printf("%3d", cnt);
    printf("   ");
    printf("%-5s%-5s", getUID(dirNode), getGID(dirNode));
    printf("%5d ", dirNode->SIZE);
    getMonth(dirNode->date.month);
    printf("%2d %02d:%02d ", dirNode->date.day, dirNode->date.hour, dirNode->date.minute);
}

void printDirNameWithColor(DirectoryNode *dirNode) {
    if (dirNode->type == 'd') {
        BOLD; BLUE;
        printf("%s\t", dirNode->name);
        DEFAULT;
    } else {
        printf("%s\t", dirNode->name);
    }
}

void printChildDirs(DirectoryNode *dirNode, int optionA) {
    DirectoryNode *tmpNode = dirNode->firstChild;
    while (tmpNode) {
        if (!optionA) {
            if (!strncmp(tmpNode->name, ".", 1)) {
                tmpNode = tmpNode->nextSibling;
                continue;
            }
        }
        printDirNameWithColor(tmpNode);
        tmpNode = tmpNode->nextSibling;
    }
    printf("\n");
}

int printHelp() {
    printf("Usage: ls [OPTION]... [FILE]...\n");
    printf("List information about the FILEs (the current directory by default).\n\n");
    printf("  Options:\n");
    printf("  -a, --all                  do not ignore entries starting with .\n");
    printf("  -l                         use a long listing format\n");
    printf("      --help     display this help and exit\n");
    return FAIL;
}

int getChildCount(DirectoryNode *dirNode) {
    int cnt = 0;
    DirectoryNode *tmpNode = dirNode->firstChild;
    if (!tmpNode) {
        if(dirNode->type == 'd') cnt = 2;
        else cnt = 1;
    } else {
        if(tmpNode->type == 'd') cnt = 3;
        else cnt = 2;
        while (tmpNode->nextSibling) {
            tmpNode = tmpNode->nextSibling;
            if (tmpNode->type == 'd') cnt += 1;
        }
    }
    return cnt;
}

int listDirSimple(DirectoryTree *dirTree, int optionA) {
    DirectoryNode *tmpNode = dirTree->current->firstChild;
    if (!optionA && !tmpNode) return FAIL;
    if (optionA) {
        BOLD; BLUE;
        printf(".\t");
        DEFAULT;
        if (dirTree->current != dirTree->root) {
            BOLD; BLUE;
            printf("..\t");
            DEFAULT;
        }
    }
    printChildDirs(dirTree->current, optionA);
    return SUCCESS;
}

int listDirDetailed(DirectoryTree *dirTree, int optionA) {
    DirectoryNode *tmpNode = dirTree->current->firstChild;
    if (!optionA && !tmpNode) {
        return FAIL;
    }
    if (optionA) {
        int cnt = getChildCount(dirTree->current);
        printDirDetails(dirTree->current, cnt);
        BOLD; BLUE;
        printf(".\n");
        DEFAULT;
        if (dirTree->current != dirTree->root) {
            cnt = getChildCount(dirTree->current->parent);
            printDirDetails(dirTree->current->parent, cnt);
            BOLD; BLUE;
            printf("..\n");
            DEFAULT;
        }
    }
    while (tmpNode) {
        if (!optionA) {
            if (!strncmp(tmpNode->name, ".", 1)) {
                tmpNode = tmpNode->nextSibling;
                continue;
            }
        }
        int cnt = getChildCount(tmpNode);
        char type = (tmpNode->type == 'd') ? 'd' : '-';
        printf("%c", type);
        printPermission(tmpNode);
        printf("%3d", cnt);
        printf("   ");
        printf("%-5s%-5s", getUID(tmpNode), getGID(tmpNode));
        printf("%5d ", tmpNode->SIZE);
        getMonth(tmpNode->date.month);
        printf("%2d %02d:%02d ", tmpNode->date.day, tmpNode->date.hour, tmpNode->date.minute);
        printDirNameWithColor(tmpNode);
        printf("\n");
        tmpNode = tmpNode->nextSibling;
    }
    return SUCCESS;
}

int ft_ls(DirectoryTree *dirTree, char *command) {
    DirectoryNode *currentNode = dirTree->current;
    char *str;
    int value;

    if (!command) {
        return listDirSimple(dirTree, 0);
    }
    if (command[0] == '-') {
        str = strtok(NULL, " ");
        if (str) {
            value = changePath(dirTree, str);
            if (value) return FAIL;
        }
        if (!strcmp(command, "-al") || !strcmp(command, "-la")) {
            listDirDetailed(dirTree, 1);
            dirTree->current = currentNode;
            return SUCCESS;
        } else if (!strcmp(command, "-l")) {
            listDirDetailed(dirTree, 0);
            dirTree->current = currentNode;
            return SUCCESS;
        } else if (!strcmp(command, "-a")) {
            listDirSimple(dirTree, 1);
            dirTree->current = currentNode;
            return SUCCESS;
        } else if (!strcmp(command, "--help")) {
            return printHelp();
        } else {
            str = strtok(command, "-");
            if (!str) {
                printf("ls: ls: cannot access '-': No such file or directory\n");
            } else {
                printf("ls: invalid option -- '%s'\n", str);
                printf("Try 'ls --help' for more information.\n");
            }
            dirTree->current = currentNode;
            return FAIL;
        }
    } else {
        value = changePath(dirTree, command);
        if (value) return FAIL;
        int result = listDirSimple(dirTree, 0);
        dirTree->current = currentNode;
        return result;
    }
}
