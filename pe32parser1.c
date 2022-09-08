#include <stdio.h>
#include <stdlib.h>
#define DIR_NUMBER 2 // 2 for import and export directory, can be 0x10 (16) for all data directories - not implemented
#define LIB_API_NAME_LEN 40

struct pe_header_information
{
    int optional_header_size;                   // size of optional header
    int num_sections;                           // number of sections
};
struct optional_header_information
{
    int data_directories_rvasize[DIR_NUMBER][4]; // rva, size, physical address and section rva caluclation of each directory
};
struct section_header_information
{
    int section_rva, section_size, section_pdata; // rva, size and pointer to raw data of each directory
};

int printmz(const unsigned char *buff)
{
    printf("MZ header\n=========\n");
    printf("\tDescription:                      Data:           Value / Address:\n");
    printf("\t--------------------------------------------------------------------------------------\n");
    printf("\tMagic:                            0x%02x%02x     --> 'MZ'\n", *(buff + 1), *(buff + 0));
    printf("\tBytesOnLastPageofFile:            0x%02x%02x\n", *(buff + 3), *(buff + 2));
    printf("\tPagesInFile:                      0x%02x%02x\n", *(buff + 5), *(buff + 4));
    printf("\tRelocations:                      0x%02x%02x\n", *(buff + 7), *(buff + 6));
    printf("\tSizeOfHeaderInParagraphs:         0x%02x%02x\n", *(buff + 9), *(buff + 8));
    printf("\tMinimumExtraParagraphs:           0x%02x%02x\n", *(buff + 11), *(buff + 10));
    printf("\tMaximumExtraParagraphs:           0x%02x%02x\n", *(buff + 13), *(buff + 12));
    printf("\tInitial (relative) SS:            0x%02x%02x\n", *(buff + 15), *(buff + 14));
    printf("\tInitial SP:                       0x%02x%02x\n", *(buff + 17), *(buff + 16));
    printf("\tChecksum:                         0x%02x%02x\n", *(buff + 19), *(buff + 18));
    printf("\tInitial IP:                       0x%02x%02x\n", *(buff + 21), *(buff + 20));
    printf("\tInitial (relative) CS:            0x%02x%02x\n", *(buff + 23), *(buff + 22));
    printf("\tOffsetToRelocationTable:          0x%02x%02x\n", *(buff + 25), *(buff + 24));
    printf("\tOverlay Number:                   0x%02x%02x\n", *(buff + 27), *(buff + 26));
    printf("\tReserved:                         0x%02x%02x\n", *(buff + 29), *(buff + 28));
    printf("\tReserved:                         0x%02x%02x\n", *(buff + 31), *(buff + 30));
    printf("\tReserved:                         0x%02x%02x\n", *(buff + 33), *(buff + 32));
    printf("\tReserved:                         0x%02x%02x\n", *(buff + 35), *(buff + 34));
    printf("\tOEMIdentifier:                    0x%02x%02x\n", *(buff + 37), *(buff + 36));
    printf("\tOEMInformation :                  0x%02x%02x\n", *(buff + 39), *(buff + 38));
    printf("\tReserved:                         0x%02x%02x\n", *(buff + 41), *(buff + 40));
    printf("\tReserved:                         0x%02x%02x\n", *(buff + 43), *(buff + 42));
    printf("\tReserved:                         0x%02x%02x\n", *(buff + 45), *(buff + 44));
    printf("\tReserved:                         0x%02x%02x\n", *(buff + 47), *(buff + 46));
    printf("\tReserved:                         0x%02x%02x\n", *(buff + 49), *(buff + 48));
    printf("\tReserved:                         0x%02x%02x\n", *(buff + 51), *(buff + 50));
    printf("\tReserved:                         0x%02x%02x\n", *(buff + 53), *(buff + 52));
    printf("\tReserved:                         0x%02x%02x\n", *(buff + 55), *(buff + 54));
    printf("\tReserved:                         0x%02x%02x\n", *(buff + 57), *(buff + 56));
    printf("\tReserved:                         0x%02x%02x\n", *(buff + 59), *(buff + 58));
    printf("\tOffsetToNewEXEHeader:             0x%02x%02x%02x%02x\n", *(buff + 63), *(buff + 62), *(buff + 61), *(buff + 60));
    printf("\n");
    return (*(buff + 60)) + (*(buff + 61) << 8) + (*(buff + 62) << 16) + (*(buff + 63) >> 24); // address of pe header
}

void printpe(const unsigned char *buff, struct pe_header_information *pe)
{
    printf("PE header\n=========\n");
    printf("\tDescription:                      Data:           Value / Address:\n");
    printf("\t--------------------------------------------------------------------------------------\n");
    printf("\tSignature:                        0x%02x%02x%02x%02x --> 'PE'\n", *(buff - 1), *(buff - 2), *(buff - 3), *(buff - 4));
    printf("\tMachine:                          0x%02x%02x\n", *(buff + 1), *(buff + 0));
    printf("\tNumberOfSections:                 0x%02x%02x\n", *(buff + 3), *(buff + 2));
    printf("\tTimeDateStamp:                    0x%02x%02x%02x%02x\n", *(buff + 7), *(buff + 6), *(buff + 5), *(buff + 4));
    printf("\tPointerToSymbolTable:             0x%02x%02x%02x%02x\n", *(buff + 11), *(buff + 10), *(buff + 9), *(buff + 8));
    printf("\tNumberOfSymbols:                  0x%02x%02x%02x%02x\n", *(buff + 15), *(buff + 14), *(buff + 13), *(buff + 12));
    printf("\tSizeOfOptionalHeader:             0x%02x%02x\n", *(buff + 17), *(buff + 16));
    printf("\tCharacteristics:                  0x%02x%02x\n", *(buff + 19), *(buff + 18));
    pe->num_sections = (*(buff + 2)) + (*(buff + 3) << 8);
    pe->optional_header_size = (*(buff + 16)) + (*(buff + 17) << 8);
    printf("\n");
}

void printoptional(const unsigned char *buff, struct optional_header_information *optional)
{
    printf("Optional header\n===============\n");
    printf("\tDescription:                      Data:           Value / Address:\n");
    printf("\t--------------------------------------------------------------------------------------\n");
    printf("\tMagic:                            0x%02x%02x\n", *(buff + 1), *(buff));
    printf("\tMajorLinkerVersion:               0x%02x\n", *(buff + 2));
    printf("\tMinorLinkerVersion:               0x%02x\n", *(buff + 3));
    printf("\tSizeOfCode:                       0x%02x%02x%02x%02x\n", *(buff + 7), *(buff + 6), *(buff + 5), *(buff + 4));
    printf("\tSizeOfInitializedData:            0x%02x%02x%02x%02x\n", *(buff + 11), *(buff + 10), *(buff + 9), *(buff + 8));
    printf("\tSizeOfUnitializedData:            0x%02x%02x%02x%02x\n", *(buff + 15), *(buff + 14), *(buff + 13), *(buff + 12));
    printf("\tAddressOfEntryPoint:              0x%02x%02x%02x%02x\n", *(buff + 19), *(buff + 18), *(buff + 17), *(buff + 16));
    printf("\tBaseOfCode:                       0x%02x%02x%02x%02x\n", *(buff + 23), *(buff + 22), *(buff + 21), *(buff + 20));
    printf("\tBaseOfData:                       0x%02x%02x%02x%02x\n", *(buff + 27), *(buff + 26), *(buff + 25), *(buff + 24));
    printf("\tImageBase:                        0x%02x%02x%02x%02x\n", *(buff + 31), *(buff + 30), *(buff + 29), *(buff + 28));
    printf("\tSectionAlignment:                 0x%02x%02x%02x%02x\n", *(buff + 35), *(buff + 34), *(buff + 33), *(buff + 32));
    printf("\tFileAlignment:                    0x%02x%02x%02x%02x\n", *(buff + 39), *(buff + 38), *(buff + 37), *(buff + 36));
    printf("\tMajorOperatingSystemVersion:      0x%02x%02x\n", *(buff + 41), *(buff + 40));
    printf("\tMinorOperatingSystemVersion:      0x%02x%02x\n", *(buff + 43), *(buff + 42));
    printf("\tMajorImageVersion:                0x%02x%02x\n", *(buff + 45), *(buff + 44));
    printf("\tMinorImageVersion:                0x%02x%02x\n", *(buff + 47), *(buff + 46));
    printf("\tMajorSubsystemVersion:            0x%02x%02x\n", *(buff + 49), *(buff + 48));
    printf("\tMinorSubsystemVersion:            0x%02x%02x\n", *(buff + 51), *(buff + 50));
    printf("\tWin32VersionValue:                0x%02x%02x%02x%02x\n", *(buff + 55), *(buff + 54), *(buff + 53), *(buff + 52));
    printf("\tSizeOfImage:                      0x%02x%02x%02x%02x\n", *(buff + 59), *(buff + 58), *(buff + 57), *(buff + 56));
    printf("\tSizeOfHeaders:                    0x%02x%02x%02x%02x\n", *(buff + 63), *(buff + 62), *(buff + 61), *(buff + 60));
    printf("\tCheckSum:                         0x%02x%02x%02x%02x\n", *(buff + 67), *(buff + 66), *(buff + 65), *(buff + 64));
    printf("\tSubsystem:                        0x%02x%02x\n", *(buff + 69), *(buff + 68));
    printf("\tDLLCharacteristics:               0x%02x%02x\n", *(buff + 71), *(buff + 70));
    printf("\tSizeOfStackReserve:               0x%02x%02x%02x%02x\n", *(buff + 75), *(buff + 74), *(buff + 73), *(buff + 72));
    printf("\tSizeOfStackCommit:                0x%02x%02x%02x%02x\n", *(buff + 79), *(buff + 78), *(buff + 77), *(buff + 76));
    printf("\tSizeOfHeapReserve:                0x%02x%02x%02x%02x\n", *(buff + 83), *(buff + 82), *(buff + 81), *(buff + 80));
    printf("\tSizeOfHeapCommit:                 0x%02x%02x%02x%02x\n", *(buff + 87), *(buff + 86), *(buff + 85), *(buff + 84));
    printf("\tLoaderFlags:                      0x%02x%02x%02x%02x\n", *(buff + 91), *(buff + 90), *(buff + 89), *(buff + 88));
    printf("\tNumberOfRvaAndSizes:              0x%02x%02x%02x%02x\n", *(buff + 95), *(buff + 94), *(buff + 93), *(buff + 92));
    printf("\t======================================================================================\n");
    printf("\tRVA:                              0x%02x%02x%02x%02x --> EXPORT table\n", *(buff + 99), *(buff + 98), *(buff + 97), *(buff + 96));
    printf("\tSize:                             0x%02x%02x%02x%02x\n", *(buff + 103), *(buff + 102), *(buff + 101), *(buff + 100));
    printf("\t======================================================================================\n");
    printf("\tRVA:                              0x%02x%02x%02x%02x --> IMPORT table\n", *(buff + 107), *(buff + 106), *(buff + 105), *(buff + 104));
    printf("\tSize:                             0x%02x%02x%02x%02x\n", *(buff + 111), *(buff + 110), *(buff + 109), *(buff + 108));
    printf("\t======================================================================================\n");
    printf("\tRVA:                              0x%02x%02x%02x%02x --> RESOURCE table\n", *(buff + 115), *(buff + 114), *(buff + 113), *(buff + 112));
    printf("\tSize:                             0x%02x%02x%02x%02x\n", *(buff + 119), *(buff + 118), *(buff + 117), *(buff + 116));
    printf("\t======================================================================================\n");
    printf("\tRVA:                              0x%02x%02x%02x%02x --> EXCEPTION table\n", *(buff + 123), *(buff + 122), *(buff + 121), *(buff + 120));
    printf("\tSize:                             0x%02x%02x%02x%02x\n", *(buff + 127), *(buff + 126), *(buff + 125), *(buff + 124));
    printf("\t======================================================================================\n");
    printf("\tRVA:                              0x%02x%02x%02x%02x --> CERTIFICATE table\n", *(buff + 131), *(buff + 130), *(buff + 129), *(buff + 128));
    printf("\tSize:                             0x%02x%02x%02x%02x\n", *(buff + 135), *(buff + 134), *(buff + 133), *(buff + 132));
    printf("\t======================================================================================\n");
    printf("\tRVA:                              0x%02x%02x%02x%02x --> BASE RELOCATION table\n", *(buff + 139), *(buff + 138), *(buff + 137), *(buff + 136));
    printf("\tSize:                             0x%02x%02x%02x%02x\n", *(buff + 143), *(buff + 142), *(buff + 141), *(buff + 140));
    printf("\t======================================================================================\n");
    printf("\tRVA:                              0x%02x%02x%02x%02x --> DEBUG directory\n", *(buff + 147), *(buff + 146), *(buff + 145), *(buff + 144));
    printf("\tSize:                             0x%02x%02x%02x%02x\n", *(buff + 151), *(buff + 150), *(buff + 149), *(buff + 148));
    printf("\t======================================================================================\n");
    printf("\tRVA:                              0x%02x%02x%02x%02x --> Architecture Specific Data\n", *(buff + 155), *(buff + 154), *(buff + 153), *(buff + 152));
    printf("\tSize:                             0x%02x%02x%02x%02x\n", *(buff + 159), *(buff + 158), *(buff + 157), *(buff + 156));
    printf("\t======================================================================================\n");
    printf("\tRVA:                              0x%02x%02x%02x%02x --> GLOBAL POINTER Register\n", *(buff + 163), *(buff + 162), *(buff + 161), *(buff + 160));
    printf("\tSize:                             0x%02x%02x%02x%02x\n", *(buff + 167), *(buff + 166), *(buff + 165), *(buff + 164));
    printf("\t======================================================================================\n");
    printf("\tRVA:                              0x%02x%02x%02x%02x --> TLS table\n", *(buff + 171), *(buff + 170), *(buff + 169), *(buff + 168));
    printf("\tSize:                             0x%02x%02x%02x%02x\n", *(buff + 175), *(buff + 174), *(buff + 173), *(buff + 172));
    printf("\t======================================================================================\n");
    printf("\tRVA:                              0x%02x%02x%02x%02x --> LOAD CONFIGURATION table\n", *(buff + 179), *(buff + 178), *(buff + 177), *(buff + 176));
    printf("\tSize:                             0x%02x%02x%02x%02x\n", *(buff + 183), *(buff + 182), *(buff + 181), *(buff + 180));
    printf("\t======================================================================================\n");
    printf("\tRVA:                              0x%02x%02x%02x%02x --> BOUND IMPORT table\n", *(buff + 187), *(buff + 186), *(buff + 185), *(buff + 184));
    printf("\tSize:                             0x%02x%02x%02x%02x\n", *(buff + 191), *(buff + 190), *(buff + 189), *(buff + 188));
    printf("\t======================================================================================\n");
    printf("\tRVA:                              0x%02x%02x%02x%02x --> IMPORT adress table\n", *(buff + 195), *(buff + 194), *(buff + 193), *(buff + 192));
    printf("\tSize:                             0x%02x%02x%02x%02x\n", *(buff + 199), *(buff + 198), *(buff + 197), *(buff + 196));
    printf("\t======================================================================================\n");
    printf("\tRVA:                              0x%02x%02x%02x%02x --> DELAY IMPORT descriptors\n", *(buff + 203), *(buff + 202), *(buff + 201), *(buff + 200));
    printf("\tSize:                             0x%02x%02x%02x%02x\n", *(buff + 207), *(buff + 206), *(buff + 205), *(buff + 204));
    printf("\t======================================================================================\n");
    printf("\tRVA:                              0x%02x%02x%02x%02x --> CLI header\n", *(buff + 211), *(buff + 210), *(buff + 209), *(buff + 208));
    printf("\tSize:                             0x%02x%02x%02x%02x\n", *(buff + 215), *(buff + 214), *(buff + 213), *(buff + 212));
    printf("\t======================================================================================\n");
    printf("\tRVA:                              0x%02x%02x%02x%02x\n", *(buff + 219), *(buff + 218), *(buff + 217), *(buff + 216));
    printf("\tSize:                             0x%02x%02x%02x%02x\n", *(buff + 223), *(buff + 222), *(buff + 221), *(buff + 220));
    printf("\t======================================================================================\n");
    printf("\n");

    unsigned char *data_dir_adr = buff + 96;
    for (int i = 0; i < DIR_NUMBER; i++)
    {
        optional->data_directories_rvasize[i][0] = (*(data_dir_adr)) + (*(data_dir_adr + 1) << 8) + (*(data_dir_adr + 2) << 16) + (*(data_dir_adr + 3) << 24);
        optional->data_directories_rvasize[i][1] = (*(data_dir_adr + 4)) + (*(data_dir_adr + 5) << 8) + (*(data_dir_adr + 6) << 16) + (*(data_dir_adr + 7) << 24);
        data_dir_adr += 8;
    }
}

void printsection(const unsigned char *buff, const int num_sections, struct section_header_information *sections)
{
    printf("Section Headers\n===============\n");
    for (int i = 0; i < num_sections; i++)
    {
        printf("\tSection Header %c%c%c%c%c%c%c%c\n", *(buff + 0), *(buff + 1), *(buff + 2), *(buff + 3), *(buff + 4), *(buff + 5), *(buff + 6), *(buff + 7));
        printf("\t\tDescription:                      Data:           Value / Address:\n");
        printf("\t\t--------------------------------------------------------------------------------------\n");
        printf("\t\tName:                             0x%02x%02x%02x%02x --> %c%c%c%c%c%c%c%c\n", *(buff + 0), *(buff + 1), *(buff + 2), *(buff + 3), *(buff + 0), *(buff + 1), *(buff + 2), *(buff + 3), *(buff + 4), *(buff + 5), *(buff + 6), *(buff + 7));
        printf("\t\tName:                             0x%02x%02x%02x%02x\n", *(buff + 4), *(buff + 5), *(buff + 6), *(buff + 7));
        printf("\t\tVirtualSize:                      0x%02x%02x%02x%02x\n", *(buff + 11), *(buff + 10), *(buff + 9), *(buff + 8));
        printf("\t\tRVA:                              0x%02x%02x%02x%02x\n", *(buff + 15), *(buff + 14), *(buff + 13), *(buff + 12));
        printf("\t\tSizeOfRawData:                    0x%02x%02x%02x%02x\n", *(buff + 19), *(buff + 18), *(buff + 17), *(buff + 16));
        printf("\t\tPointerToRawData:                 0x%02x%02x%02x%02x\n", *(buff + 23), *(buff + 22), *(buff + 21), *(buff + 20));
        printf("\t\tPointerToRelocations:             0x%02x%02x%02x%02x\n", *(buff + 27), *(buff + 26), *(buff + 25), *(buff + 24));
        printf("\t\tPointerToLineNumbers:             0x%02x%02x%02x%02x\n", *(buff + 31), *(buff + 30), *(buff + 29), *(buff + 28));
        printf("\t\tNumberOfRelocations:              0x%02x%02x\n", *(buff + 33), *(buff + 32));
        printf("\t\tNumberOfLineNumbers:              0x%02x%02x\n", *(buff + 35), *(buff + 34));
        printf("\t\tCharacteristics:                  0x%02x%02x%02x%02x\n", *(buff + 39), *(buff + 38), *(buff + 37), *(buff + 36));
        printf("\t==============================================================================================\n");

        sections[i].section_rva = (*(buff + 12)) + (*(buff + 13) << 8) + (*(buff + 14) << 16) + (*(buff + 15) << 24);
        sections[i].section_size = (*(buff + 16)) + (*(buff + 17) << 8) + (*(buff + 18) << 16) + (*(buff + 19) << 24);
        sections[i].section_pdata = (*(buff + 20)) + (*(buff + 21) << 8) + (*(buff + 22) << 16) + (*(buff + 23) << 24);

        buff += 36 + 4;
    }
    printf("\n");
}

void calculate_directory_address(struct optional_header_information *optional, const struct section_header_information *sections, int n) 
{
    for (int i = 0; i < DIR_NUMBER; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (optional->data_directories_rvasize[i][0] > sections[j].section_rva && optional->data_directories_rvasize[i][1] < (sections[j].section_rva + sections[j].section_size) && optional->data_directories_rvasize[i][0] != 0)
            {
                optional->data_directories_rvasize[i][2] = optional->data_directories_rvasize[i][0] - sections[j].section_rva + sections[j].section_pdata;
                optional->data_directories_rvasize[i][3] = -sections[j].section_rva + sections[j].section_pdata;
            }
            else if (!(optional->data_directories_rvasize[i][0] != 0))
            {
                optional->data_directories_rvasize[i][2] = 0;
                optional->data_directories_rvasize[i][3] = -1;
            }
        }
    }
}

void printimport(const unsigned char *buff, int dir_size, int dir_offset, int rvacalc)
{
    printf("Import table\n============\n");
    unsigned char *pointer = buff + dir_offset, libName[LIB_API_NAME_LEN], hint[2];
    int name_addr, addr_table_rva, api_addr, counter = 0, breakcount, countwhile = 0;
    while (countwhile < dir_size)
    {
        for (int i = 0; i < 5; i++)         //for additional checking has the pointer reached the end of import directory table, checks 5 dwords (32 bits).
        {
            if ((*(pointer + 3 + i * 4) << 24) == 0x00 && (*(pointer + 2 + i * 4) << 16) == 0x00 && (*(pointer + 1 + i * 4) << 8) == 0x00 && (*(pointer + i * 4)) == 0x00)
            {
                breakcount++;
            }
        }
        if (breakcount == 5)
            break;
        breakcount = 0;
        printf("\tImport directory\n\t================\n");
        printf("\t\tDescription:                      Data:           Value / Address:\n");
        printf("\t\t--------------------------------------------------------------------------------------\n");
        printf("\t\tImportNameTableRVA:               0x%02x%02x%02x%02x\n", *(pointer + 3), *(pointer + 2), *(pointer + 1), *(pointer));
        printf("\t\tTimeDateStamp:                    0x%02x%02x%02x%02x\n", *(pointer + 7), *(pointer + 6), *(pointer + 5), *(pointer + 4));
        printf("\t\tForwarderChain:                   0x%02x%02x%02x%02x\n", *(pointer + 11), *(pointer + 10), *(pointer + 9), *(pointer + 8));
        name_addr = (*(pointer + 12)) + (*(pointer + 13) << 8) + (*(pointer + 14) << 16) + (*(pointer + 15) << 24) + rvacalc;
        for (int j = 0; j < LIB_API_NAME_LEN && counter < 2; j++)       // library name generation
        {
            if (*(buff + name_addr + j) == 0x00)
            {
                counter++;
                libName[j] = 0x00;
            }
            else
            {
                libName[j] = *(buff + name_addr + j);
            }
        }
        counter = 0;
        printf("\t\tNameRVA:                          0x%02x%02x%02x%02x --> %s\n", *(pointer + 15), *(pointer + 14), *(pointer + 13), *(pointer + 12), libName);
        printf("\t\tImportAdressTableRVA:             0x%02x%02x%02x%02x\n", *(pointer + 19), *(pointer + 18), *(pointer + 17), *(pointer + 16));
        printf("\t\tImport thunks\n\t\t=============\n");
        printf("\t\t\tDescription:              RVA:           Address:       Hint      Name:\n");
        printf("\t\t\t--------------------------------------------------------------------------------------\n");
        if ((*(pointer + 3) > 0x00) || (*(pointer + 2) > 0x00) || (*(pointer + 1) > 0x00) || (*(pointer + 0) > 0x00))
        { // is there original import thunk, or only first thunk?
            addr_table_rva = (*(pointer + 3) << 24) + (*(pointer + 2) << 16) + (*(pointer + 1) << 8) + (*(pointer + 0)) + rvacalc;
        }
        else
        {
            addr_table_rva = (*(pointer + 19) << 24) + (*(pointer + 18) << 16) + (*(pointer + 17) << 8) + (*(pointer + 16)) + rvacalc;
        }
        while ((*(buff + addr_table_rva + 3) != 0x00) || (*(buff + addr_table_rva + 2) != 0x00) || (*(buff + addr_table_rva + 1) != 0x00) || (*(buff + addr_table_rva) != 0x00))
        { // finding hint and name of api
            api_addr = (*(buff + addr_table_rva + 3) << 24) + (*(buff + addr_table_rva + 2) << 16) + (*(buff + addr_table_rva + 1) << 8) + (*(buff + addr_table_rva)) + rvacalc;
            hint[0] = *(buff + api_addr);
            hint[1] = *(buff + api_addr + 1);
            api_addr += 2;
            for (int j = 0; j < LIB_API_NAME_LEN && counter < 1; j++)       // function name generation
            {
                if (*(buff + api_addr + j) == 0x00)
                {
                    counter++;
                    libName[j] = 0x00;
                }
                else
                {
                    libName[j] = *(buff + api_addr + j);
                }
            }
            counter = 0;
            printf("\t\t\tApi:                      0x%08x     0x%08x     0x%02x%02x    %s\n", api_addr - rvacalc - 2, api_addr-2, hint[1], hint[0], libName); // -2 because of hint +2
            addr_table_rva += 4;
        }

        countwhile += 0x14;
        pointer += 0x14;
    }
    printf("\n");
}

void printexport(const unsigned char *buff, int dir_size, int dir_offset, int rvacalc)
{
    unsigned char *pointer = buff + dir_offset, libName[LIB_API_NAME_LEN];                  // maybe a bit too many variables?!??!?!?!?!
    int name_addr, counter = 0, num_func, num_names, ord_base, addr_table_rva, name_ptr_rva, ord_table_rva, addr_table_data, name_ptr_data, diff_count = 0;
    printf("Export table\n============\n");
    printf("\tExport directory\n\t================\n");
    printf("\tDescription:                      Data:           Value / Address:\n");
    printf("\t--------------------------------------------------------------------------------------\n");
    printf("\tCharacteristics:                  0x%02x%02x%02x%02x\n", *(pointer + 3), *(pointer + 2), *(pointer + 1), *(pointer));
    printf("\tTimeDateStamp:                    0x%02x%02x%02x%02x\n", *(pointer + 7), *(pointer + 6), *(pointer + 5), *(pointer + 4));
    printf("\tMajorVersion:                     0x%02x%02x\n", *(pointer + 9), *(pointer + 8));
    printf("\tMinorVersion:                     0x%02x%02x\n", *(pointer + 11), *(pointer + 10));
    name_addr = (*(pointer + 15) << 24) + (*(pointer + 14) << 16) + (*(pointer + 13) << 8) + (*(pointer + 12)) + rvacalc;       
    for (int j = 0; j < LIB_API_NAME_LEN && counter < 2; j++)
    {
        if (*(buff + name_addr + j) == 0x00)
        {
            counter++;
            libName[j] = 0x00;
        }
        else
        {
            libName[j] = *(buff + name_addr + j);
        }
    }
    counter = 0;
    printf("\tNameRva:                          0x%02x%02x%02x%02x --> %s\n", *(pointer + 15), *(pointer + 14), *(pointer + 13), *(pointer + 12), libName);
    ord_base = (*(pointer + 19) << 24) + (*(pointer + 18) << 16) + (*(pointer + 17) << 8) + (*(pointer + 16));
    printf("\tOrdinalBase:                      0x%02x%02x%02x%02x\n", *(pointer + 19), *(pointer + 18), *(pointer + 17), *(pointer + 16));
    num_func = (*(pointer + 23) << 24) + (*(pointer + 22) << 16) + (*(pointer + 21) << 8) + (*(pointer + 20));
    printf("\tNumberOfFunctions:                0x%02x%02x%02x%02x\n", *(pointer + 23), *(pointer + 22), *(pointer + 21), *(pointer + 20));
    char buffer2[num_func][200];
    num_names = (*(pointer + 27) << 24) + (*(pointer + 26) << 16) + (*(pointer + 25) << 8) + (*(pointer + 24));
    printf("\tNumberOfNames:                    0x%02x%02x%02x%02x\n", *(pointer + 27), *(pointer + 26), *(pointer + 25), *(pointer + 24));
    addr_table_rva = (*(pointer + 31) << 24) + (*(pointer + 30) << 16) + (*(pointer + 29) << 8) + (*(pointer + 28)) + rvacalc;
    printf("\tAdressTableRVA:                   0x%02x%02x%02x%02x\n", *(pointer + 31), *(pointer + 30), *(pointer + 29), *(pointer + 28));
    name_ptr_rva = (*(pointer + 35) << 24) + (*(pointer + 34) << 16) + (*(pointer + 33) << 8) + (*(pointer + 32)) + rvacalc;
    printf("\tNamePointerTableRVA:              0x%02x%02x%02x%02x\n", *(pointer + 35), *(pointer + 34), *(pointer + 33), *(pointer + 32));
    ord_table_rva = (*(pointer + 39) << 24) + (*(pointer + 38) << 16) + (*(pointer + 37) << 8) + (*(pointer + 36)) + rvacalc;
    printf("\tOrdinalTableRVA:                  0x%02x%02x%02x%02x\n", *(pointer + 39), *(pointer + 38), *(pointer + 37), *(pointer + 36));
    diff_count = 0;

    char names[num_func][LIB_API_NAME_LEN];    // array of structs???
    int ordinals[num_func];
    int addresses[num_func];
    int name_addresses[num_func];
    for (int k = 0; k < num_func; k++)
    {
        addr_table_data = (*(buff + addr_table_rva + k * 4 + 3) << 24) + (*(buff + addr_table_rva + k * 4 + 2) << 16) + (*(buff + addr_table_rva + k * 4 + 1) << 8) + (*(buff + addr_table_rva + k * 4 + 0));
        addresses[k] = addr_table_data;
        if (addr_table_data != 0)
        {
            name_ptr_data = (*(buff + name_ptr_rva + diff_count * 4 + 3) << 24) + (*(buff + name_ptr_rva + diff_count * 4 + 2) << 16) + (*(buff + name_ptr_rva + diff_count * 4 + 1) << 8) + (*(buff + name_ptr_rva + diff_count * 4 + 0)) + rvacalc;
            name_addresses[k] = name_ptr_data - rvacalc;
            for (int j = 0; j < LIB_API_NAME_LEN && counter < 1; j++)
            {
                if (*(buff + name_ptr_data + j) == 0x00)
                {
                    counter++;
                    names[k][j] = 0x00;
                }
                else
                {
                    names[k][j] = *(buff + name_ptr_data + j);
                }
            }
            counter = 0;
            ordinals[k] = (*(buff + ord_table_rva + diff_count * 2 + 1) << 8) + (*(buff + ord_table_rva + diff_count * 2) + 1);
            diff_count++;
        }
        else
        {
            for (int j = 0; j < LIB_API_NAME_LEN; j++)
            {
                names[k][j] = 0x00;
            }
            counter = 0;
            ordinals[k] = k+1;
        }
    }

    int order[num_func], ord_copy[num_func];        //for sorted output of address table by ordinals
    for(int h = 0; h < num_func; h++) {
        order[h] = h;
        ord_copy[h] = ordinals[h];
    }
    { // address table
        printf("\tExport address table\n\t====================\n");
        printf("\t\tDescription:              RVA:           Address:       Ordinal:     Name:\n");
        printf("\t\t--------------------------------------------------------------------------------------\n");
        for (int k = 0; k < num_func - 1; k++)
        {
            for (int h = 0; h < num_func - k - 1; h++)
            {
                if(ord_copy[h] > ord_copy[h+1] ){
                    int temp = order[h];
                    order[h] = order[h+1];
                    order[h+1] = temp;
                    int temp2 = ord_copy[h];
                    ord_copy[h] = ord_copy[h+1];
                    ord_copy[h+1] = temp2;
                }
            }
        }
        for(int k = 0; k < num_func ; k++){
            if (!(addresses[k] >= dir_offset && addresses[k] < dir_offset + dir_size))
            { // if the data is outside of export directory section --> function rva, else --> forwaded name rva
                printf("\t\tApi:                      0x%08x     0x%08x     0x%04x       %s \n", addresses[k], addresses[k] > 0 ? addresses[k]+rvacalc : 0, ordinals[order[k]], names[order[k]]);
            }
            else
            {
                printf("\t\tForwardedName             0x%08x     0x%08x     0x%04x       %s \n", addresses[k], addresses[k]+rvacalc, ordinals[order[k]], names[order[k]]);
            }
        }
    }
    { // function name table
        printf("\tExport function name table\n\t==========================\n");
        printf("\t\tDescription:              RVA:           Address:       Ordinal:     Name:\n");
        printf("\t\t--------------------------------------------------------------------------------------\n");
        for(int k = 0; k < num_func; k++){
            if(addresses[k] > 0){
                printf("\t\tApi:                      0x%08x     0x%08x     0x%04x       %s \n", name_addresses[k], name_addresses[k] > 0 ? name_addresses[k]+rvacalc : 0, ordinals[k], names[k]);
            }
        }
    }
    { // ordinal table
        printf("\tExport ordinal table\n\t====================\n");
        printf("\t\tDescription:              Ordinal:       Decoded Ordinal:            Name:\n");
        printf("\t\t--------------------------------------------------------------------------------------\n");
        for(int k = 0; k < num_func; k++){
            if(addresses[k] > 0){
                printf("\t\tApi:                      0x%04x         0x%04x                      %s \n", ordinals[k]-ord_base, ordinals[k], names[k]);
            }
        }
    }
}

int main(int argc, char **argv)
{
    unsigned char file_name[30], *buffer;
    int pe, section_header, optional_header;
    long file_size;
    FILE *input_file;

    struct pe_header_information pe_info;                  
    struct optional_header_information optional_info;

    printf("Enter name of PE: ");
    scanf("%s", &file_name);
    input_file = fopen(file_name, "rb");

    fseek(input_file, 0, SEEK_END);
    file_size = ftell(input_file);
    rewind(input_file);
    buffer = (unsigned char *)malloc(sizeof(char) * file_size);
    fread(buffer, 1, file_size, input_file);
    fclose(input_file);

    if (*(buffer) != 0x4d && *(buffer + 1) != 0x5a)
    {
        printf("Invalid MZ!\n");
        free(buffer);
        return 0;
    }
    pe = printmz(buffer);
    if (*(buffer + pe) != 0x50 && *(buffer + pe + 1) != 0x45)
    {
        printf("Invalid PE!\n");
        free(buffer);
        return 0;
    }
    if (*(buffer + pe + 23) != 0x01 && *(buffer + pe + 24) != 0x0b)
    {
        printf("File is not PE32 format!\n");
        free(buffer);
        return 0;
    }
    printpe(buffer + pe + 4, &pe_info);
    optional_header = pe + 20 + 4;
    printoptional(buffer + optional_header, &optional_info);
    struct section_header_information sections_array[pe_info.num_sections];
    section_header = 20 + pe + 4 + pe_info.optional_header_size;                    
    printsection(buffer + section_header, pe_info.num_sections, sections_array);      
    calculate_directory_address(&optional_info, sections_array, pe_info.num_sections);          // caluclates in which section each directory is.
    if (optional_info.data_directories_rvasize[1][1] > 0)
    {
        printimport(buffer, optional_info.data_directories_rvasize[1][1], optional_info.data_directories_rvasize[1][2], optional_info.data_directories_rvasize[1][3]);
    }
    if (optional_info.data_directories_rvasize[0][1] > 0)
    {
        printexport(buffer, optional_info.data_directories_rvasize[0][1], optional_info.data_directories_rvasize[0][2], optional_info.data_directories_rvasize[0][3]);
    }

    free(buffer);
    return 0;
}


// my apologies for the bad code.