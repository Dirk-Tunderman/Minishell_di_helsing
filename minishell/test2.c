#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_env
{
    char *key;
    char *value;
    struct s_env *next;
} t_env;

void sortLinkedList(t_env **head)
{
    if (*head == NULL || (*head)->next == NULL)
        return;

    int i;

    i = 0;
    while (i == 0)
    {
        i = 1;
        t_env *current = *head;
        t_env *previous = NULL;

        while (current->next != NULL)
        {
            if (strcmp(current->key, current->next->key) > 0)
            {
                i = 0;
                t_env *temp = current->next;
                current->next = temp->next;
                temp->next = current;

                if (previous != NULL)
                    previous->next = temp;
                else
                    *head = temp;

                current = temp;
            }

            previous = current;
            current = current->next;
        }
    }
}

int main()
{
    // Create a sample linked list
    t_env *head = NULL;

    t_env *node1 = (t_env *)malloc(sizeof(t_env));
    node1->key = "Dety";
    node1->value = "Value D";
    node1->next = NULL;

    t_env *node2 = (t_env *)malloc(sizeof(t_env));
    node2->key = "Bdhdety";
    node2->value = "Value B";
    node2->next = NULL;

    t_env *node3 = (t_env *)malloc(sizeof(t_env));
    node3->key = "ABhsh";
    node3->value = "Value C";
    node3->next = NULL;

    t_env *node4 = (t_env *)malloc(sizeof(t_env));
    node4->key = "ACrth";
    node4->value = "Value A";
    node4->next = NULL;

    head = node1;
    node1->next = node2;
    node2->next = node3;
    node3->next = node4;

    // Before sorting
    t_env *current = head;
    printf("Before sorting:\n");
    while (current != NULL)
    {
        printf("%s\n", current->key);
        current = current->next;
    }

    // Sort the linked list
    sortLinkedList(&head);

    // After sorting
    printf("\nAfter sorting:\n");
    current = head;
    while (current != NULL)
    {
        printf("%s\n", current->key);
        current = current->next;
    }

    return 0;
}