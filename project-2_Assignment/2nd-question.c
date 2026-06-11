#include <stdio.h>
#include <string.h>

typedef struct {
    char name[20];
    int risk;
} Passenger;

void swap(Passenger *a, Passenger *b) {
    Passenger temp = *a;
    *a = *b;
    *b = temp;
}

// Maintains Max-Heap property upwards (Used for Insertion)
void heapifyUp(Passenger heap[], int i) {
    while (i > 0 && heap[(i - 1) / 2].risk < heap[i].risk) {
        swap(&heap[i], &heap[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

// Maintains Max-Heap property downwards (Used for Build and Deletion)
void heapifyDown(Passenger heap[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && heap[left].risk > heap[largest].risk) largest = left;
    if (right < n && heap[right].risk > heap[largest].risk) largest = right;

    if (largest != i) {
        swap(&heap[i], &heap[largest]);
        heapifyDown(heap, n, largest);
    }
}

void printHeap(Passenger heap[], int n) {
    for (int i = 0; i < n; i++) {
        printf("Passenger: %s [Risk: %d]\n", heap[i].name, heap[i].risk);
    }
}

int main() {
    // Task: Initial Passenger List
    Passenger heap[10] = {{"Noah", 89}, {"Chen", 78}, {"Fatima", 56}, 
                          {"Alice", 45}, {"Sofia", 34}, {"Brian", 12}};
    int n = 6;

    // 1. Build Max-Heap
    for (int i = (n / 2) - 1; i >= 0; i--) heapifyDown(heap, n, i);

    printf("--- Screening Priority Queue Sequence ---\n");
    // Show priority queue extraction
    Passenger tempHeap[10];
    memcpy(tempHeap, heap, sizeof(Passenger) * n);
    int tempN = n;
    while (tempN > 0) {
        printf("Passenger: %s [Risk: %d]\n", tempHeap[0].name, tempHeap[0].risk);
        tempHeap[0] = tempHeap[tempN - 1];
        tempN--;
        heapifyDown(tempHeap, tempN, 0);
    }

    // 2. Insert new passenger (Risk 98) - Task 3
    strcpy(heap[n].name, "UrgentGuest");
    heap[n].risk = 98;
    n++;
    heapifyUp(heap, n - 1);
    printf("\nAfter inserting UrgentGuest [Risk: 98]\n");
    printHeap(heap, n);

    // 3. Clear passenger (Remove 98) - Task 4
    // 98 is at index 0 after heapifyUp
    heap[0] = heap[n - 1];
    n--;
    heapifyDown(heap, n, 0);
    printf("\nAfter clearing passenger [Risk: 98]\n");
    printHeap(heap, n);

    return 0;
}
