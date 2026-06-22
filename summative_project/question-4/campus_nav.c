#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_NODES 7
#define INF 999999
#define MAX_NAME_LEN 32

// --- Node Mapping ---
const char* node_names[NUM_NODES] = {
    "Library",            // 0
    "Cafeteria",          // 1
    "Engineering",        // 2
    "Science Block",      // 3
    "Dormitory",          // 4
    "Administration",     // 5
    "Charging Station"    // 6
};

int adj_matrix[NUM_NODES][NUM_NODES];

// --- Helper: Find Node Index ---
int get_node_index(const char* name) {
    for (int i = 0; i < NUM_NODES; i++) {
        if (strcasecmp(node_names[i], name) == 0) { // Case-insensitive matching
            return i;
        }
    }
    return -1;
}

// --- Initialize Graph Weights ---
void init_graph() {
    for (int i = 0; i < NUM_NODES; i++) {
        for (int j = 0; j < NUM_NODES; j++) {
            if (i == j) adj_matrix[i][j] = 0;
            else adj_matrix[i][j] = INF;
        }
    }

    // Connect undirected weighted paths based on requirements
    adj_matrix[0][1] = adj_matrix[1][0] = 6;  // Library --6-- Cafeteria
    adj_matrix[0][2] = adj_matrix[2][0] = 15; // Library --15-- Engineering
    adj_matrix[1][3] = adj_matrix[3][1] = 4;  // Cafeteria --4-- Science Block
    adj_matrix[3][4] = adj_matrix[4][3] = 8;  // Science Block --8-- Dormitory
    adj_matrix[2][5] = adj_matrix[5][2] = 5;  // Engineering --5-- Administration
    adj_matrix[5][4] = adj_matrix[4][5] = 3;  // Administration --3-- Dormitory
    adj_matrix[1][6] = adj_matrix[6][1] = 2;  // Cafeteria --2-- Charging Station
    adj_matrix[6][5] = adj_matrix[5][6] = 4;  // Charging Station --4-- Administration
}

// --- Path Reconstruction ---
void print_path(int parent[], int j) {
    if (parent[j] == -1) {
        printf("%s", node_names[j]);
        return;
    }
    print_path(parent, parent[j]);
    printf(" -> %s", node_names[j]);
}

// --- Dijkstra's Algorithm Implementation ---
void find_shortest_path(int start_node) {
    int dist[NUM_NODES];
    int visited[NUM_NODES];
    int parent[NUM_NODES];
    int dest_node = 4; // Dormitory is our explicit constant destination node

    for (int i = 0; i < NUM_NODES; i++) {
        dist[i] = INF;
        visited[i] = 0;
        parent[i] = -1;
    }

    dist[start_node] = 0;

    for (int count = 0; count < NUM_NODES - 1; count++) {
        // Find the node with the minimum distance from the unvisited set
        int min = INF, u = -1;
        for (int v = 0; v < NUM_NODES; v++) {
            if (!visited[v] && dist[v] <= min) {
                min = dist[v];
                u = v;
            }
        }

        if (u == -1) break; // Remaining nodes are unreachable
        visited[u] = 1;

        // Update the distance values of adjacent vertices
        for (int v = 0; v < NUM_NODES; v++) {
            if (!visited[v] && adj_matrix[u][v] != INF && dist[u] != INF 
                && dist[u] + adj_matrix[u][v] < dist[v]) {
                dist[v] = dist[u] + adj_matrix[u][v];
                parent[v] = u;
            }
        }
    }

    // Render results
    printf("\n============================================\n");
    printf("           NAVIGATION ROUTE REPORT          \n");
    printf("============================================\n");
    if (dist[dest_node] == INF) {
        printf("Error: No viable path exists from %s to the Dormitory.\n", node_names[start_node]);
    } else {
        printf("Starting Point : %s\n", node_names[start_node]);
        printf("Destination    : Dormitory\n");
        printf("Total Distance : %d meters\n", dist[dest_node]);
        printf("Optimized Path : ");
        print_path(parent, dest_node);
        printf("\n");
    }
    printf("============================================\n");
}

// --- Main Program Entry ---
int main() {
    init_graph();
    char input_buf[MAX_NAME_LEN];

    printf("Campus Delivery Robot Navigation System\n");
    printf("Available Locations:\n");
    for (int i = 0; i < NUM_NODES; i++) {
        printf(" - %s\n", node_names[i]);
    }
    printf("\n");

    while (1) {
        printf("Enter Starting Building Name (or 'exit' to quit): ");
        if (!fgets(input_buf, sizeof(input_buf), stdin)) break;

        // Clean newline markers secure
        input_buf[strcspn(input_buf, "\r\n")] = '\0';

        if (strcasecmp(input_buf, "exit") == 0) {
            printf("Shutting down navigation interface.\n");
            break;
        }

        int start_idx = get_node_index(input_buf);
        if (start_idx == -1) {
            printf("[Validation Error] '%s' is not an approved campus building. Please try again.\n\n", input_buf);
            continue;
        }

        find_shortest_path(start_idx);
        printf("\n");
    }

    return 0;
}
