#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char* argv[])
{
    int address = 0;
    int ascii[16];
    unsigned char buff[16];

    FILE *fpr = fopen(argv[1], "rb");
    FILE *fpw = fopen("report.txt","w");

    if(fpr == NULL || fpw == NULL)
    {
        printf("%s failed to open \n",argv[1]);
        return 1;
    }

    printf("  地址                                 字节码内容                                    ASCII字符\n");
    fprintf(fpw, "  地址                                 字节码内容                                     ASCII字符\n");
    //while(fgets(buff,sizeof(buff),fpr) != NULL){
    //此处应将fgets修正为fread，因为fgets每次读取完后会在字符串后补/0导致输出的文件出错，在后续PEINFO中发现了这个问题
    while(fread(buff,1,sizeof(buff),fpr) != NULL){
        fread(buff,1,sizeof(buff),fpr);
        printf("%08x    ", address);
        fprintf(fpw, "%08x    ", address);
        for(int i = 0; i < 16; ++i){
            ascii[i] = buff[i];
            printf("%02X",buff[i]);
            fprintf(fpw, "%02X",buff[i]);
            if(i == 7){
                printf(" - ");
                fprintf(fpw, " - ");
            }
            else{
                printf("  ");
                fprintf(fpw, "  ");
            }
        }
        printf("    ");
        fprintf(fpw, "    ");
        for(int i = 0; i < 16; ++i){
            if(ascii[i] > 37 && ascii[i] < 127){
                printf("%c",ascii[i]);
                fprintf(fpw, "%c",ascii[i]);
            }
            else{
                printf(".");
                fprintf(fpw,".");
            }
        }
        address += 16;
        printf("\n");
        fprintf(fpw, "\n");
    }

    fclose(fpr);
    fclose(fpw);

    return 0;
}
