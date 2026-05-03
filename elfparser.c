#include <stdio.h>
#include <stdlib.h>

#include "elfparser.h"

void elfparser(FILE* file) {

    void* header = malloc(sizeof(elf_header));
    
    fread(header, sizeof(elf_header), 1, file);
    
    elf_header* h = (elf_header*) header;

    printf("%i", h->e_version);

    fclose(file);
}
