/******************************************************************************
 * NAME
 *   heap.h
 *
 * DESCRIPTION
 *   Header file for the min-heap implementation used in Dijkstra's algorithm.
 *
 * COMMENTS
 *
 ******************************************************************************/

#ifndef HEAP_H
#define HEAP_H

// Heap node structure
struct heapNode {
    int city;
    int weight;
};

// Min-heap structure
struct minHeap {
    int size;
    int capacity;
    struct heapNode* arr;
};

// Heap utility functions
int parent(int i);
int left(int i);
int right(int i);

// Min-heap operations
struct minHeap* createMinHeap(int capacity);
void swap(struct heapNode* x, struct heapNode* y);
void heapifyUp(struct minHeap* heap, int i, int* heapIndex);
void heapifyDown(struct minHeap* heap, int i, int* heapIndex);
void insertMinHeap(struct minHeap* heap, int city, int weight, int* heapIndex);
struct heapNode extractMin(struct minHeap* heap, int* heapIndex);
void decreaseKey(struct minHeap* heap, int index, int newWeight, int* heapIndex);
int isEmpty(struct minHeap* heap);
void freeMinHeap(struct minHeap* heap);

#endif
