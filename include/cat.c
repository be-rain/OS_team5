#include "main.h"
#include "cat.h"

/*
 * concatenate: ���� ���� �ý���(DirectoryTree) ������ ���� �б�/���� ����
 * @param dirTree ������ to ���͸� Ʈ��
 * @param fName   ���� �̸�/���
 * @param type    0=����, 1=��ü �б�, 2=�� ��ȣ ���, 3=�� �� �ǳʶٱ�
 */
int concatenate(DirectoryTree* dirTree, char* fName, int type) {
    DirectoryNode* saved = dirTree->current;
    DirectoryNode* node = NULL;
    char pathBuf[MAX_DIR];
    char* dirPath;
    char* fname;
    int status = SUCCESS;

    /* 1) ���/���ϸ� �и� */
    strncpy(pathBuf, fName, MAX_DIR);
    pathBuf[MAX_DIR - 1] = '\0';
    dirPath = getDir(pathBuf);
    fname = pathBuf + strlen(dirPath);
    while (*fname == '/' || *fname == '\\') fname++;

    /* 2) ���͸� �̵� */
    if (changePath(dirTree, dirPath) != SUCCESS) {
        printf("cat: cannot change directory to '%s'\n", dirPath);
        status = FAIL;
        goto restore;
    }

    /* 3) ���� ���� ���� */
    node = dirExistence(dirTree, fname, 'f');
    if (!node) {
        if (type == 0) {
            /* ���� ��忡���� ���� ���� */
            if (makeDir(dirTree, fname, 'f') != SUCCESS) {
                printf("cat: failed to create file '%s'\n", fname);
                status = FAIL;
                goto restore;
            }

            // ���ƴ��� �߰��ش޶�� �� �ڵ� 
            {
                DirectoryNode* newNode = dirExistence(dirTree, fname, 'f');
                // �θ��ڽġ����� ������ �ʱ�ȭ/����
                newNode->parent = saved;
                newNode->firstChild = NULL;
                newNode->nextSibling = NULL;
                if (!saved->firstChild) {
                    saved->firstChild = newNode;
                }
                else {
                    DirectoryNode* sib = saved->firstChild;
                    while (sib->nextSibling)
                        sib = sib->nextSibling;
                    sib->nextSibling = newNode;
                }
            }
            // 

            node = dirExistence(dirTree, fname, 'f');
            if (!node) {
                printf("cat: file '%s' not found after creation\n", fname);
                status = FAIL;
                goto restore;
            }
        }
        else {
            printf("cat: '%s' does not exist\n", fname);
            status = FAIL;
            goto restore;
        }
    }

    /* 4) ���� ��� */
    if (type == 0) {
        /* ���� ���� ���� �ʱ�ȭ */
        node->data[0] = '\0';
        char lineBuf[MAX_BUFFER];
        printf("Enter file content. End with a line containing only 'end'.\n");
        while (fgets(lineBuf, MAX_BUFFER, stdin)) {
            if (strcmp(lineBuf, "end\n") == 0 || strcmp(lineBuf, "end\r\n") == 0)
                break;
            /* ���� �����÷ο� ���� */
            if ((int)(strlen(node->data) + strlen(lineBuf)) < LENGTH_SIZE) {
                strcat(node->data, lineBuf);
            }
        }
    }
    /* 5) �б� ��� */
    else {
        char buf[LENGTH_SIZE];
        strncpy(buf, node->data, LENGTH_SIZE);
        buf[LENGTH_SIZE - 1] = '\0';
        char* tok = strtok(buf, "\n");
        int lineNo = 1;
        while (tok) {
            int isBlank = (*tok == '\0');
            if (type == 1 || (type == 2) || (type == 3 && !isBlank)) {
                if (type == 2) {
                    printf("%6d  %s\n", lineNo++, tok);
                }
                else {
                    printf("%s\n", tok);
                }
            }
            tok = strtok(NULL, "\n");
        }
    }

restore:
    /* 6) ����ġ ���� */
    dirTree->current = saved;
    return status;
}


/*
 * ft_cat: ������ �Ѿ�� Ŀ�ǵ�(cmd) �Ľ� �� concatenate ȣ��
 */
int ft_cat(DirectoryTree* dirTree, char* cmd) {
    char* arg = cmd;
    int mode = 1; /* �⺻ �б� ��� */

    /* ���� ���� */
    while (*arg == ' ' || *arg == '\t') arg++;

    /* ���� ���𷺼�('>') ó�� */
    if (*arg == '>') {
        mode = 0;
        arg++;
    }
    /* �ɼ� ó�� */
    else if (*arg == '-') {
        if (strncmp(arg, "-n", 2) == 0) {
            mode = 2;
            arg += 2;
        }
        else if (strncmp(arg, "-b", 2) == 0) {
            mode = 3;
            arg += 2;
        }
        else {
            printf("cat: invalid option '%s'\n", arg);
            printf("Try 'cat --help' for more information.\n");
            return FAIL;
        }
    }

    /* ���ϸ� �� ���� ���� */
    while (*arg == ' ' || *arg == '\t') arg++;
    if (!*arg) {
        printf("cat: missing file operand\n");
        return FAIL;
    }

    return concatenate(dirTree, arg, mode);
}