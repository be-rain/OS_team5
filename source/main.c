#include "../include/main.h"

int main() {
    char command[50];

    Linux = loadDirectoryTree(); // Directory 파일을 읽어와서 DirectoryTree 구조체에 저장
    usrList = loadUserList(); // User 파일을 읽어와서 UserList 구조체에 저장
    dStack = initStack(); // DirectoryStack 초기화

    login(usrList, Linux); // User 리스트에 있는 User로 로그인
    printStart(); // Welcome 메시지 출력
    saveUserList(usrList); // 현재 UserList 정보를 User 파일에 저장

    while (TRUE) {
        printPrompt(Linux, dStack); // Prompt 출력
        fgets(command, sizeof(command), stdin); // 명령어 입력
        command[strlen(command) - 1] = '\0'; // 개행문자 자리에 NULL 문자 입력 -> 문자열로 변경
        executeCommand(Linux, command); // 명령어 실행
    }
    return 0;
}
