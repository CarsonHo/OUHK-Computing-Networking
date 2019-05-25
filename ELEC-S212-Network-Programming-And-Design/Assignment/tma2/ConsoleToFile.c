//
//  main.c
//  tma02
//
//  Created by CarsonHo on 7/1/2019.
//  Copyright © 2019 CarsonHo. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

void flush_input_buffer(){
    while  (getchar()  !=  '\n')  {
        continue;
    }
}

int main(int argc, const char * argv[]) {
    // insert code here...
    FILE *outputFile;
    char file_name[100];
    int ch;
    
    printf("This program saves your console input to an output file. \n");
    printf("Enter the name of the output file: ");
    scanf("%s",file_name);
    
    

    
    flush_input_buffer();
    
    outputFile = fopen(file_name,"w");
    
    if(outputFile == NULL){
        printf("Cannot open %s for writing. \n",file_name);
        return EXIT_FAILURE;
    }
    
    printf("Enter your input (Press Ctrl-D when done) ....\n");
    
    while ((ch = getc(stdin)) != EOF )
    {
        putc(ch, outputFile);
    }
    
    fclose(outputFile);
    
    printf("=========Ctrl-D pressed===== \n");
    printf("Input saved to file %s\n",outputFile);
    
    return EXIT_SUCCESS;
}

