#include <stdio.h>
#include <stdlib.h>

#define V 7 // Number of smart city hubs (A through G)

// Structure to represent a fiber-optic link (edge)
typedef struct {
    int u, v, weight;
} Edge;

// Structure for Union-Find (Disjoint Set)
typedef struct {
    int parent;
    int rank;
} Subset;

// Find set of an element i (uses path compression)
int find(Subset subsets[], int i) {
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);
    return subsets[i].parent;
}

// Function that unites two sets (uses union by rank)
void Union(Subset subsets[], int x, int y) {
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);

    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;
    else {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

// Comparator function for qsort to sort edges by weight
int compareEdges(const void* a, const void* b) {
    Edge* a1 = (Edge*)a;
    Edge* b1 = (Edge*)b;
    return a1->weight > b1->weight;
}

int main() {
    // Task 1: The Adjacency Matrix
    // A=0, B=1, C=2, D=3, E=4, F=5, G=6
    // Unlabeled edges explicitly set to 0.
    int adjMatrix[V][V] = {
        {0, 6, 0, 0, 0, 0, 0},
        {6, 0, 11, 5, 0, 0, 0},
        {0, 11, 0, 17, 0, 0, 25},
        {0, 5, 17, 0, 22, 0, 0},
        {0, 0, 0, 22, 0, 10, 0},
        {0, 0, 0, 0, 10, 0, 22},
        {0, 0, 25, 0, 0, 22, 0}
    };

    char hubs[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};
    Edge edges[20]; 
    int edgeCount = 0;

    // Extract edges from the upper triangle of the matrix to avoid duplicates
    for (int i = 0; i < V; i++) {
        for (int j = i + 1; j < V; j++) {
            if (adjMatrix[i][j] != 0) {
                edges[edgeCount].u = i;
                edges[edgeCount].v = j;
                edges[edgeCount].weight = adjMatrix[i][j];
                edgeCount++;
            }
        }
    }

    // Task 2: Sort edges for Kruskal's
    qsort(edges, edgeCount, sizeof(edges[0]), compareEdges);

    // Allocate memory for subsets
    Subset* subsets = (Subset*)malloc(V * sizeof(Subset));
    for (int v = 0; v < V; ++v) {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }

    Edge mst[V]; // Array to store the final MST
    int e = 0;   // Index for mst[]
    int i = 0;   // Index for sorted edges
    int totalCost = 0;

    printf("--- Kruskal's Algorithm Execution ---\n");

    // Task 3: Execute Kruskal's and build the MST
    while (e < V - 1 && i < edgeCount) {
        Edge next_edge = edges[i++];

        int x = find(subsets, next_edge.u);
        int y = find(subsets, next_edge.v);

        // If including this edge does not cause a cycle (x != y)
        if (x != y) {
            mst[e++] = next_edge;
            totalCost += next_edge.weight;
            Union(subsets, x, y);
            printf("Accepted: %c - %c (Cost: %d)\n", hubs[next_edge.u], hubs[next_edge.v], next_edge.weight);
        } else {
            printf("Rejected: %c - %c (Cost: %d) -> Creates cycle\n", hubs[next_edge.u], hubs[next_edge.v], next_edge.weight);
        }
    }

    // Task 4: Output the final budget summary
    printf("\n--- Final Network Architecture ---\n");
    printf("Selected Fiber-Optic Links:\n");
    for (i = 0; i < e; ++i) {
        printf("%c - %c\n", hubs[mst[i].u], hubs[mst[i].v]);
    }
    printf("\nTotal Minimum Installation Expenditure: $%d Million Dollars\n", totalCost);

    free(subsets);
    return 0;
}
