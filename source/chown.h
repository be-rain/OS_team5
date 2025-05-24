/* chown.h */
#ifndef CHOWN_H
#define CHOWN_H

#pragma once
#pragma warning(disable:4996)

#include "main.h"

/**
 * @brief ���� �Ǵ� ���丮 ������ ����
 * @param dirTree        ���� ���丮 Ʈ�� ������
 * @param userName       ������ ����� �̸�
 * @param nodeName       ��� ����/���丮 �̸�
 * @param recursiveFlag  ���� ���丮 ��� ���� ���� (0: ����, 1: ���)
 * @return SUCCESS(0) �Ǵ� FAIL(-1)
 */
int changeOwner(DirectoryTree* dirTree,
    char* userName,
    char* nodeName,
    int recursiveFlag);

/**
 * @brief ���丮 ��� ���� �׸� ��ü ������ ��� ����
 * @param node      ���� ���丮 ���
 * @param userName  ������ ����� �̸�
 */
void changeOwnerAll(DirectoryNode* node,
    char* userName);

/**
 * @brief chown ��ɾ� �Ľ� �� ����
 * @param dirTree  ���� ���丮 Ʈ�� ������
 * @param cmd      ��ü ��ɾ� ���ڿ� (��: "chown -R alice /path")
 * @return SUCCESS(0) �Ǵ� FAIL(-1)
 */
int ft_chown(DirectoryTree* dirTree,
    char* cmd);

#endif // CHOWN_H
