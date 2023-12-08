#include "../minishell.h"

void    init_data(t_data *data, t_cmd *cmd, t_env *env, char *input)
{
    data->cmd = cmd;
    data->line = input;
    data->env = env;

}

void    init_cmd(t_cmd *cmd, t_hrd *hrd, char *input)
{
    cmd->cmd = NULL;
    cmd->line = input;
    cmd->opt = NULL;
    cmd->arg = NULL;
    cmd->x = 0;
    cmd->hrd = hrd;
}

void    init_env(t_env *env)
{
    env->key = NULL;
    env->value = NULL;
    env->next = NULL;
}

void    init_hrd(t_hrd *hrd)
{
    hrd->cmd = NULL;
    hrd->f_name = NULL;
    hrd->x = 0;
}