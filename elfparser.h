#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
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
    uint32_t e_flags; 
    uint16_t e_ehsize; // size of header
    uint16_t e_phentsize;// size of program header table
    uint16_t e_phnum; //number of entries in program header table
    uint16_t e_shentsize; // size of section header table
    uint16_t e_shnum; // number of entries in section header table
    uint16_t e_shstrndx; // index of section header table

} elf_header;

typedef struct __attribute__((packed)) {

    uint32_t p_type;
    uint32_t p_flags;
    uint64_t p_offset;
    uint64_t p_vaddr;
    uint64_t p_paddr;
    uint64_t p_filesz;
    uint64_t p_memsz;
    uint64_t p_align;

} program_header;

typedef struct __attribute__((packed)) {

    uint32_t sh_name;
    uint32_t sh_type;
    uint64_t sh_flags;
    uint64_t sh_addr;
    uint64_t sh_offset;
    uint64_t sh_size;
    uint32_t sh_link;
    uint32_t sh_info;
    uint64_t sh_addralign;
    uint64_t sh_entsize;

} section_header;

int elfparser(FILE* file);

char* get_string_table(FILE* file, elf_header* header);
char* get_section_name(FILE* file, char* string_table, section_header* section_header);
int program_table(FILE* file, elf_header* header);
int section_header_table(FILE* file, elf_header* header);

bool validate_magicnumber(elf_header* header);


