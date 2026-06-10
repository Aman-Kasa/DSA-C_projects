#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char id;
    int priority;
} Job;

// Helper function to swap two jobs
void swap(Job *x, Job *y) {
    Job temp = *x;
    *x = *y;
    *y = temp;
}

// Utility function to print the current array layout of the heap
void printHeap(Job heap[], int size, const char *msg) {
    printf("\n--- %s ---\n", msg);
    printf("Index:    ");
    for (int i = 0; i < size; i++) printf("%-4d ", i);
    printf("\nJob ID:   ");
    for (int i = 0; i < size; i++) printf("%-4c ", heap[i].id);
    printf("\nPriority: ");
    for (int i = 0; i < size; i++) printf("%-4d ", heap[i].priority);
    printf("\n");
}

// Bottom-up Max-Heapify operation for maintenance and deletion
void maxHeapify(Job heap[], int size, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < size && heap[left].priority > heap[largest].priority)
        largest = left;

    if (right < size && heap[right].priority > heap[largest].priority)
        largest = right;

    if (largest != i) {
        swap(&heap[i], &heap[largest]);
        maxHeapify(heap, size, largest); // Recursively process affected sub-tree
    }
}

// Converts an arbitrary complete binary tree array into a Max-Heap
void buildMaxHeap(Job heap[], int size) {
    for (int i = (size / 2) - 1; i >= 0; i--) {
        maxHeapify(heap, size, i);
    }
}

// Inserts a new job into the Max-Heap and restores order using Heapify-Up
void insertJob(Job heap[], int *size, Job newJob) {
    int i = *size;
    heap[i] = newJob;
    (*size)++;

    // Heapify-Up process
    while (i != 0 && heap[(i - 1) / 2].priority < heap[i].priority) {
        swap(&heap[i], &heap[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

// Removes the root node (highest priority) and restores structural order
Job extractMax(Job heap[], int *size) {
    if (*size <= 0) {
        Job empty = {'\0', -1};
        return empty;
    }
    
    Job maxJob = heap[0];
    heap[0] = heap[*size - 1];
    (*size)--;
    
    maxHeapify(heap, *size, 0);
    return maxJob;
}

int main() {
    // Initial dataset configuration
    char ids[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K'};
    int priorities[] = {42, 17, 93, 28, 65, 81, 54, 60, 99, 73, 88};
    int size = 11;
    
    Job heap[20]; // Pre-allocated space to allow insertions safely
    for (int i = 0; i < size; i++) {
        heap[i].id = ids[i];
        heap[i].priority = priorities[i];
    }

    printHeap(heap, size, "Initial Input Array Configuration");

    // Task 1: Build the Max-Heap
    buildMaxHeap(heap, size);
    printHeap(heap, size, "Converted Array Structure (Max-Heap Complete)");

    // Task 2: Insert Urgent Job L with priority 100
    Job urgentJob = {'L', 100};
    insertJob(heap, &size, urgentJob);
    printHeap(heap, size, "Array Structure After Inserting Urgent Job L(100)");

    // Task 3: Abort/Remove the Urgent Job (which sits at index 0 as max priority)
    Job removed = extractMax(heap, &size);
    printf("\n[Action]: Successfully aborted and removed Job %c with Priority %d\n", removed.id, removed.priority);
    printHeap(heap, size, "Final Array Structure After Removing Job L(100)");

    return 0;
}
