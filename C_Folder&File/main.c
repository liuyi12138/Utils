#include <stdio.h>
#include <stdlib.h>>
#include "Folder&File.h"

int main()
{
    FolderList Folders = NULL;
    creartFolderLink(&Folders, "W:/HUST/ChallengeCup/stressManager/back_end/app/Http");
    getALlFolders(&Folders);
    showFolders(&Folders);

    FileList Files = NULL;
    getFileList(&Files, "W:/HUST/ChallengeCup/stressManager");
    showFiles(&Files);

    writeFile("W:/HUST/ChallengeCup/stressManager/test.txt",Files->content);

    return 0;
}
