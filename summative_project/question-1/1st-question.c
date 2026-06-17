#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Incident {
    char details[100];
    struct Incident* prev;
    struct Incident* next;
} Incident;

Incident* head = NULL;
Incident* tail = NULL;
Incident* current = NULL;
int count = 0;
int liveMonitoring = 0;
const int MAX_CAPACITY = 25;

void addIncident(const char* info) {
    if (count >= MAX_CAPACITY) {
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

void deleteAll() {
    while (head) {
        Incident* temp = head;
        head = head->next;
        free(temp);
    }
    head = tail = current = NULL;
    count = 0;
}

int main() {
    char cmd;
    char buffer[100];
    int running = 1;

    while (running) {
        printf("\n--- Emergency Tracker (%d/%d) | Live: %s ---\n", count, MAX_CAPACITY, liveMonitoring ? "ON" : "OFF");
        if (current) printf("Viewing: %s\n", current->details);
        else printf("No incidents recorded.\n");
        
        printf("Commands: [n]ew, [f]wd, [b]ack, [l]ive, [s]top, [d]elete, [q]uit: ");
        scanf(" %c", &cmd);

        if (liveMonitoring) {
            // In a real app, this would be handled by a thread or non-blocking input
            addIncident("Automated Incident Received");
        }

        switch (cmd) {
            case 'n': printf("Enter details: "); scanf("%s", buffer); addIncident(buffer); break;
            case 'f': if (current && current->next) current = current->next; break;
            case 'b': if (current && current->prev) current = current->prev; break;
            case 'l': liveMonitoring = 1; break;
            case 's': liveMonitoring = 0; break;
            case 'd': deleteAll(); break;
            case 'q': running = 0; break;
            default: printf("Invalid Command!\n");
        }
    }
    deleteAll(); // Cleanup on exit
    return 0;
}
