#include <stdio.h>
#include "elfparser.h"

int main(int argc, char* argv[]) {

    if(argc <= 1) {
        printf("Provide an .out file for parsing");
        return 1;
    }
    
    FILE* out = fopen(argv[1], "rb");
    if(!out) {
        printf("File not found");
        return 1;
    }
    
    elfparser(out);


    return 0;
}

