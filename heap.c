/*
* AED Project 2025/2026
* Authors: Filipe Serafim (ist1110177) and Lena Wang (ist1110762)
* File: heap.c
* Description: Implements functions for a min-heap, used by Dijkstra's algorithm.
*/

#include "heap.h"
#include <stdlib.h>
#include <stdio.h>

/***********************************************************************************************************************
 * parent()
 *
 * Arguments: i - index of a node in the heap
 * Returns: index of the parent node of i
 * Side-Effects: none
 *
 * Description: calculates the index of a parent node in a binary heap.
 ***********************************************************************************************************************/
int parent(int i) {
    return (i - 1) / 2;
}

/***********************************************************************************************************************
 * left()
 *
 * Arguments: i - index of a node in the heap
 * Returns: index of the left child node
 * Side-Effects: none
 *
 * Description: calculates the index of a left child node in a binary heap.
 ***********************************************************************************************************************/
int left(int i) {
    return 2 * i + 1;
}

/***********************************************************************************************************************
 * right()
 *
 * Arguments: i - index of a node in the heap
 * Returns: index of the right child node
 * Side-Effects: none
 *
 * Description: calculates the index of a right child node in a binary heap.
 ***********************************************************************************************************************/
int right(int i) {
    return 2 * i + 2;
}

/***********************************************************************************************************************
 * createMinHeap()
 *
 * Arguments: capacity - maximum capacity of the heap
 * Returns: pointer to the created minHeap structure
 * Side-Effects: allocates dynamic memory
 *
 * Description: creates and initializes a min-heap used in Dijkstra's algorithm to manage the vertex with minimum weight.
 ***********************************************************************************************************************/
struct minHeap* createMinHeap(int capacity) {
    struct minHeap* heap = (struct minHeap*)malloc(sizeof(struct minHeap));
    heap->arr = malloc(capacity * sizeof(struct heapNode));
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

/***********************************************************************************************************************
 * swap()
 *
 * Arguments: x - pointer to first heap node
 *            y - pointer to second heap node
 * Returns: void
 * Side-Effects: swaps the contents of two heapNode structures
 *
 * Description: swaps data between two nodes in the heap.
 ***********************************************************************************************************************/
void swap(struct heapNode* x, struct heapNode* y) {
    struct heapNode temp = *x;
    *x = *y;
    *y = temp;
}

/***********************************************************************************************************************
 * heapifyUp()
 *
 * Arguments: heap - pointer to the minHeap
 *            i - index of the node to move up
 *            heapIndex - auxiliary array with each city's index in the heap
 * Returns: void
 * Side-Effects: updates heap positions and index array
 * 
 * Description: adjusts the heap to maintain the min-heap property after insertion or update.
 ***********************************************************************************************************************/
void heapifyUp(struct minHeap* heap, int i, int* heapIndex) {
    while (i != 0 && heap->arr[parent(i)].weight > heap->arr[i].weight) {
        swap(&heap->arr[i], &heap->arr[parent(i)]);
        heapIndex[heap->arr[i].city] = i;
        heapIndex[heap->arr[parent(i)].city] = parent(i);
        i = parent(i);
    }
}

/***********************************************************************************************************************
 * heapifyDown()
 *
 * Arguments: heap - pointer to the minHeap
 *            i - index of the node to move down
 *            heapIndex - auxiliary array with each city's index in the heap
 * Returns: void
 * Side-Effects: updates heap positions and index array
 * 
 * Description: adjusts the heap to maintain the min-heap property after removing the minimum node or swapping the root.
 ***********************************************************************************************************************/
void heapifyDown(struct minHeap* heap, int i, int* heapIndex) {
    int smallest = i;
    int l = left(i);
    int r = right(i);

    if (l < heap->size && heap->arr[l].weight < heap->arr[smallest].weight)
        smallest = l;
    if (r < heap->size && heap->arr[r].weight < heap->arr[smallest].weight)
        smallest = r;

    if (smallest != i) {
        swap(&heap->arr[i], &heap->arr[smallest]);
        heapIndex[heap->arr[i].city] = i;
        heapIndex[heap->arr[smallest].city] = smallest;
        heapifyDown(heap, smallest, heapIndex);
    }
}

/***********************************************************************************************************************
 * insertMinHeap()
 *
 * Arguments: heap - pointer to the minHeap
 *            city - node identifier
 *            weight - value associated with the node
 *            heapIndex - auxiliary array with each city's index in the heap
 * Returns: void
 * Side-Effects: updates heap and index array, increases heap size
 * 
 * Description: inserts a new node and readjusts the heap to maintain the min-heap property.
 ***********************************************************************************************************************/
void insertMinHeap(struct minHeap* heap, int city, int weight, int* heapIndex) {
    if (heap == NULL || heap->size == heap->capacity) {
        return;
    }
    int i = heap->size++;
    heapIndex[city] = i;
    heap->arr[i].city = city;
    heap->arr[i].weight = weight;
    heapifyUp(heap, i, heapIndex);
}

/***********************************************************************************************************************
 * extractMin()
 *
 * Arguments: heap - pointer to the minHeap
 *            heapIndex - auxiliary array with each city's index in the heap
 * Returns: heapNode structure with minimum city and weight
 * Side-Effects: reduces heap size and updates indices
 * 
 * Description: removes and returns the node with minimum weight and readjusts the heap.
 ***********************************************************************************************************************/
struct heapNode extractMin(struct minHeap* heap, int* heapIndex) {
    struct heapNode minNode = { -1, -1 };
    if (heap->size <= 0) {
        return minNode;
    }

    minNode = heap->arr[0];
    struct heapNode lastNode = heap->arr[heap->size - 1];
    heap->arr[0] = lastNode;
    heapIndex[lastNode.city] = 0;
    heapIndex[minNode.city] = -2; // removed node

    heap->size--;

    if (heap->size > 0)
        heapifyDown(heap, 0, heapIndex);

    return minNode;
}

/***********************************************************************************************************************
 * decreaseKey()
 *
 * Arguments: heap - pointer to the minHeap
 *            index - node index in the heap
 *            newWeight - new weight value
 *            heapIndex - auxiliary array with each city's index in the heap
 * Returns: void
 * Side-Effects: updates node weight and reorganizes the heap if necessary
 * 
 * Description: updates the weight of a node and moves it up if the min-heap property is violated.
 ***********************************************************************************************************************/
void decreaseKey(struct minHeap* heap, int index, int newWeight, int* heapIndex) {
    if(heap->arr[index].weight > newWeight) {
        heap->arr[index].weight = newWeight;
        heapifyUp(heap, index, heapIndex);
    }
}

/***********************************************************************************************************************
 * isEmpty()
 *
 * Arguments: heap - pointer to the minHeap
 * Returns: 1 if the heap is empty, 0 otherwise
 * Side-Effects: none
 * 
 * Description: checks if the heap is empty
 ***********************************************************************************************************************/
int isEmpty(struct minHeap* heap) {
    return heap->size == 0;
}

/***********************************************************************************************************************
 * freeMinHeap()
 *
 * Arguments: heap - pointer to the minHeap
 * Returns: void
 * Side-Effects: frees dynamically allocated memory for the heap
 * 
 * Description: frees all memory allocated by the heap.
 ***********************************************************************************************************************/
void freeMinHeap(struct minHeap* heap) {
    free(heap->arr);
    free(heap);
}
