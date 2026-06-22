#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCEDURES 50
#define MAX_STR_LEN 64

// --- BST Node Structure ---
typedef struct Node {
    char procedure[MAX_STR_LEN];
    struct Node *left;
    struct Node *right;
} Node;

// Global variables for tracking tracking state
int procedure_count = 0;
char closest_suggestion[MAX_STR_LEN];
int min_distance = 999;

// --- Helper Functions ---
Node* create_node(const char *proc) {
    Node *new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    strncpy(new_node->procedure, proc, MAX_STR_LEN - 1);
    new_node->procedure[MAX_STR_LEN - 1] = '\0';
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

Node* insert(Node *root, const char *proc) {
    if (procedure_count >= MAX_PROCEDURES) {
        return root; // Constraint strictly enforced
    }
    
    if (root == NULL) {
        procedure_count++;
        return create_node(proc);
    }

    int cmp = strcmp(proc, root->procedure);
    if (cmp < 0) {
        root->left = insert(root->left, proc);
    } else if (cmp > 0) {
        root->right = insert(root->right, proc);
    }
    return root;
}

// Exact match binary search lookup
int search(Node *root, const char *proc) {
    if (root == NULL) return 0;
    
    int cmp = strcmp(proc, root->procedure);
    if (cmp == 0) return 1;
    if (cmp < 0) return search(root->left, proc);
    return search(root->right, proc);
}

// Levenshtein Distance for similarity analysis
int levenshtein(const char *s1, const char *s2) {
    int len1 = strlen(s1);
    int len2 = strlen(s2);
    int matrix[len1 + 1][len2 + 1];

    for (int i = 0; i <= len1; i++) matrix[i][0] = i;
    for (int j = 0; j <= len2; j++) matrix[0][j] = j;

    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
            int delete_val = matrix[i - 1][j] + 1;
            int insert_val = matrix[i][j - 1] + 1;
            int substitute_val = matrix[i - 1][j - 1] + cost;
            
            int min = delete_val < insert_val ? delete_val : insert_val;
            matrix[i][j] = min < substitute_val ? min : substitute_val;
        }
    }
    return matrix[len1][len2];
}

// Traverse tree to find the absolute closest matching string
void find_closest(Node *root, const char *input) {
    if (root == NULL) return;

    int dist = levenshtein(input, root->procedure);
    if (dist < min_distance) {
        min_distance = dist;
        strcpy(closest_suggestion, root->procedure);
    }

    find_closest(root->left, input);
    find_closest(root->right, input);
}

void log_unauthorized(const char *input) {
    FILE *log_file = fopen("audit.log", "a");
    if (log_file) {
        fprintf(log_file, "[REJECTED ALERT] Unknown entry attempt: %s\n", input);
        fclose(log_file);
    }
}

void free_tree(Node *root) {
    if (root == NULL) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

void ensure_sample_file() {
    FILE *f = fopen("procedures.txt", "r");
    if (!f) {
        f = fopen("procedures.txt", "w");
        if (f) {
            fprintf(f, "LOCK_PANEL\nRESET_SENSOR\nCALIBRATE_ARM\nRESTART_LINE\n");
            fclose(f);
        }
    } else {
        fclose(f);
    }
}

// --- Main Verification Execution ---
int main() {
    ensure_sample_file(); // Self-contained initialization step

    Node *root = NULL;
    FILE *file = fopen("procedures.txt", "r");
    if (!file) {
        printf("Error: Could not open procedures.txt\n");
        return 1;
    }

    char buffer[MAX_STR_LEN];
    while (fgets(buffer, sizeof(buffer), file) && procedure_count < MAX_PROCEDURES) {
        // Strip trailing newlines securely
        buffer[strcspn(buffer, "\r\n")] = '\0';
        if (strlen(buffer) > 0) {
            root = insert(root, buffer);
        }
    }
    fclose(file);

    printf("Secure Maintenance Procedure Validator Loaded (%d procedures).\n", procedure_count);
    printf("Enter a procedure to verify (or type 'EXIT' to quit):\n\n");

    while (1) {
        printf("Technician Input -> ");
        if (!fgets(buffer, sizeof(buffer), stdin)) break;
        buffer[strcspn(buffer, "\r\n")] = '\0';

        if (strcmp(buffer, "EXIT") == 0) break;
        if (strlen(buffer) == 0) continue;

        if (search(root, buffer)) {
            printf("[ACCESS APPROVED] Execution authorized for: %s\n\n", buffer);
        } else {
            min_distance = 999; // Reset state tracking
            find_closest(root, buffer);
            
            printf("[ACCESS REJECTED] Unknown entry pattern detected.\n");
            log_unauthorized(buffer);
            printf(" -> Audit log updated.\n");
            
            // Suggesting close alternative if edit threshold is reasonable
            if (min_distance <= 5) {
                printf(" -> Did you mean: %s?\n\n", closest_suggestion);
            } else {
                printf(" -> No clear structural match found.\n\n");
            }
        }
    }

    // Clean teardown for a perfect Valgrind report card
    free_tree(root);
    printf("Memory safely cleared. System closed.\n");
    return 0;
}
