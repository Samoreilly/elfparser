#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
/*
    https://en.wikipedia.org/wiki/Executable_and_Linkable_Format
*/


//__attribute__((packed)) removed padding between members

typedef struct __attribute__((packed)) {
    
    uint32_t magic; //0x00 - 0x03
    uint8_t ei_class; //0x04
    uint8_t ei_data; //0x05
    uint8_t ei_version; //0x06
    uint8_t ei_osabi; //0x07
    uint64_t abi_and_pad; //0x08 and 7 bytes padding
    uint16_t e_type; //0x10
    uint16_t e_machine; //0x12
    uint32_t e_version; //0x14
    uint64_t e_entry; //entry point of processes instructions
    uint64_t e_phoff; //header table pointer
    uint64_t e_shoff; //section header table
    uint32_t flags; 
    uint16_t e_ehsize; // size of header
    uint16_t e_phentsize;// size of program header table
    uint16_t e_phnum; //number of entries in program header table
    uint16_t e_shentsize; // size of section header table
    uint16_t e_shnum; // number of entries in section header table
    uint16_t e_shstrndx; // index of section header table

} elf_header;


void elfparser(FILE* file);

