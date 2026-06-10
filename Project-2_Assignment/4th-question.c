#include <stdio.h>
#include <stdlib.h>

#define NUM_NODES 7

// Utility function to map numerical matrix indices back to uppercase labels
char get_node_name(int idx) {
    return (char)('A' + idx);
}

// Utility function to map character inputs securely to array coordinates
int get_node_index(char name) {
    if (name >= 'a' && name <= 'z') name -= 32; // Normalize lowercase inputs
    if (name >= 'A' && name <= 'G') return name - 'A';
    return -1; // Out-of-bounds error indicator
}

int main() {
    // 7x7 Symmetric Adjacency Matrix representing network sessions
    // Vertices: A=0, B=1, C=2, D=3, E=4, F=5, G=6
    // Unlabeled edges (A-D, D-F) are set to 0, indicating no active communication
    int graph[NUM_NODES][NUM_NODES] = {
        {0,  6,  0,  0,  0,  0,  0},  // A
        {6,  0, 11,  5,  0,  0,  0},  // B
        {0, 11,  0, 17,  0,  0, 25},  // C
        {0,  5, 17,  0, 22,  0,  0},  // D
        {0,  0,  0, 22,  0, 10,  0},  // E
        {0,  0,  0,  0, 10,  0, 22},  // F
        {0,  0, 25,  0,  0, 22,  0}   // G
    };

    char input_node;
    printf("Enter the suspected compromised workstation node (A-G): ");
    if (scanf(" %c", &input_node) != 1) return 1;

    int start_node = get_node_index(input_node);
    if (start_node == -1) {
        printf("Error: Workstation lies outside tracked network topology.\n");
        return 1;
    }

    printf("\n--- Initiating Cyber Incident BFS Triage from Node %c ---\n", get_node_name(start_node));

    int max_transfer_time = -1;
    int highest_risk_node = -1;
    int neighbor_count = 0;

    // BFS First-Layer Neighborhood Frontier Loop
    for (int col = 0; col < NUM_NODES; col++) {
        int session_time = graph[start_node][col];
        if (session_time > 0) { // Found a valid 1-hop connected neighbor
            neighbor_count++;
            printf("  -> Discovered 1-Hop Peer Workstation: %c (Transfer Time: %d mins)\n", get_node_name(col), session_time);

            // Track the maximum communication intensity
            if (session_time > max_transfer_time) {
                max_transfer_time = session_time;
                highest_risk_node = col;
            }
        }
    }

    // Comprehensive Forensics Output Summary
    printf("\n--- Incident Forensic Analysis Report ---\n");
    if (neighbor_count == 0) {
        printf("Workstation %c is completely isolated. No direct propagation vectors detected.\n", get_node_name(start_node));
    } else {
        printf("Total Direct Workstations Exposed: %d\n", neighbor_count);
        printf("Highest Contagion Propagation Risk: Workstation %c\n", get_node_name(highest_risk_node));
        printf("Peak Intensity Metric: %d minutes of recorded transfer session logs.\n", max_transfer_time);
    }

    return 0;
}
