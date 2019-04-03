#include <iostream>
#include <sys/stat.h>

std::string filepath = "/sdcard/xxx/xxx/xxx/xxx.json";
std::fstream file;
std::string tempFilename = "/";
char filenames[100];
std::strcpy(filenames,filepath.c_str());
char *token = std::strtok(filenames, "/");
while (token != NULL) {
    tempFilename += token;
    token = std::strtok(NULL, "/");
    file.open(tempFilename, std::ios::in);
    if (!file.is_open()){
        if (tempFilename == filepath){
            std::ofstream outfile;
            SE_LOG_INFO(kSettingModule,"Write File");
            outfile.open(filepath.c_str());
            if (outfile.fail()) {
                outfile.close();
                SE_LOG_ERROR(kSettingModule, "Mkdir ERROR");
                return (-1);
            }
            outfile.close();
        }
        else{
            int flag = mkdir(tempFilename.c_str(), 0777);
            if (flag != 0){
                SE_LOG_ERROR(kSettingModule, "Mkdir ERROR");
                return (-1);
            }
        }
    }
    file.close();
    tempFilename += "/";
}