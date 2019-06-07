#include "Folder&File.h"


void creartFolderLink(FolderList *L, char* dir){
    FolderNode *head;
    head=(FolderNode*)malloc(sizeof(FolderNode));
    head->dir = dir;
    head->folderName = strrchr(dir,'/') + 1;
    head->fatherFolder = "none";
    head->depth = 0;
    head->next = NULL;
    (*L) = head;
    return;
}
void getAllFolders(FolderList *L){
    FolderList Folders = *L;
    while(Folders) {
        getFolders(&Folders);
        Folders = Folders->next;
    }
    return;
}


void getFolders(FolderList *L){
    FolderList Folders = *L;
    int depth = Folders->depth;
    char* fatherFolderName = Folders->folderName;
    char* fatherDir = Folders->dir;
    while (Folders->next) Folders = Folders->next;

    long Handle;
    struct _finddata_t FileInfo;
    char trueDir[999];
    strcpy(trueDir,fatherDir);
    strcat(trueDir,"/*.*");
    if((Handle=_findfirst(trueDir,&FileInfo))==-1L)
        return;
    else
    {
        FolderNode *temp;
        char* tempDir;
        char* tempName;
        _findnext(Handle,&FileInfo);
        while(_findnext(Handle,&FileInfo)==0) {
            if (FileInfo.attrib ==48)
            {
                temp = (FolderNode*)malloc(sizeof(FolderNode));
                tempDir = (char*)malloc(999 * sizeof(char));
                tempName = (char*)malloc(256 * sizeof(char));

                strcpy(tempDir,fatherDir);
                strcat(tempDir,"/");
                strcat(tempDir,FileInfo.name);
                strcpy(tempName,FileInfo.name);

                temp->dir = tempDir;
                temp->depth = depth + 1;
                temp->fatherFolder = fatherFolderName;
                temp->folderName = tempName;

                temp->next = NULL;
                Folders->next = temp;
                Folders = temp;
            }
        }
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
