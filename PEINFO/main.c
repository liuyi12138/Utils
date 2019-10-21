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
    DWORD IAT_RVA = 0;                      //IAT��RVA
    DWORD ImportDir_RVA = 0;                //������RVA
    DWORD IAT_offset = 0;                   //��IAT��RVA��������ļ�ƫ�Ƶ�ַ
    DWORD ImportDir_offset = 0;             //�ɵ�����RVA��������ļ�ƫ�Ƶ�ַ
    DWORD IAT_Section_RVA = 0;              //IAT���ڵĽڵ�RVA
    DWORD IAT_Section_offset = 0;           //IAT���ڵĽڵ��ļ�ƫ��
    DWORD ImportDir_Section_RVA = 0;        //��������ڵĽڵ�RVA
    printf("==================IMAGE_DATA_DIRECTORY==================\n");
    //����Ŀ¼
    for(i = 0; i < myNTHeader.OptionalHeader.NumberOfRvaAndSizes; ++i){
        if(myNTHeader.OptionalHeader.DataDirectory[i].VirtualAddress != 0){
            if(i == 1){
                ImportDir_RVA = myNTHeader.OptionalHeader.DataDirectory[i].VirtualAddress;
            }
            if(i == 12){
                IAT_RVA = myNTHeader.OptionalHeader.DataDirectory[i].VirtualAddress;
            }
            printf("���ݿ�%d:\n", i+1);
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
        //��ȡIAT�͵�������ڵĽڵ���Ϣ
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
        printf("����%d:\n", i);
        printf("�ڵ�����(BYTE)             NAME:                %s\n",pmySectionHeader->Name);
        printf("δ����ǰ��ʵ����(DWORD)    VirtualSize:         %08X\n",pmySectionHeader->Misc.VirtualSize);
        printf("�ڵ�RVA(DWORD)             VirtualAddress:      %08X\n",pmySectionHeader->VirtualAddress);
        printf("�ļ��ж���󳤶�(DWORD)    SizeOfRawData:       %08X\n",pmySectionHeader->SizeOfRawData);
        printf("���ļ��е�ƫ��(DWORD)      PointerToRawData:    %08X\n",pmySectionHeader->PointerToRawData);
        printf("�ڵ�����(DWORD)            Characteristics:     %08X\n\n",pmySectionHeader->Characteristics);
    }

    printf("========================================================\n\n");
    if(ImportDir_offset != 0){
        int count = 0;      //������п����������
        int count1 = 0;     //ĳ������к�����������
        while(1){
            pmyImportDes = (IMAGE_IMPORT_DESCRIPTOR*)malloc(sizeof(IMAGE_IMPORT_DESCRIPTOR));
            getValueByOffset(fp, ImportDir_offset + sizeof(IMAGE_IMPORT_DESCRIPTOR) * count, pmyImportDes, sizeof(IMAGE_IMPORT_DESCRIPTOR));
            count++;
            if(pmyImportDes->Name == 0)
                break;
            else{
                //��ȡ�������
                DWORD name_offset = (pmyImportDes->Name - IAT_Section_RVA) + IAT_Section_offset;
                char name[32];
                getValueByOffset(fp, name_offset, name, 32);
                printf("�����: %s\n", name);
                printf("-------------------------------\n");
                printf("OriginalFirstThunk   %08X\n", pmyImportDes->OriginalFirstThunk);
                printf("TimeDateStamp        %08X\n", pmyImportDes->TimeDateStamp);
                printf("ForwarderChain       %08X\n", pmyImportDes->ForwarderChain);
                printf("NameRva              %08X\n", pmyImportDes->Name);
                printf("NameOffest           %08X\n", name_offset);
                printf("FirstThunk           %08X\n", pmyImportDes->FirstThunk);
                //��λ��IMAGE_THUNK_DATA
                if(pmyImportDes->OriginalFirstThunk < 0x10000000){
                    count1 = 0;
                    printf("-------------------------------\n");
                    //ѭ����ȡ����IMAGE_THUNK_DATA
                    while(1){
                        DWORD* pmyThunkData;
                        pmyThunkData = (DWORD*)malloc(sizeof(DWORD));
                        DWORD thunk_offset = (pmyImportDes->OriginalFirstThunk - IAT_Section_RVA) + IAT_Section_offset;
                        getValueByOffset(fp, thunk_offset + sizeof(DWORD) * count1, pmyThunkData, sizeof(DWORD));
                        count1++;
                        if((*pmyThunkData) == 0)
                            break;
                        else{
                            //��ȡHint_Name_offset�ṹ
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

//ͨ�����ļ��е�ƫ�ƻ�ȡ����
void getValueByOffset(FILE* fp,DWORD offset, char* value, int size){
    long int position = ftell(fp);
    fseek(fp, offset,SEEK_SET);
    fread(value, sizeof(char),size,fp);
    fseek(fp, position,SEEK_SET);
}
