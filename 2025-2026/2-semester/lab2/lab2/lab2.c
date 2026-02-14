#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Node {
    double data;
    struct Node* next;
} Node;

int getSafeInt(const char* prompt) {
    int value;
    int result;
    while (1) {
        printf("%s", prompt);
        result = scanf("%d", &value);
        if (result == 1) {
            char nextChar = getchar();
            if (nextChar == '\n' || nextChar == ' ') {
                return value;
            }
            while (nextChar != '\n' && nextChar != EOF) nextChar = getchar();
        }
        else {
            while (getchar() != '\n');
        }
        printf("Invalid input. Please enter an integer.\n");
    }
}

Node* createNode(double value) {
    Node* newNode = (Node*)calloc(1, sizeof(Node)); 
    if (!newNode) {
        fprintf(stderr, "Memory allocation error!\n");
        return NULL;
    }
    newNode->data = value;
    return newNode;
}

void appendFast(Node** head, Node** tail, double value) {
    Node* newNode = createNode(value);
    if (!newNode) exit(1); 

    if (*head == NULL) {
        *head = newNode;
        *tail = newNode;
    }
    else {
        (*tail)->next = newNode;
        *tail = newNode; 
    }
}

void insertAfter(Node* prevNode, double value) {
    if (prevNode == NULL) return;

    Node* newNode = createNode(value);
    if (!newNode) return;

    newNode->next = prevNode->next;
    prevNode->next = newNode;
}

void printList(const Node* head) {
    if (head == NULL) {
        printf("List is empty.\n");
        return;
    }
    printf("List: ");
    while (head != NULL) {
        printf("%.2f -> ", head->data);
        head = head->next;
    }
    printf("NULL\n");
}

void freeList(Node** head) {
    Node* current = *head;
    while (current != NULL) {
        Node* next = current->next;
        free(current);
        current = next;
    }
    *head = NULL;
    printf("Memory freed.\n");
}

void processTask(Node* head) {
    if (head == NULL) {
        printf("List is empty.\n");
        return;
    }

    Node* minNode = head, * maxNode = head;
    Node* current = head->next;

    while (current != NULL) {
        if (current->data < minNode->data) minNode = current;
        if (current->data > maxNode->data) maxNode = current;
        current = current->next;
    }

    printf("\nFound Max: %.2f\n", maxNode->data);
    printf("Found Min: %.2f\n", minNode->data);

    double maxVal = maxNode->data;
    double minVal = minNode->data;

    insertAfter(maxNode, minVal);
    insertAfter(minNode, maxVal);

    printf("Task completed: Min inserted after Max, Max inserted after Min.\n");
}

int main() {
    Node* head = NULL;
    Node* tail = NULL; 

    printf("=== Optimized Linked List Manager ===\n\n");

    int n = getSafeInt("Enter number of elements (n > 0): ");
    while (n <= 0) {
        n = getSafeInt("Please enter a positive number: ");
    }

    srand((unsigned int)time(NULL));

    printf("\nChoose input method:\n1 - Random\n2 - Manual\n");
    int choice = getSafeInt("Choice: ");

    printf("\nGenerating list...\n");
    for (int i = 0; i < n; i++) {
        double val;
        if (choice == 1) {
            val = ((double)rand() / RAND_MAX) * 200.0 - 100.0;
        }
        else {
            printf("Element %d: ", i + 1);
            if (scanf("%lf", &val) != 1) {
                printf("Invalid input. Using 0.0\n");
                val = 0.0;
                while (getchar() != '\n'); 
            }
        }
        appendFast(&head, &tail, val);
    }

    printf("\n--- Before Processing ---\n");
    printList(head);

    processTask(head);

    printf("\n--- After Processing ---\n");
    printList(head);

    freeList(&head);
    return 0;
}