/******************************************************************************
 * NAME
 *   file.h
 *
 * DESCRIPTION
 *   Header file for handling input/output files and file processing.
 *
 * COMMENTS
 *
 ******************************************************************************/

#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdlib.h>

// Opens an input file for reading
FILE *openFile(char *filename);

// Creates an output .sol file corresponding to the input file
FILE *createOutputFile(char *filename);

// Processes input files and writes results to the output file
int processFiles(FILE *mapsInput, FILE *clientInput, FILE *output);

#endif
