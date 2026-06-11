#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INF 999999
#define NUM_VERTICES 10
#define NUM_EDGES 15

typedef struct {
    int src;
    int dest;
    int weight;
} TransactionEdge;

int getBranchIndex(char label) { return label - 'A'; }
char getBranchLabel(int index) { return (char)('A' + index); }

void backtrackPath(int parent[], int vertex) {
    if (parent[vertex] == -1) {
        printf("%c", getBranchLabel(vertex));
        return;
    }
    backtrackPath(parent, parent[vertex]);
    printf(" -> %c", getBranchLabel(vertex));
}

void executeBellmanFord(TransactionEdge edges[], int V, int E, int sourceIdx) {
    int cumulative_cost[NUM_VERTICES];
    int parent_tracker[NUM_VERTICES];

    for (int i = 0; i < V; i++) {
        cumulative_cost[i] = INF;
        parent_tracker[i] = -1;
    }
    cumulative_cost[sourceIdx] = 0;

    for (int i = 1; i <= V - 1; i++) {
        for (int j = 0; j < E; j++) {
            int u = edges[j].src;
            int v = edges[j].dest;
            int weight = edges[j].weight;
            if (cumulative_cost[u] != INF && cumulative_cost[u] + weight < cumulative_cost[v]) {
                cumulative_cost[v] = cumulative_cost[u] + weight;
                parent_tracker[v] = u;
            }
        }
    }

    int dynamic_cycle_flag = 0;
    for (int j = 0; j < E; j++) {
        int u = edges[j].src;
        int v = edges[j].dest;
        int weight = edges[j].weight;
        if (cumulative_cost[u] != INF && cumulative_cost[u] + weight < cumulative_cost[v]) {
            dynamic_cycle_flag = 1;
            break;
        }
    }

    printf("========================================================================\n");
    printf("               INTERNAL RISK ANALYSIS & PATH AUDIT REPORT               \n");
    printf("========================================================================\n");
    
    if (dynamic_cycle_flag) {
        printf("[CRITICAL ALERT]: A negative-weight transaction cycle has been identified!\n");
        return;
    }

    printf("[CYCLES STATUS]: Verified Clear. No negative-weight cycles detected.\n\n");
    printf("%-18s %-22s %-30s\n", "Target Branch", "Minimum Cost", "Optimal Routing Sequence");
    printf("------------------------------------------------------------------------\n");

    for (int i = 0; i < V; i++) {
        printf("Branch %-11c ", getBranchLabel(i));
        if (cumulative_cost[i] == INF) {
            printf("%-22s %-30s\n", "INFINITY", "PATH DISCONNECTED");
        } else {
            printf("%-22d ", cumulative_cost[i]);
            backtrackPath(parent_tracker, i);
            printf("\n");
        }
    }
}

int main() {
    TransactionEdge edges[NUM_EDGES];
    edges[0]  = (TransactionEdge){getBranchIndex('A'), getBranchIndex('B'), 4};
    edges[1]  = (TransactionEdge){getBranchIndex('A'), getBranchIndex('D'), 16};
    edges[2]  = (TransactionEdge){getBranchIndex('B'), getBranchIndex('A'), 4};
    edges[3]  = (TransactionEdge){getBranchIndex('B'), getBranchIndex('C'), 6};
    edges[4]  = (TransactionEdge){getBranchIndex('B'), getBranchIndex('D'), 6};
    edges[5]  = (TransactionEdge){getBranchIndex('B'), getBranchIndex('J'), 7};
    edges[6]  = (TransactionEdge){getBranchIndex('C'), getBranchIndex('G'), 9};
    edges[7]  = (TransactionEdge){getBranchIndex('D'), getBranchIndex('B'), 6};
    edges[8]  = (TransactionEdge){getBranchIndex('D'), getBranchIndex('E'), 7};
    edges[9]  = (TransactionEdge){getBranchIndex('D'), getBranchIndex('J'), 0};
    edges[10] = (TransactionEdge){getBranchIndex('E'), getBranchIndex('I'), 2};
    edges[11] = (TransactionEdge){getBranchIndex('F'), getBranchIndex('G'), 0};
    edges[12] = (TransactionEdge){getBranchIndex('G'), getBranchIndex('F'), 0};
    edges[13] = (TransactionEdge){getBranchIndex('G'), getBranchIndex('H'), 13};
    edges[14] = (TransactionEdge){getBranchIndex('J'), getBranchIndex('F'), 3};

    executeBellmanFord(edges, NUM_VERTICES, NUM_EDGES, getBranchIndex('A'));
    return 0;
}

