/******************************************************************************
 * NAME
 *   processFiles.h
 *
 * DESCRIPTION
 *   Header file for processing input files, building the graph, and calling
 *   Dijkstra's algorithm for each client.
 *
 * COMMENTS
 *
 ******************************************************************************/

#ifndef PROCESSFILES_H
#define PROCESSFILES_H

#include <stdio.h>
#include <stdlib.h>
#include "dijkstra.h"

// Processes input files and executes Dijkstra for each client
int processFiles(FILE *mapsInput, FILE *clientsInput, FILE *output);

#endif
