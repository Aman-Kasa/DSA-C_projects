#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_AIRPORTS 20
#define NAME_LEN 4

// --- Graph Structure ---
typedef struct {
    char names[MAX_AIRPORTS][NAME_LEN];
    int adj_matrix[MAX_AIRPORTS][MAX_AIRPORTS];
    int count;
} Graph;

Graph graph;

// --- Helper: Find Airport Index ---
int get_index(const char *name) {
    for (int i = 0; i < graph.count; i++) {
        if (strcmp(graph.names[i], name) == 0) {
            return i;
        }
    }
    return -1;
}

// --- Dynamic Operations ---
void add_airport(const char *name) {
    if (graph.count >= MAX_AIRPORTS) {
        printf("[Error] Graph capacity reached.\n");
        return;
    }
    if (get_index(name) != -1) {
        printf("[Info] Airport %s already exists.\n", name);
        return;
    }
    
    strncpy(graph.names[graph.count], name, NAME_LEN - 1);
    graph.names[graph.count][NAME_LEN - 1] = '\0';
    
    // Clear rows/cols for the new node
    for (int i = 0; i <= graph.count; i++) {
        graph.adj_matrix[graph.count][i] = 0;
        graph.adj_matrix[i][graph.count] = 0;
    }
    
    graph.count++;
    printf("[Success] Added airport: %s\n", name);
}

void add_route(const char *src, const char *dest) {
    int u = get_index(src);
    int v = get_index(dest);
    
    if (u == -1 || v == -1) {
        printf("[Error] One or both airports do not exist.\n");
        return;
    }
    
    graph.adj_matrix[u][v] = 1; // Directed Edge
    printf("[Success] Added route: %s -> %s\n", src, dest);
}

void remove_route(const char *src, const char *dest) {
    int u = get_index(src);
    int v = get_index(dest);
    
    if (u == -1 || v == -1) {
        printf("[Error] Invalid route parameters.\n");
        return;
    }
    
    graph.adj_matrix[u][v] = 0;
    printf("[Success] Removed route: %s -> %s\n", src, dest);
}

void remove_airport(const char *name) {
    int idx = get_index(name);
    if (idx == -1) {
        printf("[Error] Airport %s not found.\n", name);
        return;
    }
    
    // Shift rows up in the matrix
    for (int i = idx; i < graph.count - 1; i++) {
        for (int j = 0; j < graph.count; j++) {
            graph.adj_matrix[i][j] = graph.adj_matrix[i + 1][j];
        }
    }
    
    // Shift columns left in the matrix
    for (int j = idx; j < graph.count - 1; j++) {
        for (int i = 0; i < graph.count; i++) {
            graph.adj_matrix[i][j] = graph.adj_matrix[i][j + 1];
        }
    }
    
    // Shift string array labels forward
    for (int i = idx; i < graph.count - 1; i++) {
        strcpy(graph.names[i], graph.names[i + 1]);
    }
    
    graph.count--;
    printf("[Success] Removed airport and all related routes for: %s\n", name);
}

// --- Query Operations ---
void query_airport(const char *name) {
    int idx = get_index(name);
    if (idx == -1) {
        printf("[Error] Airport %s does not exist.\n", name);
        return;
    }
    
    printf("\n--- Relationship Analysis for %s ---\n", name);
    
    // Outgoing Flights (Reachable Directly)
    printf("  Direct Flights OUTbound to: ");
    int found_out = 0;
    for (int j = 0; j < graph.count; j++) {
        if (graph.adj_matrix[idx][j] == 1) {
            printf("%s ", graph.names[j]);
            found_out = 1;
        }
    }
    if (!found_out) printf("None");
    printf("\n");
    
    // Incoming Flights (Direct Inbound)
    printf("  Direct Flights INbound from: ");
    int found_in = 0;
    for (int i = 0; i < graph.count; i++) {
        if (graph.adj_matrix[i][idx] == 1) {
            printf("%s ", graph.names[i]);
            found_in = 1;
        }
    }
    if (!found_in) printf("None");
    printf("\n-----------------------------------\n");
}

// --- Display Adjacency Matrix Matrix ---
void display_matrix() {
    printf("\n--- Adjacency Matrix Representation ---\n\n     ");
    for (int i = 0; i < graph.count; i++) {
        printf("%-5s", graph.names[i]);
    }
    printf("\n");
    
    for (int i = 0; i < graph.count; i++) {
        printf("%-5s", graph.names[i]);
        for (int j = 0; j < graph.count; j++) {
            printf("%-5d", graph.adj_matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n---------------------------------------\n");
}

// --- Base Dataset Seeding ---
void seed_default_data() {
    graph.count = 0;
    add_airport("KGL");
    add_airport("NBO");
    add_airport("EBB");
    add_airport("JNB");
    add_airport("ADD");
    add_airport("CAI");
    add_airport("CPT");

    add_route("KGL", "NBO");
    add_route("KGL", "EBB");
    add_route("NBO", "JNB");
    add_route("EBB", "ADD");
    add_route("ADD", "CAI");
    add_route("JNB", "CPT");
}

// --- Execution Loop ---
int main() {
    seed_default_data();
    int choice;
    char arg1[NAME_LEN], arg2[NAME_LEN];

    while (1) {
        printf("\n== Airline Route Relationship Analyzer ==\n");
        printf("1. Display Adjacency Matrix\n");
        printf("2. Query Airport Details\n");
        printf("3. Add Airport\n");
        printf("4. Remove Airport\n");
        printf("5. Add Route\n");
        printf("6. Remove Route\n");
        printf("7. Exit\n");
        printf("Selection -> ");
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); // flush invalid chars cleanly
            continue;
        }

        switch (choice) {
            case 1:
                display_matrix();
                break;
            case 2:
                printf("Enter Airport Code (e.g. KGL): ");
                scanf("%s", arg1);
                query_airport(arg1);
                break;
            case 3:
                printf("Enter New Airport Code: ");
                scanf("%s", arg1);
                add_airport(arg1);
                break;
            case 4:
                printf("Enter Airport to Remove: ");
                scanf("%s", arg1);
                remove_airport(arg1);
                break;
            case 5:
                printf("Enter Source Code: ");
                scanf("%s", arg1);
                printf("Enter Destination Code: ");
                scanf("%s", arg2);
                add_route(arg1, arg2);
                break;
            case 6:
                printf("Enter Source Code: ");
                scanf("%s", arg1);
                printf("Enter Destination Code: ");
                scanf("%s", arg2);
                remove_route(arg1, arg2);
                break;
            case 7:
                printf("Exiting Analyzer clean.\n");
                return 0;
            default:
                printf("Invalid selection.\n");
        }
    }
    return 0;
}
