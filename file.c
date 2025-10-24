/*
* AED Project 2025/2026
* Authors: Filipe Serafim (ist1110177) and Lena Wang (ist1110762)
* File: file.c
* Description: Responsible for handling file operations and processing input/output files.
*/

#include "file.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Description: Opens an input file for reading.
* Arguments: filename - name of the file to open
* Output: Returns a pointer to the opened file.
*/
FILE *openFile(char *filename) {
    FILE *file = fopen(filename, "r");
    return file;
}

/* Description: Creates the output .sol file corresponding to the input .cli file.
* Arguments: filename - name of the input .cli file
* Output: Returns a pointer to the created output file.
*/
FILE *createOutputFile(char *filename) {

    int filenameLength = strlen(filename);

    // Allocate space for new filename (+8 for ".sol" and +1 for null terminator)
    char* outputName = (char*)malloc(filenameLength + 8 + 1);  
    if (outputName == NULL) {
        exit(0);
    }

    strcpy(outputName, filename);
    outputName[filenameLength] = '\0';

    // Remove the extension
    char* lastDot = strrchr(outputName, '.');
    if (lastDot != NULL) {
        *lastDot = '\0';
    }

    // Append the new ".sol" extension
    strcat(outputName, ".sol");

    FILE *file = fopen(outputName, "w");
    if (file == NULL) {
        free(outputName);
        exit(0);
    }

    free(outputName);
    return file;   
}
