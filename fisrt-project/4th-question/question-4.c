#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_LEN 100

typedef struct StudentDataNode {
    char firstName[STR_LEN];
    double grade;
    struct StudentDataNode *next;
} StudentDataNode;

typedef struct BSTNode {
    char lastName[STR_LEN];
    StudentDataNode *dataHead;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

BSTNode* createBSTNode(const char *ln, const char *fn, double gr) {
    BSTNode *node = malloc(sizeof(BSTNode));
    if (!node) return NULL;
    strncpy(node->lastName, ln, STR_LEN - 1);
    node->lastName[STR_LEN - 1] = '\0';
    
    StudentDataNode *dNode = malloc(sizeof(StudentDataNode));
    strncpy(dNode->firstName, fn, STR_LEN - 1);
    dNode->firstName[STR_LEN - 1] = '\0';
    dNode->grade = gr;
    dNode->next = NULL;

    node->dataHead = dNode;
    node->left = node->right = NULL;
    return node;
}

void insertBST(BSTNode **root, const char *ln, const char *fn, double gr) {
    if (*root == NULL) {
        *root = createBSTNode(ln, fn, gr);
        return;
    }

    int cmp = strcmp(ln, (*root)->lastName);
    if (cmp < 0) insertBST(&((*root)->left), ln, fn, gr);
    else if (cmp > 0) insertBST(&((*root)->right), ln, fn, gr);
    else {
        StudentDataNode *dNode = malloc(sizeof(StudentDataNode));
        strncpy(dNode->firstName, fn, STR_LEN - 1);
        dNode->firstName[STR_LEN - 1] = '\0';
        dNode->grade = gr;
        dNode->next = (*root)->dataHead;
        (*root)->dataHead = dNode;
    }
}

void searchBST(BSTNode *root, const char *target) {
    if (!root) {
        printf("No records found for: '%s'.\n", target);
        return;
    }
    int cmp = strcmp(target, root->lastName);
    if (cmp < 0) searchBST(root->left, target);
    else if (cmp > 0) searchBST(root->right, target);
    else {
        printf("\nMatches found for '%s':\n", target);
        StudentDataNode *curr = root->dataHead;
        while (curr) {
            printf("- %s | Grade: %.2f\n", curr->firstName, curr->grade);
            curr = curr->next;
        }
    }
}

void freeBST(BSTNode *root) {
    if (!root) return;
    freeBST(root->left);
    freeBST(root->right);
    
    StudentDataNode *curr = root->dataHead;
    while (curr != NULL) {
        StudentDataNode *temp = curr->next;
        free(curr);
        curr = temp;
    }
    free(root);
}

int main() {
    FILE *file = fopen("raw_students.txt", "r");
    if (!file) {
        perror("Error: Missing raw_students.txt");
        return EXIT_FAILURE;
    }

    BSTNode *root = NULL;
    char fName[STR_LEN], lName[STR_LEN];
    double gradeVal;

    while (fscanf(file, " %99[^|]|%99[^|]|%lf", fName, lName, &gradeVal) == 3) {
        insertBST(&root, lName, fName, gradeVal);
    }
    fclose(file);

    char query[STR_LEN];
    printf("Enter Student Last Name to search: ");
    scanf("%99s", query);
    searchBST(root, query);

    freeBST(root);
    return 0;
}
