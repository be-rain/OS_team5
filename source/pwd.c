#include "../include/main.h"

void printFullPath(Stack *dirStack) {
    while (isEmpty(dirStack) == FALSE) {
        printf("/");
        printf("%s", popStack(dirStack));
    }
    printf("\n");
}

int printPath(DirectoryTree *dirTree, Stack *dirStack) {
    DirectoryNode *tmpNode = dirTree->current;

    if (tmpNode == dirTree->root) printf("/");
    else {
        while (tmpNode->parent) {
            pushStack(dirStack, tmpNode->name);
            tmpNode = tmpNode->parent;
        }
        printFullPath(dirStack);
    }
    return SUCCESS;
}

void printErrorOption(char *command, char *str) {
    printf("%s: Unrecognized option -- '%s'\n", command, str);
    printf("Try '%s --help' for more information.\n", command);
}

int ft_pwd(DirectoryTree *dirTree, Stack *dirStack, char *command) {
    char *str = NULL;
    if (!command) {
        printPath(dirTree, dirStack);
    } else if(command[0] == '-') {
        if (!strcmp(command, "--help")) {
            printf("pwd: pwd\n");
            printf("    Print the name of the current working directory.\n\n");
            printf("Options:\n");
            printf("      --help     display this help and exit\n");
            return FAIL;
        } else {
            str = strtok(command, "-");
            if (!str) {
                printf("pwd: Invalid option\n");
                printf("Try 'cd --help' for more information.\n");
                return -1;
            } else {
                printErrorOption("pwd", str);
                return FAIL;
            }
        }
    }
    return SUCCESS;
}
