#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

IMAGE_DOS_HEADER myDosHeader;
IMAGE_NT_HEADERS myNTHeader;
IMAGE_OPTIONAL_HEADER myOptionHeader;
IMAGE_SECTION_HEADER* pmySectionHeader;
LONG e_lfanew;

int main(int argc, char* argv[])
{
    int i = 0;
    printf("File Name: %s\n",argv[1]);
    FILE *fp = fopen(argv[1], "r");
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
    printf("==================IMAGE_DATA_DIRECTORY==================\n");
    for(i = 0; i < myNTHeader.OptionalHeader.NumberOfRvaAndSizes; ++i){
        if(myNTHeader.OptionalHeader.DataDirectory[i].VirtualAddress != 0){
            printf("数据块%d:\n", i);
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
        printf("节区%d:\n", i);
        printf("节的名称(BYTE)             NAME:                %s\n",pmySectionHeader->Name);
        printf("未对齐前真实长度(DWORD)    VirtualSize:         %08X\n",pmySectionHeader->Misc.VirtualSize);
        printf("文件中对齐后长度(DWORD)    SizeOfRawData:       %08X\n",pmySectionHeader->SizeOfRawData);
        printf("在文件中的偏移(DWORD)      PointerToRawData:    %08X\n",pmySectionHeader->PointerToRawData);
        printf("节的属性(DWORD)            Characteristics:     %08X\n\n",pmySectionHeader->Characteristics);
    }

	fclose(fp);
    return 0;
}
