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
void getALlFolders(FolderList *L);
void getFolders(FolderList *L);
void showFolders(FolderList *L);


typedef struct File{
    char* fileName;
    int fileSize;
    char* dir;
    char* folder;
    char* content;
    struct File *next;
}FileNode,*FileList;

void getFileList(FileList *L, char* Folderdir);
char* getFileContent(char* dir);
void showFiles(FileList *L);
void writeFile(char* fileName, char* content);


#endif // FOLDER&FILE_H_INCLUDED
