#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Cross-platform structural sleep router
#ifdef _WIN32
    #include <windows.h>
    #define TIME_DELAY(seconds) Sleep((seconds) * 1000)
#else
    #include <unistd.h>
    #define TIME_DELAY(seconds) sleep(seconds)
#endif

#define MAX_STR_LEN 150

typedef struct BusStop {
    char name[MAX_STR_LEN];
    int stopNumber;
    struct BusStop *next;
    struct BusStop *prev;
} BusStop;

// Allocates and sets up a new bus stop node
BusStop* createNode(const char *name, int stopNumber) {
    BusStop *newNode = malloc(sizeof(BusStop));
    if (!newNode) {
        perror("Allocation failed");
        exit(EXIT_FAILURE);
    }
    strncpy(newNode->name, name, MAX_STR_LEN - 1);
    newNode->name[MAX_STR_LEN - 1] = '\0';
    newNode->stopNumber = stopNumber;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

// Appends a new node to the end of the route
void appendStop(BusStop **head, BusStop **tail, const char *name, int stopNumber) {
    BusStop *newNode = createNode(name, stopNumber);
    if (*head == NULL) {
        *head = newNode;
        *tail = newNode;
    } else {
        (*tail)->next = newNode;
        newNode->prev = *tail;
        *tail = newNode; // Tail pointer optimization maintains O(1) performance
    }
}

// Simulates traveling along the route with structured timed delays
void traverseRoute(BusStop *startNode, int forwardDirection) {
    if (!startNode) {
        printf("The bus route is currently empty.\n");
        return;
    }
    
    BusStop *current = startNode;
    printf("\n--- Commencing Route Traversal ---\n");
    while (current != NULL) {
        printf("Arriving at Stop #%d: [%s]\n", current->stopNumber, current->name);
        fflush(stdout);
        
        current = forwardDirection ? current->next : current->prev;
        if (current != NULL) {
            TIME_DELAY(3); // Precise 3-second delay execution
        }
    }
    printf("--- Destination Reached. Traversal terminated. ---\n\n");
}

// Cleanly releases all allocated list memory to prevent leaks
void freeRoute(BusStop *head) {
    BusStop *current = head;
    while (current != NULL) {
        BusStop *temp = current->next;
        free(current);
        current = temp;
    }
}

int main() {
    BusStop *head = NULL;
    BusStop *tail = NULL;
    int choice, num;
    char buffer[MAX_STR_LEN];

    printf("Initialize Bus Route Database:\n");
    printf("How many base entries would you like to create? ");
    if (scanf("%d", &num) != 1) return EXIT_FAILURE;

    for (int i = 0; i < num; i++) {
        printf("Enter name for stop %d: ", i + 1);
        scanf(" %[^\n]", buffer);
        appendStop(&head, &tail, buffer, i + 1);
    }

    do {
        printf("\n=== Navigation Controller Menu ===\n");
        printf("1. Move Forward (Home -> Campus)\n");
        printf("2. Move Backward (Campus -> Home)\n");
        printf("3. Add New Bus Stop at End of Route\n");
        printf("4. Exit Navigation System\n");
        printf("Select option: ");
        if (scanf("%d", &choice) != 1) break;

        switch (choice) {
            case 1:
                traverseRoute(head, 1);
                break;
            case 2:
                traverseRoute(tail, 0);
                break;
            case 3:
                printf("Enter name of new destination stop: ");
                scanf(" %[^\n]", buffer);
                int structuralID = (tail != NULL) ? (tail->stopNumber + 1) : 1;
                appendStop(&head, &tail, buffer, structuralID);
                printf("Stop successfully added to the end of the route.\n");
                break;
            case 4:
                printf("Shutting down navigation system...\n");
                break;
            default:
                printf("Invalid selection. Try again.\n");
        }
    } while (choice != 4);

    freeRoute(head);
    return EXIT_SUCCESS;
}
