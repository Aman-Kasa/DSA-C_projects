#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INF 999999
#define NUM_VERTICES 10
#define NUM_EDGES 15

// Structure to represent a weighted edge in the transaction network
typedef struct {
    int src, dest, weight;
} Edge;

// Helper function to map branch characters to zero-based array indices
int nodeIndex(char name) {
    return name - 'A';
}

// Helper function to map indices back to branch characters for printing
char nodeName(int index) {
    return (char)('A' + index);
}

// Recursive helper function to print the exact path from parent arrays
void printPathSequence(int parent[], int j) {
    if (parent[j] == -1) {
        printf("%c", nodeName(j));
        return;
    }
    printPathSequence(parent, parent[j]);
    printf(" -> %c", nodeName(j));
}

void runBellmanFord(Edge edges[], int V, int E, int source) {
    int distance[NUM_VERTICES];
    int parent[NUM_VERTICES];

    // Initialize all distances to infinity and parents to undefined
    for (int i = 0; i < V; i++) {
        distance[i] = INF;
        parent[i] = -1;
    }
    distance[source] = 0;

    // Task 2: Relax all edges exactly V - 1 times
    for (int i = 1; i <= V - 1; i++) {
        for (int j = 0; j < E; j++) {
            int u = edges[j].src;
            int v = edges[j].dest;
            int weight = edges[j].weight;
            if (distance[u] != INF && distance[u] + weight < distance[v]) {
                distance[v] = distance[u] + weight;
                parent[v] = u;
            }
        }
    }

    // Task 3: Execute the V-th pass to detect and report negative-weight cycles
    int hasNegativeCycle = 0;
    for (int j = 0; j < E; j++) {
        int u = edges[j].src;
        int v = edges[j].dest;
        int weight = edges[j].weight;
        if (distance[u] != INF && distance[u] + weight < distance[v]) {
            hasNegativeCycle = 1;
            break;
        }
    }

    printf("========================================================================\n");
    printf("                FINANCIAL RISK ROUTING AUDIT REPORT                     \n");
    printf("========================================================================\n");
    
    if (hasNegativeCycle) {
        printf("[CRITICAL ALERT]: Negative-weight cycle detected inside the network!\n");
        printf("Transaction channels are unstable, indicating potentially exploitable fraud/arbitrage.\n");
        return;
    }

    printf("[STATUS]: No negative-weight cycles detected. Transaction paths are stable.\n\n");
    printf("%-15s %-25s %-30s\n", "Target Branch", "Minimum Cost", "Optimal Path Sequence");
    printf("------------------------------------------------------------------------\n");

    for (int i = 0; i < V; i++) {
        printf("Branch %-9c ", nodeName(i));
        if (distance[i] == INF) {
            printf("%-25s %-30s\n", "UNREACHABLE", "None");
        } else {
            printf("%-25d ", distance[i]);
            printPathSequence(parent, i);
            printf("\n");
        }
    }
    printf("========================================================================\n");
}

int main() {
    Edge edges[NUM_EDGES];

    // Populating the exact edge network from the system graph diagram
    edges[0]  = (Edge){nodeIndex('A'), nodeIndex('B'), 4};
    edges[1]  = (Edge){nodeIndex('A'), nodeIndex('D'), 16};
    edges[2]  = (Edge){nodeIndex('B'), nodeIndex('A'), 4};
    edges[3]  = (Edge){nodeIndex('B'), nodeIndex('C'), 6};
    edges[4]  = (Edge){nodeIndex('B'), nodeIndex('D'), 6};
    edges[5]  = (Edge){nodeIndex('B'), nodeIndex('J'), 7};
    edges[6]  = (Edge){nodeIndex('C'), nodeIndex('G'), 9};
    edges[7]  = (Edge){nodeIndex('D'), nodeIndex('B'), 6};
    edges[8]  = (Edge){nodeIndex('D'), nodeIndex('E'), 7};
    edges[9]  = (Edge){nodeIndex('D'), nodeIndex('J'), 0}; // Unlabeled cross-channel
    edges[10] = (Edge){nodeIndex('E'), nodeIndex('I'), 2};
    edges[11] = (Edge){nodeIndex('F'), nodeIndex('G'), 0}; // Unlabeled cross-channel
    edges[12] = (Edge){nodeIndex('G'), nodeIndex('F'), 0}; // Unlabeled cross-channel
    edges[13] = (Edge){nodeIndex('G'), nodeIndex('H'), 13};
    edges[14] = (Edge){nodeIndex('J'), nodeIndex('F'), 3};

    // Run the routine tracking from central clearing center Branch A
    runBellmanFord(edges, NUM_VERTICES, NUM_EDGES, nodeIndex('A'));

    return 0;
}
