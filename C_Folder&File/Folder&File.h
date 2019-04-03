#ifndef FOLDER&FILE_H_INCLUDED
#define FOLDER&FILE_H_INCLUDED

typedef struct Folder{
    char* folderName;
    int depth;
    char* dir;
    char* fatherFolder;
    struct Folder *next;
}FolderNode,*FolderList;

void creartFolderLink(FolderList *L, char* dir);
void getFolders(FolderList *L);
void showFolders(FolderList *L);


typedef struct File{
    char* fileName;
    int fileSize;
    char* dir;
    char* folder;
    char* content;
}FileNode,*FileList;

void getFileList(char* dir, FileList *L);
char* getFileContent(char* dir);


#endif // FOLDER&FILE_H_INCLUDED
