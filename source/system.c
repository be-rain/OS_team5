#include "../include/main.h"

void printHelpMsg() {
    printf("You can use the command displayed below.\n");
    printf("\t- ls\n\t- cat\n\t- cd\n\t- mkdir\n\t- touch\n\t- rm\n\t- chmod\n\t- chown\n\t- grep\n\t- find\n\t- pwd\n\n");
    printf("The 'exit' command allows you to exit the program.\n\n");

}

// 현재 시간을 가져오는 함수
void getToday(Date *date) {
    time(&ltime);
    today = localtime(&ltime);

    date->weekday = today->tm_wday;
    date->month = today->tm_mon + 1;
    date->day = today->tm_mday;
    date->hour = today->tm_hour;
    date->minute = today->tm_min;
    date->second = today->tm_sec;
    date->year = today->tm_year + 1900;
}

// welcome message를 출력하는 함수
void printStart() {
    Date tmpDate;
    getToday(&tmpDate);
    printf("Welcome to Operating-System Team5-LINUX\n\n");
    printf("System information as of ");
    // getWeekday(tmpDate.weekday);
    // getMonth(tmpDate.month);
    // printf("%d %02d:%02d:%02d UTC %d\n\n", tmpDate.day, tmpDate.hour, tmpDate.minute, tmpDate.second, tmpDate.year);
    // printf("You can use the command displayed below.\n");
    printf("\t- ls\n\t- cat\n\t- cd\n\t- mkdir\n\t- touch\n\t- rm\n\t- chmod\n\t- chown\n\t- cp\n\t- grep\n\t- find\n\t- pwd\n\n - clear\n\n");
    printf("The 'exit' command allows you to exit the program.\n\n");
    printf("Last login: ");
    getWeekday(usrList->current->date.weekday);
    getMonth(usrList->current->date.month);
    printf("%d %02d:%02d:%02d %d\n", usrList->current->date.day, usrList->current->date.hour, usrList->current->date.minute, usrList->current->date.second, usrList->current->date.year);
}

// prompt를 출력하는 함수
void printPrompt(DirectoryTree *dirTree, Stack *dirStack) {
    DirectoryNode *tmpNode = NULL;
    char tmp[MAX_DIR] = "";
    char tmp2[MAX_DIR] = "";
    char usr;

    if (usrList->current == usrList->head) {
        usr = '#'; // root로 로그인 했을 떄
        BOLD;
        printf("%s@Operating-System-Team5", usrList->current->name);
        DEFAULT;
        printf(":");
    }
    else {
        usr = '$'; // 그 외 user로 로그인 했을 때
        BOLD;GREEN;
        printf("%s@Operating-System-Team5", usrList->current->name);
        DEFAULT;
        printf(":");
    }
    tmpNode = dirTree->current;

    if(tmpNode == dirTree->root){
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
    strncpy(tmp2, tmp, strlen(usrList->current->dir));
    if (usrList->current == usrList->head){
        BOLD;BLUE;
        printf("%s", tmp);
    } else if (strcmp(usrList->current->dir, tmp2)) {
	    BOLD;BLUE;
        printf("%s", tmp);
    } else {
        tmpNode = dirTree->current;
        while (tmpNode->parent) {
            pushStack(dirStack, tmpNode->name);
            tmpNode = tmpNode->parent;
        }
        popStack(dirStack);
        popStack(dirStack);
	    BOLD;BLUE;
        printf("~");
        while (isEmpty(dirStack) == FALSE) {
            printf("/");
            printf("%s", popStack(dirStack));
        }
    }
    DEFAULT;
    printf("%c ", usr);
}

void login(UserList *userList, DirectoryTree *dirTree) {
    UserNode *tmpUser = NULL;
    char userName[MAX_NAME];
    char tmp[MAX_DIR];

    tmpUser = userList->head;

    printf("Users: ");
    while (tmpUser) {
        printf("%s ", tmpUser->name);
        tmpUser = tmpUser->nextNode;
    }
    printf("\n");
    while (TRUE) {
        printf("Login as: ");
        fgets(userName, sizeof(userName), stdin);
        userName[strlen(userName) - 1] = '\0';
        if (!strcmp(userName, "exit")) {
            exit(0);
        }
        tmpUser = userExistence(userList, userName);
        if (tmpUser) {
            userList->current = tmpUser;
            break;
        }
        printf("'%s' User not found\n", userName);
    }
    strcpy(tmp, userList->current->dir);
    changePath(dirTree, tmp);
}