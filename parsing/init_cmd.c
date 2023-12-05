#include "../minishell.h"


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
    ft_memset(cmd_node->argv, 0, sizeof(char*) * MAX_ARGS);
    cmd_node->opt = NULL;
    cmd_node->x = 0;
    cmd_node->hrd = NULL;
    cmd_node->next = NULL;
    return cmd_node;
}
