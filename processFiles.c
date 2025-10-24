#include "processFiles.h"  
#include <string.h> 
#include <stdlib.h> 
#include <stdio.h>

/***********************************************************************************************************************
 * processFiles()
 *
 * Arguments: mapsInput - input file containing the map
 *            clientsInput - input file containing clients and their requests
 *            output - output file where results will be written
 * Returns: 0
 * Side-Effects: reads data from input files and writes results to the output file
 *               allocates and frees dynamic memory
 *               calls dijkstra() to process each client
 *
 * Description: reads input files, builds the adjacency list representing the map graph,
 *              reads each client's information and restrictions, and executes Dijkstra's algorithm.
 ***********************************************************************************************************************/
int processFiles(FILE *mapsInput, FILE *clientsInput, FILE *output) {
    int clientID, numClients, numRestrictions, cities, connections, startCity, endCity, departureTime;
    char filter[6], restriction[3];

    if (fscanf(mapsInput, "%d", &cities) != 1) return 0;
    if (fscanf(mapsInput, "%d", &connections) != 1) return 0; 
    if (fscanf(clientsInput, "%d", &numClients) != 1) return 0;

    struct Adjacency** adj = malloc(cities * sizeof(struct Adjacency*));
    if (!adj) {
        fclose(clientsInput);
        fclose(mapsInput);
        fclose(output);
        return 0;
    }
    for (int i = 0; i < cities; i++) {
        adj[i] = NULL;
    }

    for (int i = 0; i < connections; i++) {
        struct Path paths;
        if (fscanf(mapsInput, "%d %d %9s %d %d %d %d %d", &paths.originCity, &paths.destinationCity, paths.transport, &paths.travelDuration, &paths.travelCost, &paths.firstDeparture, &paths.lastDeparture, &paths.departurePeriodicity) != 8) return 0;

        struct Adjacency* newAdj = malloc(sizeof(struct Adjacency));
        if (!newAdj) return 0;
        newAdj->path = paths;
        newAdj->next = adj[paths.originCity - 1];
        adj[paths.originCity - 1] = newAdj;

        struct Adjacency* reverse = malloc(sizeof(struct Adjacency));
        if (!reverse) return 0;
        reverse->path = paths;
        reverse->path.originCity = paths.destinationCity;
        reverse->path.destinationCity = paths.originCity;
        reverse->next = adj[paths.destinationCity - 1];
        adj[paths.destinationCity - 1] = reverse;
    }

    while (fscanf(clientsInput, "%d", &clientID) == 1) {
        if (fscanf(clientsInput, "%d", &startCity) != 1) return 0;
        if (fscanf(clientsInput, "%d", &endCity) != 1) return 0;
        if (fscanf(clientsInput, "%d", &departureTime) != 1) return 0;
        if (fscanf(clientsInput, "%s", filter) != 1) return 0;
        if (fscanf(clientsInput, "%d", &numRestrictions) != 1) return 0;

        struct Restrictions* clientRestrictions = malloc(sizeof(struct Restrictions));
        if (!clientRestrictions) {
            fclose(clientsInput);
            fclose(mapsInput);
            fclose(output);
            free(adj);
            exit(0);
        }

        strcpy(clientRestrictions->restrictedTransport, "");
        clientRestrictions->totalDuration = 0;
        clientRestrictions->totalCost = 0;
        clientRestrictions->maxCost = 0;
        clientRestrictions->maxDuration = 0;

        if (numRestrictions < 0 || numRestrictions > 2) return 0;

        clientRestrictions->A1 = false;
        clientRestrictions->A2 = false;
        clientRestrictions->A3 = false;
        clientRestrictions->B1 = false;
        clientRestrictions->B2 = false;

        switch (numRestrictions) {
            case 0:
                break;
            case 1:
                if (fscanf(clientsInput, "%s", restriction) != 1) return 0;
                if (strcmp(restriction, "A1") == 0) {
                    clientRestrictions->A1 = true;
                    if(fscanf(clientsInput, "%9s", clientRestrictions->restrictedTransport) != 1) return 0;
                } else if (strcmp(restriction, "A2") == 0) {
                    clientRestrictions->A2 = true;
                    if(fscanf(clientsInput, "%d", &clientRestrictions->maxDuration) != 1) return 0;
                } else if (strcmp(restriction, "A3") == 0) {
                    clientRestrictions->A3 = true;
                    if(fscanf(clientsInput, "%d", &clientRestrictions->maxCost) != 1) return 0;
                } else if (strcmp(restriction, "B1") == 0) {
                    clientRestrictions->B1 = true;
                    if(fscanf(clientsInput, "%d", &clientRestrictions->totalDuration) != 1) return 0;
                } else if (strcmp(restriction, "B2") == 0) {
                    clientRestrictions->B2 = true; 
                    if(fscanf(clientsInput, "%d", &clientRestrictions->totalCost) != 1) return 0;
                }
                break;
            case 2:
                for (int i = 0; i < 2; i++) {
                    if (fscanf(clientsInput, "%s", restriction) != 1) return 0;
                    if (strcmp(restriction, "A1") == 0) {
                        clientRestrictions->A1 = true;
                        if(fscanf(clientsInput, "%9s", clientRestrictions->restrictedTransport) != 1) return 0;
                    } else if (strcmp(restriction, "A2") == 0) {
                        clientRestrictions->A2 = true;
                        if(fscanf(clientsInput, "%d", &clientRestrictions->maxDuration) != 1) return 0;
                    } else if (strcmp(restriction, "A3") == 0) {
                        clientRestrictions->A3 = true;
                        if(fscanf(clientsInput, "%d", &clientRestrictions->maxCost) != 1) return 0;
                    } else if (strcmp(restriction, "B1") == 0) {
                        clientRestrictions->B1 = true;
                        if(fscanf(clientsInput, "%d", &clientRestrictions->totalDuration) != 1) return 0;
                    } else if (strcmp(restriction, "B2") == 0) {
                        clientRestrictions->B2 = true; 
                        if(fscanf(clientsInput, "%d", &clientRestrictions->totalCost) != 1) return 0;
                    }
                }
                break;
            default:
                break;    
        }

        // In case of restriction violation, print "<clientID> -1"
        if(endCity <= 0 || startCity <= 0 || endCity > cities || startCity > cities){
            free(clientRestrictions);
            fprintf(output, "%d -1\n", clientID);
            continue;
        }

        dijkstra(adj, *clientRestrictions, cities, startCity, endCity, departureTime, filter, clientID, output);

        free(clientRestrictions);
    }

    for (int i = 0; i < cities; i++) {
        struct Adjacency* ptr = adj[i];
        while (ptr) {
            struct Adjacency* tmp = ptr;
            ptr = ptr->next;
            free(tmp);
        }
    }
    free(adj);

    return 0;
}
