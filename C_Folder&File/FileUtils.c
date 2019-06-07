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
    result = (char*)malloc(MAXDATA * sizeof(char));

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


void initFile(char* fileName, char* content) {

	FILE *fp =fopen(fileName,"w");
	if(fp == NULL)
	{
		return 0;
	}
	fprintf(fp ,"%s",content);
	fclose(fp);

}

void clearFile(char* fileName) {
    FILE *fp=fopen(fileName,"w");
	if(fp == NULL)
	{
		return 0;
	}
	fprintf(fp,"%s","");
	fclose(fp);
	printf("clear success\n");
}

void deleteFile(char* fileName) {
    if(remove(fileName) == 0 )
        printf("delete success\n");
    else
        printf("delete error\n");

}

void addToFile(char* fileName, int row, char* content) {

    char buff[512];
    char* result;
    int i = 1;
    result = (char*)malloc(MAXDATA * sizeof(char));

    FILE *fp = fopen(fileName, "rt+");
    if(fp == NULL)
    {
        printf("failed to open \n");
        return "";
    }

    while(fgets(buff,sizeof(buff),fp) != NULL) {
        if(i == row){
            strcat(result,content);
            strcat(result,"\n");
        }
        strcat(result,buff);
        ++i;
    }
    if(row == 0){
        strcat(result,content);
        strcat(result,"\n");
    }
    fclose(fp);

    initFile(fileName,result);
    printf("add success\n");
}

void updateToFile(char* fileName, int row, char* content) {
    char buff[512];
    char* result;
    int i = 1;
    result = (char*)malloc(MAXDATA * sizeof(char));

    FILE *fp = fopen(fileName, "rt+");
    if(fp == NULL)
    {
        printf("failed to open \n");
        return "";
    }

    while(fgets(buff,sizeof(buff),fp) != NULL) {
        if(i == row){
            strcat(result,content);
            strcat(result,"\n");
        }
        else
            strcat(result,buff);
        ++i;
    }
    fclose(fp);

    initFile(fileName,result);
    printf("update success\n");
}

void removeFromFile(char* fileName, int row) {
    char buff[512];
    char* result;
    int i = 1;
    int ch;
    result = (char*)malloc(MAXDATA * sizeof(char));

    FILE *fp = fopen(fileName, "rt+");
    if(fp == NULL)
    {
        printf("failed to open \n");
        return "";
    }

    while(fgets(buff,sizeof(buff),fp) != NULL) {
        if (row != 0 || EOF != (ch = fgetc(fp))) {
            if(i != row)
                strcat(result,buff);
            ++i;
        }
    }
    fclose(fp);

    initFile(fileName,result);
    printf("remove success\n");
}

char* getFromFile(char* fileName, int row) {
    char buff[512];
    char* result;
    int i = 1;
    int ch;
    result = (char*)malloc(512 * sizeof(char));

    FILE *fp = fopen(fileName, "r");
    if(fp == NULL)
    {
        printf("failed to open \n");
        return "";
    }

    while(fgets(buff,sizeof(buff),fp) != NULL) {
        if (row == 0 && EOF == (ch = fgetc(fp)))
            strcpy(result,buff);
        else if(i == row){
            strcpy(result,buff);
            break;
        }
        ++i;
    }
    fclose(fp);

    return result;
}
