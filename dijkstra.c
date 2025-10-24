#include "dijkstra.h"
#include "heap.h"

#include <limits.h>
#include <string.h>

#define INF INT_MAX

/***********************************************************************************************************************
 * dijkstra()
 *
 * Arguments: adjList - adjacency list representing the graph
 *            restrictions - structure containing travel restrictions
 *            numCities - total number of cities (graph vertices)
 *            startCity - source city
 *            endCity - destination city
 *            departureTime - departure time from the source city
 *            filter - string defining optimization mode ("cost" or "duration")
 *            clientID - client identifier
 *            output - file pointer for output
 * Returns: 0
 * Side-Effects: writes result to the output file:
 *                  - <clientID> -1 if path is invalid or violates restrictions;
 *                  - <clientID> ... if path is valid, including duration and cost
 *               allocates and frees dynamic memory
 *
 * Description: implements Dijkstra's algorithm to minimize cost or duration considering travel restrictions.
 ***********************************************************************************************************************/

int dijkstra(struct Adjacency** adjList, struct Restrictions restrictions, int numCities, int startCity, int endCity, int departureTime, char* filter, int clientID, FILE *output){
    int newWeight = 0;
    int* weight;
    int* secondaryWeight;
    int* heapIndex;
    int* prevCity;
    char** prevTransport;

    weight = (int*)malloc(numCities * sizeof(int));
    if(weight == NULL) exit(0);
    secondaryWeight = (int*)malloc(numCities * sizeof(int));
    if(secondaryWeight == NULL) exit(0);
    heapIndex = (int*)malloc(numCities * sizeof(int));
    if(heapIndex == NULL) exit(0);
    prevCity = (int*)malloc(numCities * sizeof(int));
    if(prevCity == NULL) exit(0);
    prevTransport = (char**)malloc(numCities * sizeof(char*));
    if(prevTransport == NULL) exit(0); 
    for(int i = 0; i < numCities; i++){
        prevTransport[i] = malloc(10 * sizeof(char));
        if(prevTransport[i] == NULL) exit(0);
    }

    for(int i = 0; i < numCities; i++){
        weight[i] = INF;
        heapIndex[i] = -1;
        prevCity[i] = i;
    }

    if(strcmp(filter, "cost") == 0){
        weight[startCity - 1] = 0;
        secondaryWeight[startCity - 1] = departureTime;
    } else {
        weight[startCity - 1] = departureTime;
        secondaryWeight[startCity - 1] = 0;
    }

    struct minHeap* heap = createMinHeap(numCities);
    insertMinHeap(heap, startCity - 1, weight[startCity - 1], heapIndex);

    while(!isEmpty(heap)) {
        struct heapNode minNode = extractMin(heap, heapIndex);
        int u = minNode.city;

        heapIndex[u] = -2;
        if (u == endCity - 1) break;

        struct Adjacency* adjacency = adjList[u];
        while (adjacency != NULL) {
            int v = adjacency->path.destinationCity - 1;

            if(!check_restrictions(restrictions, adjacency) || heapIndex[v] == -2){
                adjacency = adjacency->next;
                continue;
            }

            if(strcmp(filter, "cost") == 0){
                newWeight = weight[u] + adjacency->path.travelCost;
            } else {
                newWeight = weight[u] + waiting_time(weight[u], adjacency->path) + adjacency->path.travelDuration;
            }

            if(weight[v] > newWeight){
                weight[v] = newWeight;
                prevCity[v] = u;
                strcpy(prevTransport[v], adjacency->path.transport);

                if(strcmp(filter, "cost") == 0){
                    secondaryWeight[v] = secondaryWeight[u] + waiting_time(secondaryWeight[u], adjacency->path) + adjacency->path.travelDuration;
                } else {
                    secondaryWeight[v] = secondaryWeight[u] + adjacency->path.travelCost;
                }

                if(heapIndex[v] == -1){
                    insertMinHeap(heap, v, weight[v], heapIndex);
                } else {
                    decreaseKey(heap, heapIndex[v], newWeight, heapIndex);
                }
            }
            adjacency = adjacency->next;
        }
    }

    bool invalidDueToRestrictionB = false;
    if(strcmp(filter, "cost") == 0){
        if(restrictions.B2){
            if(weight[endCity - 1] > restrictions.totalCost){
                invalidDueToRestrictionB = true;
            }
        }
    } else {
        if(restrictions.B1){
            if(weight[endCity - 1] > restrictions.totalDuration){
                invalidDueToRestrictionB = true;
            }
        }    
    }

    if (weight[endCity - 1] == INF || invalidDueToRestrictionB) {
        fprintf(output, "%d -1\n", clientID);
        freeMinHeap(heap);
        free(weight);
        free(secondaryWeight);
        free(heapIndex);
        free(prevCity);
        for(int i = 0; i < numCities; i++){
            free(prevTransport[i]);
        }
        free(prevTransport);
        return 0;
    }

    int* trip = (int*)malloc(numCities * sizeof(int));
    if(trip == NULL) exit(0);
    int count = 0, storeCount = 0;

    for (int v = endCity - 1; v != startCity - 1; v = prevCity[v]) {
        count++;
    }
    storeCount = count;
    for (int v = endCity - 1; v != startCity - 1; v = prevCity[v]) {
        trip[--count] = v;
    }

    fprintf(output, "%d %d ", clientID, startCity);

    for (int i = 0; i < storeCount; i++) {
        fprintf(output, "%s %d ", prevTransport[trip[i]], trip[i] + 1);
    }

    if(strcmp(filter, "cost") == 0){
        fprintf(output, "%d %d\n", secondaryWeight[endCity - 1] - departureTime, weight[endCity - 1]);
    } else {
        fprintf(output, "%d %d\n", weight[endCity - 1] - departureTime, secondaryWeight[endCity - 1]);
    }

    freeMinHeap(heap);
    free(weight);
    free(secondaryWeight);
    free(heapIndex);
    free(prevCity);
    free(trip);
    for(int i = 0; i < numCities; i++){
        free(prevTransport[i]);
    }
    free(prevTransport);
    return 0;
}

/***********************************************************************************************************************
 * check_restrictions()
 *
 * Arguments: restrictions - structure containing travel restrictions
 *            adjacency - pointer to adjacency structure to check
 * Returns: true - if the edge satisfies all restrictions
 *          false - if the edge violates any restriction
 * Side-Effects: none
 * Description: checks if a graph edge respects the defined travel restrictions.
 ***********************************************************************************************************************/

bool check_restrictions(struct Restrictions restrictions, struct Adjacency* adjacency){
    if(restrictions.A1){
        if(strcmp(adjacency->path.transport, restrictions.restrictedTransport) == 0){
            return false;
        }
    }
    if(restrictions.A2){
        if(adjacency->path.travelDuration > restrictions.maxDuration){
            return false;
        }
    }
    if(restrictions.A3){
        if(adjacency->path.travelCost > restrictions.maxCost){
            return false;
        }
    }
    return true;
}

/***********************************************************************************************************************
 * waiting_time()
 *
 * Arguments: currentTime - current time in minutes
 *            path - structure with departure info (first, last, periodicity)
 * Returns: waiting time in minutes until the next departure
 * Side-Effects: none
 * Description: calculates the minutes until the next transport departure considering the schedule in 'path'.
 ***********************************************************************************************************************/

int waiting_time(int currentTime, struct Path path) {
    int first = path.firstDeparture;      
    int last  = path.lastDeparture;        
    int period  = path.departurePeriodicity;  
    
    int h = currentTime % 1440;

    if (h <= first) return (first - h);
    if (h > last)   return (1440 - h) + first;

    int nextDeparture = first + ((h - first + period - 1) / period) * period;

    if (nextDeparture > last) {
        return (1440 - h) + first;
    }
    return nextDeparture - h;
}
