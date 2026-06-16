#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Incident {
    char details[100];
    struct Incident* prev;
    struct Incident* next;
} Incident;

// Global pointers for the tracker
Incident* head = NULL;
Incident* tail = NULL;
Incident* current = NULL;
int count = 0;
int liveMonitoring = 0;

void addIncident(const char* info) {
    if (count >= 25) {
        Incident* temp = head;
        head = head->next;
        if (head) head->prev = NULL;
        else tail = NULL;
        free(temp);
        count--;
    }

    Incident* newNode = (Incident*)malloc(sizeof(Incident));
    strncpy(newNode->details, info, 99);
    newNode->next = NULL;
    newNode->prev = tail;

    if (!head) {
        head = tail = current = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    count++;
}

int main() {
    // Example usage
    addIncident("Emergency: Fire at Sector 7");
    printf("Incident added. Current: %s\n", current->details);
    
    return 0;
}
