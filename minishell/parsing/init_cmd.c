#include "../minishell.h"

// initialte the t_cmd struct

t_cmd* init_cmd_list() {
    t_cmd* head = (t_cmd*) malloc(sizeof(t_cmd));
    head->cmd = NULL;
    head->argv = (char**) malloc(sizeof(char*) * MAX_ARGS);
    memset(head->argv, 0, sizeof(char*) * MAX_ARGS);
    head->opt = NULL;
    head->x = 0;
    head->hrd = NULL;
    head->next = NULL;
    return head;
}


t_cmd* create_cmd_node() {
    t_cmd* cmd_node = (t_cmd*) malloc(sizeof(t_cmd));
    cmd_node->cmd = NULL;
    cmd_node->argv = (char**) malloc(sizeof(char*) * MAX_ARGS);
    memset(cmd_node->argv, 0, sizeof(char*) * MAX_ARGS);
    cmd_node->opt = NULL;
    cmd_node->x = 0;
    cmd_node->hrd = NULL;
    cmd_node->next = NULL;
    return cmd_node;
}

// void transfer_node_to_cmd(t_node* node, t_cmd* cmd) {
//     if (!node || !cmd) return;
//     t_cmd* current_cmd = cmd;
//     int i = 0;
//     while (node) {
//         switch(node->type) {
//             case OPERATOR:
//                 // Move to the next command
//                 current_cmd->next = create_cmd_node();
//                 current_cmd = current_cmd->next;
//                 i = 0;
//                 break;
//             case CMD:
//                 current_cmd->cmd = node->data;
//                 break;
//             default:
//                 // We are adding to the argv
//                 if (node->data && node->data[0] == '-') {
//                     current_cmd->opt = node->data;
//                 } else {
//                     current_cmd->argv[i] = node->data;
//                     i++;
//                 }
//                 break;
//         }
//         node = node->next;
//     }
// }

void display_cmd_list(t_cmd *head) {
    t_cmd *current = head;
    int i;

    while (current != NULL) {
        printf("Command: %s\n", current->cmd);

        if (current->argv != NULL) {
            printf("Arguments:\n");
            for (i = 0; current->argv[i] != NULL; i++) {
                printf("  argv[%d]: %s\n", i, current->argv[i]);
            }
        }

        if (current->opt != NULL) {
            printf("Option: %s\n", current->opt);
        }
        // If you have other fields you want to display, you can add them here.

        printf("\n");
        current = current->next;
    }
}
