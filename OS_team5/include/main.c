#include "main.h"
#define INPUT_SIZE 256

pthread_mutex_t dirTreeMutex = PTHREAD_MUTEX_INITIALIZER;
time_t ltime = 0;
struct tm *today = NULL; 

DirectoryTree* Linux = NULL;
Stack* dStack = NULL;
UserList* usrList = NULL;

FILE* Dir = NULL;
FILE* User = NULL;

int main() {
    char input[INPUT_SIZE];

    // 초기화
    Linux = loadDirectoryTree();
    if (Linux == NULL) {
        // 루트 디렉토리 만들기
        Linux = (DirectoryTree*)malloc(sizeof(DirectoryTree));
        DirectoryNode* root = (DirectoryNode*)malloc(sizeof(DirectoryNode));
        strcpy(root->name, "/");
        root->type = 'd';
        root->firstChild = NULL;
        root->nextSibling = NULL;
        root->parent = NULL;
        root->id.UID = 0;
        root->id.GID = 0;
        root->permission.mode = 755;
        getToday(&root->date);
        modeToPermission(root);

        Linux->root = root;
        Linux->current = root;
    }

    usrList = loadUserList();
    dStack = initStack();

    printf("Mini Linux Shell 시작합니다. 'exit' 입력 시 종료.\n");

    // 쉘 루프, 명령어 완성 될때마다 추가
    while (1) {
        printPrompt(Linux, dStack);
        fgets(input, INPUT_SIZE, stdin);
        input[strcspn(input, "\n")] = 0;  // 줄 바꿈 제거

        if (strcmp(input, "exit") == 0) {
            break;
        } else if (strncmp(input, "mkdir", 5) == 0) {
            ft_mkdir(Linux, input);
        } else if (strncmp(input, "ls", 2) == 0) {
            ft_ls(Linux, input);
        } else if (strncmp(input, "cd", 2) == 0) {
            ft_cd(Linux, input);
        } else if (strncmp(input, "pwd", 3) == 0) {
            ft_pwd(Linux, dStack, input);
        } else {
            printf("Unknown command: %s\n", input);
        }
    }

    printf("시스템을 종료합니다.\n");
    return 0;
}