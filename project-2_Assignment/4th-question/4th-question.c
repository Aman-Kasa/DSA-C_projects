#include <stdio.h>
#include <stdlib.h>

#define TOTAL_VERTICES 10

char resolveNodeCharacter(int position) {
    if (position == 9) return 'J';
    return (char)('A' + position);
}

int resolveNodeIndex(char marker) {
    if (marker >= 'a' && marker <= 'z') marker -= 32;
    if (marker == 'J') return 9;
    if (marker >= 'A' && marker <= 'I') return marker - 'A';
    return -1;
}

int main() {
    // 10x10 Transaction Adjacency Weight Matrix Modeling
    int networkTopology[TOTAL_VERTICES][TOTAL_VERTICES] = {
        {0,  4,  0,  16, 0,  0,  0,  0,  0,  0}, // Workstation A
        {4,  0,  6,  6,  0,  0,  0,  0,  0,  7}, // Workstation B
        {0,  6,  0,  0,  0,  0,  9,  0,  0,  0}, // Workstation C
        {16, 6,  0,  0,  7,  0,  0,  0,  0,  0}, // Workstation D
        {0,  0,  0,  7,  0,  10, 0,  0,  2,  0}, // Workstation E
        {0,  0,  0,  0,  10, 0,  0,  0,  10, 3}, // Workstation F
        {0,  0,  9,  0,  0,  0,  0,  13, 0,  0}, // Workstation G
        {0,  0,  0,  0,  0,  0,  13, 0,  0,  0}, // Workstation H
        {0,  0,  0,  0,  2,  10, 0,  0,  0,  0}, // Workstation I
        {0,  7,  0,  0,  0,  3,  0,  0,  0,  0}  // Workstation J
    };

    char inputToken;
    printf("[AUDIT ENGINE]: Enter suspected compromised workstation marker (A-J): ");
    if (scanf(" %c", &inputToken) != 1) return 1;

    int evaluationPivot = resolveNodeIndex(inputToken);
    if (evaluationPivot == -1) {
        printf("[CRITICAL UNKNOWN]: Entry marker out of structural domain bounds.\n");
        return 1;
    }

    printf("\n--- BFS One-Hop Direct Neighbor Exposure Tracking ---\n");
    int peakWeightValue = -1;
    int criticalContagionIndex = -1;
    int neighborCount = 0;

    for (int trackingColumn = 0; trackingColumn < TOTAL_VERTICES; trackingColumn++) {
        int transactionWeight = networkTopology[evaluationPivot][trackingColumn];
        if (transactionWeight > 0) {
            neighborCount++;
            printf("  -> Discovered One-Hop Node Link: %c (Logging Session: %d mins)\n", 
                   resolveNodeCharacter(trackingColumn), transactionWeight);
            
            if (transactionWeight > peakWeightValue) {
                peakWeightValue = transactionWeight;
                criticalContagionIndex = trackingColumn;
            }
        }
    }

    printf("\n--- Final Forensics Forensic Conclusion Report ---\n");
    if (neighborCount > 0) {
        printf("Total Lateral Contagion Footprint Vectors Detected: %d\n", neighborCount);
        printf("Highest Contagion Vector Identified: Workstation %c with transaction duration of %d mins.\n", 
               resolveNodeCharacter(criticalContagionIndex), peakWeightValue);
    } else {
        printf("Target station is fully isolated. No neighboring nodes uncovered via BFS layer tracking.\n");
    }
    printf("========================================================================\n");

    return 0;
}

