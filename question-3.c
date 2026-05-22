#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
    int id;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// Queue implementation to support level-order (BFS) node insertion
typedef struct QueueNode {
    TreeNode *treeNode;
    struct QueueNode *next;
} QueueNode;

void enqueue(QueueNode **head, QueueNode **tail, TreeNode *tNode) {
    if (!tNode) return;
    QueueNode *newNode = malloc(sizeof(QueueNode));
    newNode->treeNode = tNode;
    newNode->next = NULL;
    if (*tail == NULL) {
        *head = newNode;
        *tail = newNode;
    } else {
        (*tail)->next = newNode;
        *tail = newNode;
    }
}

TreeNode* dequeue(QueueNode **head, QueueNode **tail) {
    if (*head == NULL) return NULL;
    QueueNode *temp = *head;
    TreeNode *res = temp->treeNode;
    *head = (*head)->next;
    if (*head == NULL) *tail = NULL;
    free(temp);
    return res;
}

TreeNode* createTreeNode(int val) {
    TreeNode *node = malloc(sizeof(TreeNode));
    if (!node) {
        perror("Allocation error");
        exit(EXIT_FAILURE);
    }
    node->id = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Inserts nodes using a level-order strategy to ensure structural balance
void insertLevelOrder(TreeNode **root, int val) {
    TreeNode *newNode = createTreeNode(val);
    if (*root == NULL) {
        *root = newNode;
        return;
    }

    QueueNode *qHead = NULL, *qTail = NULL;
    enqueue(&qHead, &qTail, *root);

    while (qHead != NULL) {
        TreeNode *curr = dequeue(&qHead, &qTail);

        if (curr->left == NULL) {
            curr->left = newNode;
            break;
        } else {
            enqueue(&qHead, &qTail, curr->left);
        }

        if (curr->right == NULL) {
            curr->right = newNode;
            break;
        } else {
            enqueue(&qHead, &qTail, curr->right);
        }
    }
    
    // Clear any remaining queue allocations to prevent memory leaks
    while (qHead != NULL) dequeue(&qHead, &qTail);
}

// Recursively processes and prints all leaf nodes
void printLeafNodes(TreeNode *root) {
    if (!root) return;
    if (!root->left && !root->right) {
        printf("%d ", root->id);
        return;
    }
    printLeafNodes(root->left);
    printLeafNodes(root->right);
}

// Locates a target node and tracks its parent pointer
TreeNode* findNodeAndParent(TreeNode *root, int target, TreeNode **parent) {
    if (!root || root->id == target) return root;

    if (root->left && root->left->id == target) {
        *parent = root;
        return root->left;
    }
    if (root->right && root->right->id == target) {
        *parent = root;
        return root->right;
    }

    TreeNode *res = findNodeAndParent(root->left, target, parent);
    if (res) return res;
    return findNodeAndParent(root->right, target, parent);
}

// Prints the direct children of a given node
void printChildren(TreeNode *node) {
    if (!node) return;
    if (node->left) printf("%d ", node->left->id);
    if (node->right) printf("%d ", node->right->id);
}

// Traverses down to locate and print grandchildren
void printGrandchildren(TreeNode *node) {
    if (!node) return;
    int count = 0;
    if (node->left) {
        if (node->left->left) { printf("%d ", node->left->left->id); count++; }
        if (node->left->right) { printf("%d ", node->left->right->id); count++; }
    }
    if (node->right) {
        if (node->right->left) { printf("%d ", node->right->left->id); count++; }
        if (node->right->right) { printf("%d ", node->right->right->id); count++; }
    }
    if (count == 0) printf("None");
}

void freeTree(TreeNode *root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    // Dataset initializing exactly 68 sample mock component identifiers
    int components[68];
    for (int i = 0; i < 68; i++) {
        components[i] = 1000 + i; 
    }

    TreeNode *root = NULL;
    for (int i = 0; i < 68; i++) {
        insertLevelOrder(&root, components[i]);
    }

    printf("Root Identifier Node: %d\n", root->id);
    printf("Identified Leaf Nodes: ");
    printLeafNodes(root);
    printf("\n");

    int targetQuery;
    printf("\nEnter Target ID to query relational metadata: ");
    if (scanf("%d", &targetQuery) != 1) return EXIT_FAILURE;

    TreeNode *parent = NULL;
    TreeNode *targetNode = findNodeAndParent(root, targetQuery, &parent);

    if (!targetNode) {
        printf("Error: Target ID %d does not exist in the system architecture.\n", targetQuery);
    } else {
        // Parent query execution
        if (parent) printf("Parent Node: %d\n", parent->id);
        else printf("Parent Node: Target is the root element.\n");

        // Sibling query execution
        printf("Sibling Nodes: ");
        if (!parent) {
            printf("None (Root node does not have siblings)");
        } else {
            if (parent->left == targetNode) {
                if (parent->right) printf("%d", parent->right->id);
                else printf("None");
            } else {
                if (parent->left) printf("%d", parent->left->id);
                else printf("None");
            }
        }
        printf("\n");

        // Grandchildren query execution
        printf("Grandchildren Nodes: ");
        printGrandchildren(targetNode);
        printf("\n");
    }

    freeTree(root);
    return EXIT_SUCCESS;
}
