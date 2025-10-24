/******************************************************************************
 * NAME
 *   dijkstra.h
 *
 * DESCRIPTION
 *   Header file for Dijkstra's algorithm implementation with travel restrictions
 *
 * COMMENTS
 *
 ******************************************************************************/

#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Structure representing a path/edge between cities
struct Path {
    int originCity;
    int destinationCity;
    char transport[10];
    int travelDuration;
    int travelCost;
    int firstDeparture;
    int lastDeparture;
    int departurePeriodicity;
};

// Node of adjacency list
struct Adjacency {
    struct Path path;
    struct Adjacency* next;
};

// Travel restrictions
struct Restrictions {
    bool A1, A2, A3, B1, B2;
    char restrictedTransport[10];
    int totalDuration;
    int totalCost;
    int maxCost;
    int maxDuration;
};

// Dijkstra algorithm
int dijkstra(struct Adjacency** adjList, struct Restrictions restrictions, int numCities, int startCity, int endCity, int departureTime, char* filter, int clientID, FILE *output);

// Check if an adjacency satisfies restrictions
bool check_restrictions(struct Restrictions restrictions, struct Adjacency* adjacency);

// Calculate waiting time until next departure
int waiting_time(int currentTime, struct Path path);

#endif
