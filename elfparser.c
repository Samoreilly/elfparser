#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "elfparser.h"

void elfparser(FILE* file) {

    void* header = malloc(sizeof(elf_header));

    fread(header, sizeof(elf_header), 1, file);

    elf_header* h = (elf_header*) header;

    if(!validate_magicnumber(h)) {
        printf("Invalid magic number");
        return;
    }

    printf("\n\nElf Header Information:\n\n");

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
    


    fclose(file);
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

