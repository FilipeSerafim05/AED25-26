/*
* AED Project 2025/2026
* Authors: Filipe Serafim (ist1110177) and Lena Wang (ist1110762)
* File: main.c
* Description: Main file of the project. Responsible for managing function calls
*              when the program is invoked from the command line. Ensures the
*              correct number of arguments and closes opened files.
* Arguments: <executable.exe> <mapsFile> <clientsFile>
* Output: Results file with the extension .sol
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "file.h"

int main(int argc, char* argv[]) {

    if(argc != 3) {
        printf("Usage: %s <mapsFile> <clientsFile>\n", argv[0]);
        exit(0);
    } 

    FILE *mapsInput = openFile(argv[1]);
    if(mapsInput == NULL) exit(0);

    FILE *clientsInput = openFile(argv[2]);
    if(clientsInput == NULL){
        fclose(mapsInput);
        exit(0);
    }

    FILE *output = createOutputFile(argv[2]);

    processFiles(mapsInput, clientsInput, output);

    fclose(mapsInput);
    fclose(clientsInput);
    fclose(output);

    exit(0);
}
