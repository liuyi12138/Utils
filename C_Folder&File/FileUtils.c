#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include "Folder&File.h"

void getFileList(FileList *L, char* Folderdir) {
    FileNode *p,*temp;

    char* tempName;
    char* tempDir;

    long Handle;
    struct _finddata_t FileInfo;
    char trueDir[999];
    strcpy(trueDir,Folderdir);
    strcat(trueDir,"/*.*");

    if((Handle=_findfirst(trueDir,&FileInfo))==-1L)
        return;
    else {
        _findnext(Handle,&FileInfo);
        while(_findnext(Handle,&FileInfo)==0) {
            if (FileInfo.attrib !=48) {
                temp = (FolderNode*)malloc(sizeof(FolderNode));
                tempDir = (char*)malloc(999 * sizeof(char));
                tempName = (char*)malloc(256 * sizeof(char));

                strcpy(tempDir,Folderdir);
                strcat(tempDir,"/");
                strcat(tempDir,FileInfo.name);
                strcpy(tempName,FileInfo.name);

                temp->fileName = tempName;
                temp->dir = tempDir;
                temp->fileSize = FileInfo.size;
                temp->folder = strrchr(Folderdir,'/') + 1;
                temp->content = getFileContent(tempDir);
                temp->next = NULL;

                if((*L) == NULL)
                    (*L) = temp;
                else
                    p->next = temp;
                p = temp;
            }
        }
        _findclose(Handle);
    }
    return;
}


char* getFileContent(char* dir) {
    char buff[512];
    char* result;
    result = (char*)malloc(999999 * sizeof(char));

    FILE *fp = fopen(dir, "r");
    if(fp == NULL)
    {
        printf("failed to open \n");
        return "";
    }

    while(fgets(buff,sizeof(buff),fp) != NULL)
        strcat(result,buff);
    fclose(fp);

    return result;
}

void showFiles(FileList *L) {
    FileList p = *L;
    while(p){
        printf("fileName: %s,  fileSize: %d,  folder: %s,  dir: %s\n", p->fileName, p->fileSize, p->folder, p->dir);
        printf("content:  \n%s\n\n\n",p->content);
        p = p->next;
    }
    return;
}


void writeFile(char* fileName, char* content) {

	FILE *fpWrite=fopen(fileName,"w");
	if(fpWrite==NULL)
	{
		return 0;
	}
	fprintf(fpWrite,"%s",content);
	fclose(fpWrite);

}
