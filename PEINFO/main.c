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
    FILE *fp = fopen(argv[1], "rb");
    if(fp == NULL)
    {
        printf("failed to open \n");
        return 1;
    }


    //DOSͷ����
    printf("====================IMAGE_DOS_HEADER====================\n");
    fread(&myDosHeader,1 ,sizeof(IMAGE_DOS_HEADER), fp);
	if (myDosHeader.e_magic!=0x5A4D)
	{
		printf("not Exe File!");
		fclose(fp);
		return 1;
	}
	printf("MZ��ʶ(WORD)               e_magic:             %04X\n", myDosHeader.e_magic);
	printf("PEƫ����(DOWRD)            e_lfaner:            %08X\n\n", myDosHeader.e_lfanew);
	e_lfanew = myDosHeader.e_lfanew;

	//NTͷ����
	printf("====================IMAGE_NT_HEADER====================\n");
	fseek(fp, e_lfanew, SEEK_SET);
	fread(&myNTHeader, 1, sizeof(IMAGE_NT_HEADERS), fp);
	if (myNTHeader.Signature != 0x4550)
	{
		printf("not PE File!");
		fclose(fp);
		exit(0);
	}
    printf("PE��ʶ(DWORD)              Signature:           %08X\n\n",myNTHeader.Signature);

    //FILEͷ����
    printf("===================IMAGE_FILE_HEADER====================\n");
    printf("����ƽ̨(WORD)             Machine:             %04X\n", myNTHeader.FileHeader.Machine);
    printf("�ڵ�����(WORD)             NumberOfSection:     %d\n", myNTHeader.FileHeader.NumberOfSections);
    printf("�ļ�����(WORD)             Characteristics:     %04X\n\n", myNTHeader.FileHeader.Characteristics);

    //OPTIONALͷ����
	printf("==================IMAGE_OPTION_HEADER===================\n");
	printf("����ִ�����(DWORD)        AddressOfEntryPoint: %08X\n", myNTHeader.OptionalHeader.AddressOfEntryPoint);
    printf("�������ʼ��(DWORD)        BaseOfCode:          %08X\n", myNTHeader.OptionalHeader.BaseOfCode);
	printf("���ݽ���ʼ��(DWORD)        BaseOfData:          %08X\n", myNTHeader.OptionalHeader.BaseOfData);
    printf("����װ�����ַ(DWORD)      ImageBase:           %08X\n", myNTHeader.OptionalHeader.ImageBase);
    printf("����Ŀ¼����(DWORD)        NumberOfRvaAndSizes: %d\n\n", myNTHeader.OptionalHeader.NumberOfRvaAndSizes);

    //����Ŀ¼
    printf("==================IMAGE_DATA_DIRECTORY==================\n");
    for(i = 0; i < myNTHeader.OptionalHeader.NumberOfRvaAndSizes; ++i){
        if(myNTHeader.OptionalHeader.DataDirectory[i].VirtualAddress != 0){
            printf("���ݿ�%d:\n", i);
            printf("������ʼ��(DWORD)          VirtualAddress:      %08X\n",myNTHeader.OptionalHeader.DataDirectory[i].VirtualAddress);
            printf("���ݿ鳤��(DWORD)          Size:                %08X\n\n",myNTHeader.OptionalHeader.DataDirectory[i].Size);
        }
    }

    //�ڱ�Ŀ¼
	printf("==================IMAGE_SECTION_HEADER==================\n");
    pmySectionHeader = (IMAGE_SECTION_HEADER*)malloc(myNTHeader.FileHeader.NumberOfSections * sizeof(IMAGE_SECTION_HEADER));
    fseek(fp, (e_lfanew + sizeof(IMAGE_NT_HEADERS)), SEEK_SET);
    fread(pmySectionHeader, sizeof(IMAGE_SECTION_HEADER), myNTHeader.FileHeader.NumberOfSections, fp);
    printf("�ڵ����Բο�:\n");
    printf("    00000020h           ��������\n");
    printf("    00000040h           �����ѳ�ʼ�������ݣ���.const\n");
    printf("    00000080h           ����δ��ʼ�������ݣ���.data?\n");
    printf("    02000000h           �����ڽ��̿�ʼ�Ժ󱻶�������.reloc\n");
    printf("    04000000h           �������ݲ���������\n");
    printf("    08000000h           �������ݲ��ᱻ������Ӳ��\n");
    printf("    10000000h           ���ݽ�����ͬ���̹���\n");
    printf("    20000000h           ��ִ��\n");
    printf("    40000000h           �ɶ�\n");
    printf("    80000000h           ��д\n\n");

        printf("�ڴ��нڵĶ�������(DWORD)  SectionAlignment:    %08X\n",myNTHeader.OptionalHeader.SectionAlignment);
        printf("�ļ��нڵĶ�������(DWORD)  FileAlignment:       %08X\n\n",myNTHeader.OptionalHeader.FileAlignment);

    for(i = 0; i < myNTHeader.FileHeader.NumberOfSections; i++,pmySectionHeader++){
        printf("����%d:\n", i);
        printf("�ڵ�����(BYTE)             NAME:                %s\n",pmySectionHeader->Name);
        printf("δ����ǰ��ʵ����(DWORD)    VirtualSize:         %08X\n",pmySectionHeader->Misc.VirtualSize);
        printf("�ļ��ж���󳤶�(DWORD)    SizeOfRawData:       %08X\n",pmySectionHeader->SizeOfRawData);
        printf("���ļ��е�ƫ��(DWORD)      PointerToRawData:    %08X\n",pmySectionHeader->PointerToRawData);
        printf("�ڵ�����(DWORD)            Characteristics:     %08X\n\n",pmySectionHeader->Characteristics);
    }

	fclose(fp);
    return 0;
}
