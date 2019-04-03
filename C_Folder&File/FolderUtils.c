#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include "Folder&File.h"


void creartFolderLink(FolderList *L, char* dir){
    FolderNode *head;
    head=(FolderNode*)malloc(sizeof(FolderNode));
    head->dir = dir;
    head->folderName = "";
    head->fatherFolder = "root";
    head->depth = 0;
    head->next = NULL;
    (*L) = head;
    return;
}

void getFolders(FolderList *L){
    FolderList Folders = *L;

    long Handle;
    struct _finddata_t FileInfo;
    if((Handle=_findfirst(Folders->dir,&FileInfo))==-1L)
        printf("no such file\n");
    else
    {
        Folders->folderName = FileInfo.name;
        printf("folder:%s\n",FileInfo.name);
        while(_findnext(Handle,&FileInfo)==0)
            printf("%s\n",FileInfo.name);
            _findclose(Handle);
    }
    return;
}

void showFolders(FolderList *L){
    FolderList p = *L;
    while(p){
        printf("folderName: %s,  depth: %d,  fatherFolder: %s,  dir: %s\n\n", p->folderName, p->depth, p->fatherFolder, p->dir);
        p = p->next;
    }
    return;
}
