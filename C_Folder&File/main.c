#include "Folder&File.h"

int main()
{
    FolderList Folders = NULL;
    creartFolderLink(&Folders, "L:/github");
    getAllFolders(&Folders);
    showFolders(&Folders);

    FileList Files = NULL;
    getFileList(&Files, "L:/github/Utils");
    showFiles(&Files);


    initFile("L:/github/test.txt","222222\n");
    clearFile("L:/github/test.txt");
    deleteFile("L:/github/test.txt");

    addToFile("L:/github/test.txt",0,"33333333");
    addToFile("L:/github/test.txt",0,"222222222");
    updateToFile("L:/github/test.txt",1,"11111111111");
    removeFromFile("L:/github/test.txt",0);

    char* a = getFromFile("L:/github/test.txt",1);
    char* b = getFromFile("L:/github/test.txt",0);
    printf("%s", a);
    printf("%s", b);

    return 0;
}
