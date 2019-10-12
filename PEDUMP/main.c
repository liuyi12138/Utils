#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char* argv[])
{
    int address = 0;
    int ascii[16];
    unsigned char buff[16];
    unsigned char* result;
    result = (char*)malloc(1024 * sizeof(char));

    FILE *fpr = fopen(argv[0], "r");
    FILE *fpw = fopen("report.txt","w");

    if(fpr == NULL || fpw == NULL)
    {
        printf("%s failed to open \n",argv[0]);
        return 1;
    }

    printf("  ��ַ                                 �ֽ�������                                    ASCII�ַ�\n");
    fprintf(fpw, "  ��ַ                                 �ֽ�������                                     ASCII�ַ�\n");
    while(fgets(buff,sizeof(buff),fpr) != NULL){
        strcat(result,buff);
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
