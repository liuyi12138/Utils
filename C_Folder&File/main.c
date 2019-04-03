#include <stdio.h>
#include <stdlib.h>>
#include "Folder&File.h"

int main()
{
    FolderList Folders = NULL;
    creartFolderLink(&Folders, "w:/HUST");
    getFolders(&Folders);
    showFolders(&Folders);
    return 0;
}
