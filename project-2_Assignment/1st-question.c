#include <stdio.h>

typedef struct {
    char id;
    int priority;
} Job;

void swap(Job *a, Job *b) {
    Job temp = *a;
    *a = *b;
    *b = temp;
}

// Maintains Max-Heap property upwards
void heapifyUp(Job heap[], int i) {
    while (i > 0 && heap[(i - 1) / 2].priority < heap[i].priority) {
        swap(&heap[i], &heap[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

// Maintains Max-Heap property downwards
void heapifyDown(Job heap[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && heap[left].priority > heap[largest].priority) largest = left;
    if (right < n && heap[right].priority > heap[largest].priority) largest = right;

    if (largest != i) {
        swap(&heap[i], &heap[largest]);
        heapifyDown(heap, n, largest);
    }
}

void printHeap(Job heap[], int n) {
    for (int i = 0; i < n; i++) {
        printf("(%c:%d) ", heap[i].id, heap[i].priority);
    }
    printf("\n");
}

int main() {
    // Initial data
    Job heap[12] = {{'A', 42}, {'B', 17}, {'C', 93}, {'D', 28}, {'E', 65}, 
                    {'F', 81}, {'G', 54}, {'H', 60}, {'I', 99}, {'J', 73}, {'K', 88}};
    int n = 11;

    // Task 1: Build initial Max-Heap
    for (int i = (n / 2) - 1; i >= 0; i--) {
        heapifyDown(heap, n, i);
    }
    printf("Initial Max-Heap: ");
    printHeap(heap, n);

    // Task 3: Insert Urgent Job (L:100)
    heap[n] = (Job){'L', 100};
    n++;
    heapifyUp(heap, n - 1);
    printf("After Inserting (L:100): ");
    printHeap(heap, n);

    // Task 4: Abort Urgent Job
    heap[0] = heap[n - 1]; // Move last to root
    n--;
    heapifyDown(heap, n, 0); // Restore Max-Heap
    printf("After Aborting (L:100): ");
    printHeap(heap, n);

    return 0;
}
