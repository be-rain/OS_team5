#include "../include/main.h"

int changeCurrentDirectory(DirectoryTree *dirTree, char *dirPath) {
    DirectoryNode *tmpNode = NULL;

    if (!strcmp(dirPath, ".")) {
        // Do nothing, stay in the current directory
    } else if (!strcmp(dirPath, "..")) {
        if (dirTree->current != dirTree->root) {
            dirTree->current = dirTree->current->parent;
        }
    } else {
        tmpNode = dirExistence(dirTree, dirPath, 'd');
        if (tmpNode) {
            dirTree->current = tmpNode;
        } else {
            return FAIL;
        }
    }
    return SUCCESS;
}

int changePath(DirectoryTree *dirTree, char *dirPath) {
    DirectoryNode *tmpNode = NULL;
    char tmpPath[MAX_DIR];
    char *str = NULL;
    int value = 0;

    strncpy(tmpPath, dirPath, MAX_DIR);
    tmpNode = dirTree->current;
    if (!strcmp(dirPath, "/")) {
        dirTree->current = dirTree->root;
    } else {
        if (!strncmp(dirPath, "/", 1)) {
            if (!strtok(dirPath, "/")) {
                return FAIL;
            }
            dirTree->current = dirTree->root;
        }
        str = strtok(tmpPath, "/");
        while (str) {
            value = changeCurrentDirectory(dirTree, str);
            if (value) {
                dirTree->current = tmpNode;
                return FAIL;
            }
            str = strtok(NULL, "/");
        }
    }
    return SUCCESS;
}

int ft_cd(DirectoryTree *dirTree, char *command) {
    DirectoryNode *tmpNode = NULL;
    char *str = NULL;
    char tmp[MAX_DIR];
    int value;

    if (!command) {
        strcpy(tmp, usrList->current->dir);
        changePath(dirTree, tmp);
    } else if (command[0] == '-') {
        if (!strcmp(command, "--help")) {
            printf("cd: cd [dir]\n");
            printf("    Change the shell working directory.\n\n");
            printf("    Change the current directory to DIR. The default DIR is the value of the\n");
            printf("    HOME shell variable.\n\n");
            printf("    The variable CDPATH defines the search path for the directory containing\n");
            printf("    DIR. Alternative directory names in CDPATH are separated by a colon (:).\n");
            printf("    A null directory name is the same as the current directory. If DIR begins\n");
            printf("    with a slash (/), then CDPATH is not used.\n\n");
            printf("    If the directory is not found, and the shell option `cdable_vars' is set,\n");
            printf("    the word is assumed to be a variable name. If that variable has a value,\n");
            printf("    its value is used for DIR.\n\n");
            printf("    Options:\n");
            printf("      --help     display this help and exit\n");
            return FAIL;
        } else {
            str = strtok(command, "-");
            if (!str) {
                printf("cd: Invalid option\n");
                printf("Try 'cd --help' for more information.\n");
                return FAIL;
            } else {
                printf("cd: Unrecognized option -- '%s'\n", str);
                printf("Try 'cd --help' for more information.\n");
                return FAIL;
            }
        }
    } else {
        tmpNode = dirExistence(dirTree, command, 'd');
        if (tmpNode) {
            if (checkPermission(tmpNode, 'x')) {
                printf("-bash: cd: '%s': Permission denied\n", command);
                return FAIL;
            }
        }
        tmpNode = dirExistence(dirTree, command, 'f');
        if (tmpNode) {
            printf("-bash: cd: '%s': Not a directory\n", command);
            return FAIL;
        }
        value = changePath(dirTree, command);
        if (value) {
            printf("-bash: cd: No such file or directory '%s'\n", command);
        }
    }
    return SUCCESS;
}
