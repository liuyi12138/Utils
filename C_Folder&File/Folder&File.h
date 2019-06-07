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

//创建文件夹链表
void creartFolderLink(FolderList *L, char* dir);
//将某地址下全部的文件夹加入链表
void getAllFolders(FolderList *L);
//将某文件夹下的第一层文件夹加入链表
void getFolders(FolderList *L);
//打印输出全部文件夹信息
void showFolders(FolderList *L);


typedef struct File{
    char* fileName;
    int fileSize;
    char* dir;
    char* folder;
    char* content;
    struct File *next;
}FileNode,*FileList;

//获取某地址下全部的文件链表
void getFileList(FileList *L, char* Folderdir);
//获取某文件内容
char* getFileContent(char* dir);
//打印输出全部文件信息
void showFiles(FileList *L);


//新建文件
void initFile(char* fileName, char* content);
//清除文件内容
void clearFile(char* fileName);
//删除文件
void deleteFile(char* fileName);

//添加内容到文件的某一行
void addToFile(char* fileName, int row, char* content);
//更新文件中某一行的信息
void updateToFile(char* fileName, int row, char* content);
//删除文件中某一行的信息
void removeFromFile(char* fileName, int row);
//获取文件中某一行的信息
char* getFromFile(char* fileName, int row);


#endif // FOLDER&FILE_H_INCLUDED
