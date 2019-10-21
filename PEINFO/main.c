#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

IMAGE_DOS_HEADER myDosHeader;
IMAGE_NT_HEADERS myNTHeader;
IMAGE_OPTIONAL_HEADER myOptionHeader;
IMAGE_SECTION_HEADER* pmySectionHeader;
LONG e_lfanew;
IMAGE_IMPORT_DESCRIPTOR* pmyImportDes;

void getValueByOffset(FILE* fp,DWORD offset, char* value, int size);

int main(int argc, char* argv[])
{
    int i = 0;
    printf("File Name: %s\n",argv[1]);
    FILE *fp = fopen(argv[1], "rb");
    if(fp == NULL)
    {
        printf("failed to open \n");
        return 1;
    }


    //DOS头部分
    printf("====================IMAGE_DOS_HEADER====================\n");
    fread(&myDosHeader,1 ,sizeof(IMAGE_DOS_HEADER), fp);
	if (myDosHeader.e_magic!=0x5A4D)
	{
		printf("not Exe File!");
		fclose(fp);
		return 1;
	}
	printf("MZ标识(WORD)               e_magic:             %04X\n", myDosHeader.e_magic);
	printf("PE偏移量(DOWRD)            e_lfaner:            %08X\n\n", myDosHeader.e_lfanew);
	e_lfanew = myDosHeader.e_lfanew;

	//NT头部分
	printf("====================IMAGE_NT_HEADER====================\n");
	fseek(fp, e_lfanew, SEEK_SET);
	fread(&myNTHeader, 1, sizeof(IMAGE_NT_HEADERS), fp);
	if (myNTHeader.Signature != 0x4550)
	{
		printf("not PE File!");
		fclose(fp);
		exit(0);
	}
    printf("PE标识(DWORD)              Signature:           %08X\n\n",myNTHeader.Signature);

    //FILE头部分
    printf("===================IMAGE_FILE_HEADER====================\n");
    printf("运行平台(WORD)             Machine:             %04X\n", myNTHeader.FileHeader.Machine);
    printf("节的数量(WORD)             NumberOfSection:     %d\n", myNTHeader.FileHeader.NumberOfSections);
    printf("文件属性(WORD)             Characteristics:     %04X\n\n", myNTHeader.FileHeader.Characteristics);

    //OPTIONAL头部分
	printf("==================IMAGE_OPTION_HEADER===================\n");
	printf("程序执行入口(DWORD)        AddressOfEntryPoint: %08X\n", myNTHeader.OptionalHeader.AddressOfEntryPoint);
    printf("代码节起始点(DWORD)        BaseOfCode:          %08X\n", myNTHeader.OptionalHeader.BaseOfCode);
	printf("数据节起始点(DWORD)        BaseOfData:          %08X\n", myNTHeader.OptionalHeader.BaseOfData);
    printf("建议装入基地址(DWORD)      ImageBase:           %08X\n", myNTHeader.OptionalHeader.ImageBase);
    printf("数据目录数量(DWORD)        NumberOfRvaAndSizes: %d\n\n", myNTHeader.OptionalHeader.NumberOfRvaAndSizes);

    //数据目录
    DWORD IAT_RVA = 0;                      //IAT的RVA
    DWORD ImportDir_RVA = 0;                //导入表的RVA
    DWORD IAT_offset = 0;                   //由IAT的RVA计算出的文件偏移地址
    DWORD ImportDir_offset = 0;             //由导入表的RVA计算出的文件偏移地址
    DWORD IAT_Section_RVA = 0;              //IAT所在的节的RVA
    DWORD IAT_Section_offset = 0;           //IAT所在的节的文件偏移
    DWORD ImportDir_Section_RVA = 0;        //导入表所在的节的RVA
    printf("==================IMAGE_DATA_DIRECTORY==================\n");
    //数据目录
    for(i = 0; i < myNTHeader.OptionalHeader.NumberOfRvaAndSizes; ++i){
        if(myNTHeader.OptionalHeader.DataDirectory[i].VirtualAddress != 0){
            if(i == 1){
                ImportDir_RVA = myNTHeader.OptionalHeader.DataDirectory[i].VirtualAddress;
            }
            if(i == 12){
                IAT_RVA = myNTHeader.OptionalHeader.DataDirectory[i].VirtualAddress;
            }
            printf("数据块%d:\n", i+1);
            printf("数据起始点(DWORD)          VirtualAddress:      %08X\n",myNTHeader.OptionalHeader.DataDirectory[i].VirtualAddress);
            printf("数据块长度(DWORD)          Size:                %08X\n\n",myNTHeader.OptionalHeader.DataDirectory[i].Size);
        }
    }

    //节表目录
	printf("==================IMAGE_SECTION_HEADER==================\n");
    pmySectionHeader = (IMAGE_SECTION_HEADER*)malloc(myNTHeader.FileHeader.NumberOfSections * sizeof(IMAGE_SECTION_HEADER));
    fseek(fp, (e_lfanew + sizeof(IMAGE_NT_HEADERS)), SEEK_SET);
    fread(pmySectionHeader, sizeof(IMAGE_SECTION_HEADER), myNTHeader.FileHeader.NumberOfSections, fp);
    printf("节的属性参考:\n");
    printf("    00000020h           包含代码\n");
    printf("    00000040h           包含已初始化的数据，如.const\n");
    printf("    00000080h           包含未初始化的数据，如.data?\n");
    printf("    02000000h           数据在进程开始以后被丢弃，如.reloc\n");
    printf("    04000000h           节中数据不经过缓存\n");
    printf("    08000000h           节中数据不会被交换到硬盘\n");
    printf("    10000000h           数据将被不同进程共享\n");
    printf("    20000000h           可执行\n");
    printf("    40000000h           可读\n");
    printf("    80000000h           可写\n\n");

    printf("内存中节的对齐粒度(DWORD)  SectionAlignment:    %08X\n",myNTHeader.OptionalHeader.SectionAlignment);
    printf("文件中节的对齐粒度(DWORD)  FileAlignment:       %08X\n\n",myNTHeader.OptionalHeader.FileAlignment);

    for(i = 0; i < myNTHeader.FileHeader.NumberOfSections; i++,pmySectionHeader++){
        //获取IAT和导入表所在的节的信息
        if(ImportDir_RVA != 0 && IAT_RVA != 0){
            if(pmySectionHeader->VirtualAddress <= IAT_RVA && pmySectionHeader->VirtualAddress + pmySectionHeader->Misc.VirtualSize > IAT_RVA){
                IAT_offset = pmySectionHeader->PointerToRawData + (IAT_RVA - pmySectionHeader->VirtualAddress);
                IAT_Section_RVA = pmySectionHeader->VirtualAddress;
                IAT_Section_offset = pmySectionHeader->PointerToRawData;
            }
            if(pmySectionHeader->VirtualAddress <= ImportDir_RVA && pmySectionHeader->VirtualAddress + pmySectionHeader->Misc.VirtualSize > ImportDir_RVA){
                ImportDir_offset = pmySectionHeader->PointerToRawData + (ImportDir_RVA - pmySectionHeader->VirtualAddress);
                ImportDir_Section_RVA = pmySectionHeader->VirtualAddress;
            }
        }
        printf("节区%d:\n", i);
        printf("节的名称(BYTE)             NAME:                %s\n",pmySectionHeader->Name);
        printf("未对齐前真实长度(DWORD)    VirtualSize:         %08X\n",pmySectionHeader->Misc.VirtualSize);
        printf("节的RVA(DWORD)             VirtualAddress:      %08X\n",pmySectionHeader->VirtualAddress);
        printf("文件中对齐后长度(DWORD)    SizeOfRawData:       %08X\n",pmySectionHeader->SizeOfRawData);
        printf("在文件中的偏移(DWORD)      PointerToRawData:    %08X\n",pmySectionHeader->PointerToRawData);
        printf("节的属性(DWORD)            Characteristics:     %08X\n\n",pmySectionHeader->Characteristics);
    }

    printf("========================================================\n\n");
    if(ImportDir_offset != 0){
        int count = 0;      //导入表中库的数量计数
        int count1 = 0;     //某导入库中函数数量计数
        while(1){
            pmyImportDes = (IMAGE_IMPORT_DESCRIPTOR*)malloc(sizeof(IMAGE_IMPORT_DESCRIPTOR));
            getValueByOffset(fp, ImportDir_offset + sizeof(IMAGE_IMPORT_DESCRIPTOR) * count, pmyImportDes, sizeof(IMAGE_IMPORT_DESCRIPTOR));
            count++;
            if(pmyImportDes->Name == 0)
                break;
            else{
                //获取导入库名
                DWORD name_offset = (pmyImportDes->Name - IAT_Section_RVA) + IAT_Section_offset;
                char name[32];
                getValueByOffset(fp, name_offset, name, 32);
                printf("导入库: %s\n", name);
                printf("-------------------------------\n");
                printf("OriginalFirstThunk   %08X\n", pmyImportDes->OriginalFirstThunk);
                printf("TimeDateStamp        %08X\n", pmyImportDes->TimeDateStamp);
                printf("ForwarderChain       %08X\n", pmyImportDes->ForwarderChain);
                printf("NameRva              %08X\n", pmyImportDes->Name);
                printf("NameOffest           %08X\n", name_offset);
                printf("FirstThunk           %08X\n", pmyImportDes->FirstThunk);
                //定位到IMAGE_THUNK_DATA
                if(pmyImportDes->OriginalFirstThunk < 0x10000000){
                    count1 = 0;
                    printf("-------------------------------\n");
                    //循环获取单个IMAGE_THUNK_DATA
                    while(1){
                        DWORD* pmyThunkData;
                        pmyThunkData = (DWORD*)malloc(sizeof(DWORD));
                        DWORD thunk_offset = (pmyImportDes->OriginalFirstThunk - IAT_Section_RVA) + IAT_Section_offset;
                        getValueByOffset(fp, thunk_offset + sizeof(DWORD) * count1, pmyThunkData, sizeof(DWORD));
                        count1++;
                        if((*pmyThunkData) == 0)
                            break;
                        else{
                            //获取Hint_Name_offset结构
                            DWORD Hint_Name_offset = (*pmyThunkData - IAT_Section_RVA) + IAT_Section_offset;
                            IMAGE_IMPORT_BY_NAME* pmyImportByName;
                            pmyImportByName = (IMAGE_IMPORT_BY_NAME*)malloc(320);
                            getValueByOffset(fp, Hint_Name_offset, pmyImportByName, 320);
                            printf("%08X       %s\n", pmyImportByName->Hint,pmyImportByName->Name);
                        }
                    }
                    printf("\n");
                }
            }
            free(pmyImportDes);
        }

    }

	fclose(fp);
    return 0;
}

//通过在文件中的偏移获取数据
void getValueByOffset(FILE* fp,DWORD offset, char* value, int size){
    long int position = ftell(fp);
    fseek(fp, offset,SEEK_SET);
    fread(value, sizeof(char),size,fp);
    fseek(fp, position,SEEK_SET);
}
