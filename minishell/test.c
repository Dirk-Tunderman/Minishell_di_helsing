#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char *data;
    struct Node *next;
} Node;

Node* createNode(char *data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = strdup(data);
    newNode->next = NULL;
    return newNode;
}

void insertNode(Node **head, char *data) {
    Node* newNode = createNode(data);
    if (*head == NULL) {
        *head = newNode;
    } else {
        Node* curr = *head;
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = newNode;
    }
}

Node* splitInput(char *input) {
    Node *head = NULL;
    char *part = NULL;
    char *saveptr = NULL;
    char *token = input;
    
    while ((part = strstr(token, "|")) != NULL) {
        *part = '\0';
        insertNode(&head, token);
        token = part + 1;
    }
    
    insertNode(&head, token);
    
    return head;
}

void printLinkedList(Node *head) {
    Node *curr = head;
    while (curr != NULL) {
        printf("%s\n", curr->data);
        curr = curr->next;
    }
}

void freeLinkedList(Node *head) {
    Node *curr = head;
    while (curr != NULL) {
        Node *temp = curr;
        curr = curr->next;
        free(temp->data);
        free(temp);
    }
}

int main() {
    char input[] = "command1  kjiuh|command2 |command3";
    Node *head = splitInput(input);
    printLinkedList(head);
    freeLinkedList(head);
    return 0;
}