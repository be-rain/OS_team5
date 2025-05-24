/* chown.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chown.h"    // �ݵ�� �ڽ��� ����� include

/**
 * @note flag == 0: ���� ����, flag == 1: ��� ����
 */
int changeOwner(DirectoryTree* dirTree,
    char* userName,
    char* dirName,
    int                 flag)
{
    // 1) ����� ��ȸ
    UserNode* usr = userExistence(usrList, userName);
    if (!usr) {
        printf("chown: Invalid user '%s'\n", userName);
        printf("Try 'chown --help' for more information.\n");
        return FAIL;
    }

    // 2) ���͸� �Ǵ� ���� ��� ��ȸ
    DirectoryNode* dirNode = dirExistence(dirTree, dirName, 'd');
    if (!dirNode) {
        dirNode = dirExistence(dirTree, dirName, 'f');
        if (!dirNode) {
            printf("chown: %s: No such file or directory\n", dirName);
            return FAIL;
        }
    }

    // 3) �ش� ����� UID/GID ����
    dirNode->id.UID = usr->id.UID;
    dirNode->id.GID = usr->id.GID;

    // 4) -R(flag==1) �ɼ��̸� ���� Ʈ�� ��ü�� ����
    if (flag == 1 && dirNode->firstChild) {
        changeOwnerAll(dirNode->firstChild, usr);
    }

    return SUCCESS;
}

/**
 * @brief ���丮 ���� ��ü ��忡 ���� ������ ��� ����
 */
void changeOwnerAll(DirectoryNode* node, UserNode* usr)
{
    // ���� ��� UID/GID ����
    node->id.UID = usr->id.UID;
    node->id.GID = usr->id.GID;

    // �ڽ� ��� ���
    if (node->firstChild) {
        changeOwnerAll(node->firstChild, usr);
    }
    // ���� ��� ���
    if (node->nextSibling) {
        changeOwnerAll(node->nextSibling, usr);
    }
}

/**
 * @brief �Էµ� ��ɾ� ���ڿ��� �Ľ��� changeOwner ȣ��
 */
int ft_chown(DirectoryTree* dirTree,
    char* cmd)
{
    char* token = strtok(cmd, " ");
    int recursive = 0;
    char userName[MAX_NAME];
    char targetName[MAX_DIR];

    // ù ��ū�� "chown"
    if (!token) {
        printf("Usage: chown [-R] user file\n");
        return FAIL;
    }

    // �ɼ� ó��
    token = strtok(NULL, " ");
    if (token && strcmp(token, "-R") == 0) {
        recursive = 1;
        token = strtok(NULL, " ");
    }

    // ����� �̸�
    if (!token) {
        printf("chown: missing user operand\n");
        return FAIL;
    }
    strncpy(userName, token, MAX_NAME - 1);
    userName[MAX_NAME - 1] = '\0';

    // ��� ����/���丮
    token = strtok(NULL, " ");
    if (!token) {
        printf("chown: missing file operand\n");
        return FAIL;
    }
    strncpy(targetName, token, MAX_DIR - 1);
    targetName[MAX_DIR - 1] = '\0';

    // ���� ������ ���� ����
    return changeOwner(dirTree, userName, targetName, recursive);
}
