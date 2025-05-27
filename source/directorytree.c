#include "../include/main.h"

DirectoryTree *loadDirectoryTree() {
    DirectoryTree *dirTree = (DirectoryTree *)malloc(sizeof(DirectoryTree));
    char tmp[MAX_LENGTH];

    Dir = fopen("file/Directory.txt", "r");
    if (Dir == NULL) {
        printf("Cannot open Directory.txt file for reading\n");
        return NULL;
    }
    
    while (fgets(tmp, MAX_LENGTH, Dir)) {
        readDirNode(dirTree, tmp);
    }
    fclose(Dir);
    
    dirTree->current = dirTree->root;

    return dirTree;
}

void saveDirectoryTree(DirectoryTree *dirTree, Stack *dirStack) {
    Dir = fopen("file/Directory.txt", "w");
    if (Dir == NULL) {
        printf("Cannot open Directory.txt file for writing\n");
        return;
    }
    
    writeDirNode(dirTree, dirTree->root, dirStack);
    
    fclose(Dir);
}
