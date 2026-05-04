#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "elfparser.h"

int elfparser(FILE* file) {

    void* header = malloc(sizeof(elf_header));

    fread(header, sizeof(elf_header), 1, file);

    elf_header* h = (elf_header*) header;

    if(!validate_magicnumber(h)) {
        printf("Invalid magic number");
        return 1;
    }

    printf("\e[31m\n\nElf Header Information:\n\n\e[0m");

    printf("magic: %i\n", h->magic);
    printf("e_class: %i\n", h->ei_class);
    printf("e_data [1 = little-endianness | 2 = big-endianness]: %i\n", h->ei_data);
    printf("ei_version: %i\n", h->ei_version);
    printf("ei_osabi: %u\n", h->ei_osabi);
    //printf("ei_abiversion: %x\n", h->abi_and_pad);
    printf("e_type: %u\n", h->e_type);
    printf("e_machine: %x\n", h->e_machine);
    printf("e_version: %i\n", h->e_version);
    printf("e_entry: %lx\n", h->e_entry);

    printf("e_phoff: %lx\n",h->e_phoff);
    printf("e_shoff: %lx\n", h->e_shoff);
    printf("e_flags: %u\n", h->e_flags);

    printf("e_ehsize: %i\n", h->e_ehsize);
    printf("e_phentsize: %i\n", h->e_phentsize);
    printf("e_phnum: %i\n", h->e_phnum);
    
    printf("e_shentsize: %i\n", h->e_shentsize);
    printf("e_shnum: %i\n", h->e_shnum);
    printf("e_shstrndx: %i\n", h->e_shstrndx);
    
    int program_sig = program_table(file, h);
    int section_sig = section_header_table(file, h);

    if(program_sig != 0 || section_sig != 0) return 1;
    
    free(h);
    fclose(file);

    return 0;
}

int program_table(FILE* file, elf_header* header) {

    if(fseek(file, header->e_phoff, SEEK_SET) != 0) {
        fclose(file);
        return 1;
    }
    
    program_header* program = (program_header*) malloc(sizeof(program_header));
   
    printf("\e[31m%s", "\n\nProgram Header Table\n\n\e[0m");

    for(int i = 0;i < header->e_phnum;i++) {

        fread(program, sizeof(program_header), 1, file);

        printf("p_type: %x\n", program->p_type);
        printf("p_flags: %x\n", program->p_flags);
        printf("p_offset: %lx\n", program->p_offset);
        printf("p_vaddr: %lx\n", program->p_vaddr);
        printf("p_paddr: %lx\n", program->p_paddr);
        printf("p_filesz: %lx\n", program->p_filesz);
        printf("p_memsz: %lx\n", program->p_memsz);
        printf("p_align: 0x%lx\n", program->p_align);    

        printf("\n");
    }

    free(program);

    return 0;    
}

char* get_string_table(FILE* file, elf_header* header) {

    fseek(file, header->e_shoff + (header->e_shstrndx * header->e_shentsize), SEEK_SET);
    
    section_header* section_names = (section_header*) malloc(sizeof(section_header));
    
    //read the string table in
    fread(section_names, sizeof(section_header), 1, file);

    char* string_table = (char*) malloc(section_names->sh_size);
   
    fseek(file, section_names->sh_offset, SEEK_SET); 
    fread(string_table, section_names->sh_size, 1, file);

    return string_table;
}

char* get_section_name(FILE* file, char* string_table, section_header* section) {
   
    char* res = malloc(64); 
    char* dest = res;
    char* src = (string_table + section->sh_name);
    
    while(*src != '\0') {
        *dest++ = *src++;
    }

    *dest = '\0';
    return res;
}

int section_header_table(FILE* file, elf_header* header) {

    if(fseek(file, header->e_shoff, SEEK_SET) != 0) {
        fclose(file);
        return 1;
    }

    //string table
    
    long offset = ftell(file);
    char* string_table = get_string_table(file, header);
    
    fseek(file, offset, SEEK_SET);

    section_header* section = (section_header*) malloc(sizeof(section_header));

    printf("\e[31m%s", "\n\nSection Header Table\n\n\e[0m");

    for(int i = 0;i < header->e_shnum;i++) {

        fread(section, sizeof(section_header), 1, file);
        
        char* section_name = get_section_name(file, string_table, section);

        printf("\e[31m Section Name: %s\e[0m\n", section_name);
        free(section_name);

        printf("sh_name: %x\n", section->sh_name);
        printf("sh_type: %x\n", section->sh_type);
        printf("sh_flags: %lx\n", section->sh_flags);
        printf("sh_addr: %lx\n", section->sh_addr);
        printf("sh_offset: %lx\n", section->sh_offset);
        printf("sh_size %lx\n", section->sh_size);
        printf("sh_link: %x\n", section->sh_link);
        printf("sh_info: %x\n", section->sh_info);
        printf("sh_addralign: %lx\n", section->sh_addralign);
        printf("sh_entsize: %lx\n", section->sh_entsize);

        printf("\n\n");

        
    }

    free(section);
    
    return 0;
}

//format 0x7F 45 4c 46
bool validate_magicnumber(elf_header* header) {

    uint8_t magic[4] = { 0x7F, 'E', 'L', 'F' };

    for (int i = 0; i < 4; i++) { 
        uint8_t segment = (header->magic >> (i * 8)) & 0xFF;     
        if (segment != magic[i]) return false;
    }

    return true;
}

