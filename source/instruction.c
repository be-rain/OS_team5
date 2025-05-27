#include "../include/main.h"

void executeCommand(DirectoryTree *dirTree, char *command) {
    char *str;
    char *str1;
    char *str2;
    int value;

    if (!strcmp(command, "") || command[0] == ' ') return;
    str = strtok(command, " ");
    if (!strcmp(str, "clear")) system("clear");
    else if (!strcmp(str, "mkdir")) {
        str = strtok(NULL, " ");
        value = ft_mkdir(dirTree, str);
        if (!value) saveDirectoryTree(dirTree, dStack);
    } else if (!strcmp(str, "touch")) {
        str = strtok(NULL, " ");
        value = ft_touch(dirTree, str);
        if (!value) saveDirectoryTree(dirTree, dStack);
    } else if (!strcmp(str, "rm")) {
        str = strtok(NULL, " ");
        value = ft_rm(dirTree, str);
        if (!value) saveDirectoryTree(dirTree, dStack);
    } else if (!strcmp(str, "cd")) {
        str = strtok(NULL, " ");
        ft_cd(dirTree, str);
    } else if(!strcmp(str, "pwd")) {
        str = strtok(NULL, " ");
        ft_pwd(dirTree, dStack, str);
    } else if (!strcmp(str, "ls")) {
        str = strtok(NULL, " ");
        ft_ls(dirTree, str);
    } else if (!strcmp(str, "cat")) {
        str = strtok(NULL, " ");
        value = ft_cat(dirTree, str);
        if (!value) saveDirectoryTree(dirTree, dStack);
    } else if(!strcmp(str, "chmod")) {
        str = strtok(NULL, " ");
        value = ft_chmod(dirTree, str);
        if (!value) saveDirectoryTree(dirTree, dStack);
    } else if(!strcmp(str, "chown")) {
        str = strtok(NULL, " ");
        value = ft_chown(dirTree, str);
        if (!value) saveDirectoryTree(dirTree, dStack);
    } else if (!strcmp(str, "find")) {
        str = strtok(NULL, " ");
        ft_find(dirTree, str);
    } else if (!strcmp(str, "grep")) {
        str = strtok(NULL, " ");
        ft_grep(dirTree, str);
    } else if (!strcmp(command, "help")) {
        printHelpMsg();
    } else if(!strcmp(command, "exit")) {
        printf("logout\n");
        exit(0);
    } else printf("Command '%s' not found\n", command);
    return;
}
