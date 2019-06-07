#ifndef FOLDER&FILE_H_INCLUDED
#define FOLDER&FILE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>

#define MAXDATA 9999999

typedef struct Folder{
    char* folderName;
    int depth;
    char* dir;
    char* fatherFolder;
    struct Folder *next;
}FolderNode,*FolderList;

//�����ļ�������
void creartFolderLink(FolderList *L, char* dir);
//��ĳ��ַ��ȫ�����ļ��м�������
void getAllFolders(FolderList *L);
//��ĳ�ļ����µĵ�һ���ļ��м�������
void getFolders(FolderList *L);
//��ӡ���ȫ���ļ�����Ϣ
void showFolders(FolderList *L);


typedef struct File{
    char* fileName;
    int fileSize;
    char* dir;
    char* folder;
    char* content;
    struct File *next;
}FileNode,*FileList;

//��ȡĳ��ַ��ȫ�����ļ�����
void getFileList(FileList *L, char* Folderdir);
//��ȡĳ�ļ�����
char* getFileContent(char* dir);
//��ӡ���ȫ���ļ���Ϣ
void showFiles(FileList *L);


//�½��ļ�
void initFile(char* fileName, char* content);
//����ļ�����
void clearFile(char* fileName);
//ɾ���ļ�
void deleteFile(char* fileName);

//������ݵ��ļ���ĳһ��
void addToFile(char* fileName, int row, char* content);
//�����ļ���ĳһ�е���Ϣ
void updateToFile(char* fileName, int row, char* content);
//ɾ���ļ���ĳһ�е���Ϣ
void removeFromFile(char* fileName, int row);
//��ȡ�ļ���ĳһ�е���Ϣ
char* getFromFile(char* fileName, int row);


#endif // FOLDER&FILE_H_INCLUDED
