#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Node {
    char procedure[50];
    struct Node *left, *right;
} Node;

// BST Insertion
Node* insert(Node* root, char* val) {
    if (!root) {
        Node* n = (Node*)malloc(sizeof(Node));
        strcpy(n->procedure, val);
        n->left = n->right = NULL;
        return n;
    }
    int cmp = strcmp(val, root->procedure);
    if (cmp < 0) root->left = insert(root->left, val);
    else if (cmp > 0) root->right = insert(root->right, val);
    return root;
}

// Search for exact match
int search(Node* root, char* val) {
    if (!root) return 0;
    if (strcmp(val, root->procedure) == 0) return 1;
    return (strcmp(val, root->procedure) < 0) ? search(root->left, val) : search(root->right, val);
}

// Levenshtein distance for similarity suggestions
int editDistance(char* s1, char* s2) {
    int l1 = strlen(s1), l2 = strlen(s2);
    int d[l1 + 1][l2 + 1];
    for (int i = 0; i <= l1; i++) d[i][0] = i;
    for (int j = 0; j <= l2; j++) d[0][j] = j;
    for (int i = 1; i <= l1; i++) {
        for (int j = 1; j <= l2; j++) {
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
            int min = d[i - 1][j] + 1;
            if (d[i][j - 1] + 1 < min) min = d[i][j - 1] + 1;
            if (d[i - 1][j - 1] + cost < min) min = d[i - 1][j - 1] + cost;
            d[i][j] = min;
        }
    }
    return d[l1][l2];
}

// Find closest match in BST
void findClosest(Node* root, char* target, char* bestMatch, int* minDist) {
    if (!root) return;
    int d = editDistance(target, root->procedure);
    if (d < *minDist) { *minDist = d; strcpy(bestMatch, root->procedure); }
    findClosest(root->left, target, bestMatch, minDist);
    findClosest(root->right, target, bestMatch, minDist);
}

void logAttempt(char* input) {
    FILE* f = fopen("audit.log", "a");
    if (!f) return;
    time_t t = time(NULL);
    fprintf(f, "[%s] Rejected: %s\n", strtok(ctime(&t), "\n"), input);
    fclose(f);
}

void freeTree(Node* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    Node* root = NULL;
    FILE* file = fopen("procedures.txt", "r");
    char buffer[50];

    // Load up to 50 procedures
    int count = 0;
    while (count < 50 && fscanf(file, "%s", buffer) != EOF) {
        root = insert(root, buffer);
        count++;
    }
    fclose(file);

    printf("Enter maintenance procedure: ");
    scanf("%s", buffer);

    if (search(root, buffer)) {
        printf("Access Approved.\n");
    } else {
        char bestMatch[50] = "";
        int minDist = 99;
        findClosest(root, buffer, bestMatch, &minDist);
        printf("Unknown entry. Did you mean: %s?\n", bestMatch);
        logAttempt(buffer);
    }

    freeTree(root);
    return 0;
}
