#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CAPACITY 50

typedef struct {
    char name[50];
    int risk_score;
} Passenger;

// Utility function to swap two passenger structures
void swap(Passenger *x, Passenger *y) {
    Passenger temp = *x;
    *x = *y;
    *y = temp;
}

// Visualizes the flat array representation of the heap
void printHeapArray(Passenger heap[], int size, const char *heading) {
    printf("\n--- %s ---\n", heading);
    printf("Index:     ");
    for (int i = 0; i < size; i++) printf("%-10d ", i);
    printf("\nName:      ");
    for (int i = 0; i < size; i++) printf("%-10s ", heap[i].name);
    printf("\nRisk Score:");
    for (int i = 0; i < size; i++) printf("%-10d ", heap[i].risk_score);
    printf("\n");
}

// Max-Heapify logic to bubble down elements violating properties
void maxHeapify(Passenger heap[], int size, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < size && heap[left].risk_score > heap[largest].risk_score)
        largest = left;

    if (right < size && heap[right].risk_score > heap[largest].risk_score)
        largest = right;

    if (largest != i) {
        swap(&heap[i], &heap[largest]);
        maxHeapify(heap, size, largest);
    }
}

// Builds a Max-Heap from an unsorted array bottom-up
void buildMaxHeap(Passenger heap[], int size) {
    for (int i = (size / 2) - 1; i >= 0; i--) {
        maxHeapify(heap, size, i);
    }
}

// Inserts a passenger and restores Max-Heap rules using Bubble-Up
void insertPassenger(Passenger heap[], int *size, const char *name, int risk) {
    if (*size >= MAX_CAPACITY) {
        printf("Error: Heap Overflow\n");
        return;
    }
    
    // Put item at the end of the complete binary tree array
    int i = *size;
    strcpy(heap[i].name, name);
    heap[i].risk_score = risk;
    (*size)++;

    // Heapify-Up
    while (i != 0 && heap[(i - 1) / 2].risk_score < heap[i].risk_score) {
        swap(&heap[i], &heap[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

// Removes and returns the highest priority passenger
Passenger extractMax(Passenger heap[], int *size) {
    if (*size <= 0) {
        Passenger empty = {"None", -1};
        return empty;
    }
    
    Passenger maxPassenger = heap[0];
    heap[0] = heap[*size - 1];
    (*size)--;
    
    maxHeapify(heap, *size, 0);
    return maxPassenger;
}

// Finds the index of a passenger by risk score (Helper for target deletion)
int findIndexByScore(Passenger heap[], int size, int score) {
    for (int i = 0; i < size; i++) {
        if (heap[i].risk_score == score) {
            return i;
        }
    }
    return -1;
}

// Deletes any arbitrary node by its index position
void deleteNode(Passenger heap[], int *size, int index) {
    if (index < 0 || index >= *size) return;

    // Substitute target node with last leaf node
    heap[index] = heap[*size - 1];
    (*size)--;

    // Re-adjust both directions to maintain proper balance properties
    maxHeapify(heap, *size, index);
    
    int i = index;
    while (i != 0 && heap[(i - 1) / 2].risk_score < heap[i].risk_score) {
        swap(&heap[i], &heap[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

int main() {
    Passenger heap[MAX_CAPACITY];
    int size = 0;

    // 1. Initial dynamic passenger arrivals
    insertPassenger(heap, &size, "Alice", 45);
    insertPassenger(heap, &size, "Brian", 12);
    insertPassenger(heap, &size, "Chen", 78);
    insertPassenger(heap, &size, "Fatima", 56);
    insertPassenger(heap, &size, "Noah", 89);
    insertPassenger(heap, &size, "Sofia", 34);

    printHeapArray(heap, size, "Initial Max-Heap Structure");

    // 2. Demonstration of Priority Queue Extraction Sequence
    // We clone our heap to process a readout without destroying original sequence tracking
    Passenger tempHeap[MAX_CAPACITY];
    int tempSize = size;
    memcpy(tempHeap, heap, sizeof(Passenger) * size);

    printf("\n--- Processing Screening Priority Queue (Highest Risk First) ---\n");
    int order = 1;
    while (tempSize > 0) {
        Passenger p = extractMax(tempHeap, &tempSize);
        printf("Screening Order #%d: %s [Risk Score: %d]\n", order++, p.name, p.risk_score);
    }

    // 3. New High-Risk Passenger Arrives (Risk: 98)
    printf("\n[Arrival Alert]: High risk passenger 'Zayn' arrives with score 98.");
    insertPassenger(heap, &size, "Zayn", 98);
    printHeapArray(heap, size, "Max-Heap After Inserting Zayn (98)");

    // 4. Passenger with score 98 is cleared and removed from structure
    int targetIndex = findIndexByScore(heap, size, 98);
    if (targetIndex != -1) {
        printf("\n[System Update]: Passenger with risk score 98 cleared. Removing from heap.");
        deleteNode(heap, &size, targetIndex);
    }
    
    printHeapArray(heap, size, "Final Max-Heap Structure After Deleting Risk 98");

    return 0;
}
