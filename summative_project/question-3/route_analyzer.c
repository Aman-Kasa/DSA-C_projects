#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_AIRPORTS 20

typedef struct {
    char code[4];
    int exists;
} Airport;

Airport airports[MAX_AIRPORTS];
int adjMatrix[MAX_AIRPORTS][MAX_AIRPORTS];
int airportCount = 0;

// Find index of airport code
int findIndex(char* code) {
    for (int i = 0; i < MAX_AIRPORTS; i++) 
        if (airports[i].exists && strcmp(airports[i].code, code) == 0) return i;
    return -1;
}

// Requirement: Dynamic Updates (Add)
void addAirport(char* code) {
    for (int i = 0; i < MAX_AIRPORTS; i++) {
        if (!airports[i].exists) {
            strcpy(airports[i].code, code);
            airports[i].exists = 1;
            airportCount++;
            return;
        }
    }
}

// Requirement: Dynamic Updates (Route)
void setRoute(char* from, char* to, int active) {
    int u = findIndex(from);
    int v = findIndex(to);
    if (u != -1 && v != -1) adjMatrix[u][v] = active;
}

// Requirement: Adjacency Matrix
void printMatrix() {
    printf("\nAdjacency Matrix:\n    ");
    for (int i = 0; i < MAX_AIRPORTS; i++) if (airports[i].exists) printf("%s ", airports[i].code);
    printf("\n");
    for (int i = 0; i < MAX_AIRPORTS; i++) {
        if (!airports[i].exists) continue;
        printf("%s: ", airports[i].code);
        for (int j = 0; j < MAX_AIRPORTS; j++) {
            if (airports[j].exists) printf("%d  ", adjMatrix[i][j]);
        }
        printf("\n");
    }
}

// Requirement: Relationship Analysis (Direct)
void analyzeAirport(char* code) {
    int idx = findIndex(code);
    if (idx == -1) return;
    printf("\nAnalysis for %s:\nOutgoing: ", code);
    for (int i = 0; i < MAX_AIRPORTS; i++)
        if (airports[i].exists && adjMatrix[idx][i]) printf("%s ", airports[i].code);
    printf("\nIncoming: ");
    for (int i = 0; i < MAX_AIRPORTS; i++)
        if (airports[i].exists && adjMatrix[i][idx]) printf("%s ", airports[i].code);
    printf("\n");
}

// Requirement: Path Analysis (BFS)
bool pathExists(char* start, char* end) {
    int u = findIndex(start), v = findIndex(end);
    if (u == -1 || v == -1) return false;
    bool visited[MAX_AIRPORTS] = {false};
    int queue[MAX_AIRPORTS], head = 0, tail = 0;
    queue[tail++] = u; visited[u] = true;
    while (head < tail) {
        int curr = queue[head++];
        if (curr == v) return true;
        for (int i = 0; i < MAX_AIRPORTS; i++) {
            if (adjMatrix[curr][i] && !visited[i]) {
                visited[i] = true;
                queue[tail++] = i;
            }
        }
    }
    return false;
}

int main() {
    memset(adjMatrix, 0, sizeof(adjMatrix));
    for(int i=0; i<MAX_AIRPORTS; i++) airports[i].exists = 0;

    // Loading Sample Routes
    addAirport("KGL"); addAirport("NBO"); addAirport("EBB");
    setRoute("KGL", "NBO", 1); setRoute("NBO", "JNB", 1);
    
    printMatrix();
    analyzeAirport("KGL");
    
    if (pathExists("KGL", "JNB")) printf("Path found: KGL -> ... -> JNB\n");
    
    return 0;
}
