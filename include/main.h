#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#ifndef MAIN_H_
#define MAIN_H_
#define DEFAULT printf("%c[%dm", 0x1B, 0)
#define BOLD printf("%c[%dm", 0x1B, 1)
#define WHITE printf("\x1b[37m")
#define BLUE printf("\x1b[34m")
#define GREEN printf("\x1b[32m")

#define MAX_BUFFER 512
#define MAX_LENGTH 200
#define MAX_DIR 50
#define MAX_NAME 20
#define MAX_THREAD 50
#define TRUE 1
#define FALSE 0
#define SUCCESS 0
#define FAIL -1

typedef int boolean;

// Date
typedef struct s_date {
    int year;
    int month;
    int weekday;
    int day;
    int hour;
    int minute;
    int second;
} Date;

// ID
typedef struct s_id {
    int UID;
    int GID;
} ID;

// Permission
typedef struct s_permission {
    int mode;
    int permission[9];
} Permission;

// UserNode
typedef struct userNode {
    char name[MAX_NAME];
    char dir[MAX_DIR];
    ID id;
    Date date;
    struct userNode *nextNode;
} UserNode;

// UserList
typedef struct userList {
    ID topId;
    UserNode *head;
    UserNode *tail;
    UserNode *current;
} UserList;

// DirectoryNode
typedef struct directoryNode {
    char name[MAX_NAME];
    char type;
    int SIZE;
    Permission permission;
    ID id;
	Date date;
	struct directoryNode *parent;
	struct directoryNode *firstChild;
	struct directoryNode *nextSibling;
} DirectoryNode;

// DirectoryTree
typedef struct directoryTree {
	DirectoryNode* root;
	DirectoryNode* current;
} DirectoryTree;

// StackNode
typedef struct stackNode {
	char name[MAX_NAME];
	struct stackNode *nextNode;
} StackNode;

// Stack
typedef struct stack {
	StackNode* topNode;
	int cnt;
} Stack;

typedef struct threadTree {
    DirectoryTree *threadTree;
    DirectoryTree *fileName;
    char *content;
    char *command;
    char *usrName;
    int mode;
    int option;
} ThreadTree;

//time
time_t ltime;
struct tm *today;

// user.c
void writeUser(UserList *userList, UserNode *userNode);
int readUser(UserList *userList, char *tmp);
UserNode *userExistence(UserList *userList, char *userName);
char *getUID(DirectoryNode *dirNode);
char *getGID(DirectoryNode *dirNode);

// userlist.c
UserList *loadUserList();
void saveUserList(UserList *userList);

// directory.c
char *getDir(char *dirPath);
void getPath(DirectoryTree *dirTree, DirectoryNode *dirNode, Stack *dirStack, char *tmp);
void writeDirNode(DirectoryTree *dirTree, DirectoryNode *dirNode, Stack *dirStack);
int readDirNode(DirectoryTree *dirTree, char *tmp);
DirectoryNode *dirExistence(DirectoryTree *dirTree, char *dirName, char type);

// directorytree.c
void saveDirectoryTree(DirectoryTree *dirTree, Stack *dirStack);
DirectoryTree *loadDirectoryTree();

// cat.c
int concatenate(DirectoryTree* dirTree, char* fName, int type);
int ft_cat(DirectoryTree* dirTree, char* cmd);

// cd.c
int changePath(DirectoryTree* dirTree, char* dirPath);
int changeCurrentDirectory(DirectoryTree* dirTree, char* dirPath);
int ft_cd(DirectoryTree* dirTree, char* cmd);

// chmod.c
int changeDirectoryMode(DirectoryNode *node, int mode);
int changeFileMode(DirectoryNode *node, int mode);
int ft_chmod(DirectoryTree* dirTree, char* cmd);

// chown.c
int changeOwner(DirectoryTree *dirTree, char *userName, char *dirName, int flag);
int ft_chown(DirectoryTree* dirTree, char* cmd);

// find.c
int readDir(DirectoryTree *dirTree, char *tmp, char *dirName, int o);
void findDir(DirectoryTree *dirTree, char *dirName, int o);
int ft_find(DirectoryTree *dirTree, char *cmd);

// grep.c
int printContent(DirectoryTree *dirtree, char *search, char *fileName, int option);
int ft_grep(DirectoryTree *dirTree, char *command);

// ls.c
int listDir(DirectoryTree *dirTree, int a, int l);
int ft_ls(DirectoryTree *dirTree, char *cmd);

// mkdir.c
int makeDir(DirectoryTree *dirTree, char *dirName, char type);
int ft_mkdir(DirectoryTree *dirTree, char *cmd);

// touch.c
int ft_touch(DirectoryTree *dirTree, char *command);

// pwd.c
int printPath(DirectoryTree *dirTree, Stack *dirStack);
int ft_pwd(DirectoryTree *dirTree, Stack *dirStack, char *cmd);

// rm.c
void destroyNode(DirectoryNode *dirNode);
void destroyDir(DirectoryNode *dirNode);
int removeDir(DirectoryTree *dirTree, char *dirName);
int ft_rm(DirectoryTree *dirTree, char *cmd);

// instruction.c
void executeCommand(DirectoryTree* dirTree, char* cmd);

// system.c
void printHelpMsg();
void getToday(Date *date);
void printStart();
void printPrompt(DirectoryTree *dirTree, Stack *dirStack);
void login(UserList *userList, DirectoryTree *dirTree);

// permission.c
int modeToPermission(DirectoryNode *dirNode);
void printPermission(DirectoryNode *dirNode);
int checkPermission(DirectoryNode *dirNode, char o);

// thread.c
void *makeDirUsedThread(void *arg);
void *removeDirUsedThread(void *arg);
void *touchUsedThread(void *arg);
void *catUsedThread(void *arg);
void *chmodUsedThread(void *arg);
void *chownUsedThread(void *arg);
void *grepUsedThread(void *arg);

// stack.c
boolean isEmpty(Stack *stack);
Stack* initStack();
int pushStack(Stack* dirStack, char* dirName);
char *popStack(Stack* dirStack);

// time.c
void getMonth(int type);
void getWeekday(int type);

//global variable
DirectoryTree* Linux; // 전체 Directory 트리
Stack* dStack; // Directory 스택
UserList* usrList; // user 리스트
FILE* Dir; // Directory 파일
FILE* User; // User 파일

#endif 