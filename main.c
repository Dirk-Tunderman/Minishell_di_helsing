#include "minishell.h"

// https://docs.rtems.org/releases/4.5.1-pre3/toolsdoc/gdb-5.0-docs/readline/readline00030.html

char	*ft_strdup(const char *s1)
{
	int		a;
	char		*str;

	a = 0;
	str = ft_calloc (ft_strlen(s1) + 1, 1);
	if (str == 0)
		return (0);
	while (a < ft_strlen(s1))
	{
		str[a] = s1[a];
		a++;
	}
	str[a] = 0;
	return (str);
}

void	resolve_path_and_commands(t_node *head, t_env *l_env)
{
    resolve_path(head, l_env);
}

int exit_status(int value)
{
    static int status;

    if (value > 255)
        return (status);
    else
        status = value;
    return (0);
}

void print_envp(char **envp) {
    int i = 0;
    while (envp[i]) {
        printf("%s\n", envp[i]);
        i++;
    }
}

int cnt_env(t_env *current) {
    int x = 0;
    while (current) {
        current = current->next;
    x++;
    }
    return x;
}
int count_cmd(t_node *head)
{
    int count = 1;
    while (head)
    {
        if (head->type == OPERATOR)
            count++;
        head = head->next;
    }
    return (count);
}


void    check_here_doc_before_exp(t_node *head)
{
    t_node *current = head;
    while (current)
    {
        if (current->type == HERDOC && current->next->before_env != NULL)
            current->next->data = current->next->before_env;
        current = current->next;
    }
}

//--------------------------------------------------------------------------------------//

t_env *duplicate_env(t_env *env)
{
    t_env *res;
    t_env *head;

    res = 0;
    if (env)
        res = malloc(sizeof(t_env));
    head = res;
    while (env)
    {
        res->key = m_ft_strdup(env->key);
        res->value = m_ft_strdup(env->value);
        res->only_export = env->only_export;
        env = env->next;
        if (env)
        {
            res->next = malloc(sizeof(t_env));
            res = res->next;
        }
        else
            res->next = 0;
    }
    return (head);
}

void alloc_wrap_env(t_env *env)
{
    while (env)
    {
        alloc_wrap (env);
        alloc_wrap(env->key);
        alloc_wrap(env->value);
        env = env->next;
    }
}

int main_loop(t_node *head, char **envp)
{
	char    *input;
	static t_env *l_env;

    input = "";
    l_env = envp_to_linked_list(envp);
    save_alloc(l_env);
	while (1)
	{
        g_signal_rec = 0;
       // if(!input)
       //     printf("\n");
        sig_init();
        input = readline("minishell$: ");
        signal(SIGINT, SIG_IGN);
        if (!input)
            fail_exit();
        alloc_wrap(input);
		if (input && ft_strlen(input) && (1) && checker_quotes(input))
        {
            printf("executing \"%s\"\n", input);
            add_history(input);
            printf("input: %s\n", input);
		    lexer(input, &head, l_env);
        	set_redirect_in_nodes(head);
		    if (error_all_check(head))
		    	continue;
		    resolve_path_and_commands(head, l_env);
            check_here_doc_before_exp(head);
            printf("\ndisplaylist:\n\n");
		    display_list(head);
		    printf("\n");
		    printf("----------------------------------------------------------------------------\n");
            t_comparsed *parsed = parsed_single_cmd(head, count_cmd(head), exit_status(256), envp);
            if (execute_list(parsed, l_env, envp, head)== SYSCALLFAIL)
                parsed->exit_status = 1;
            exit_status(parsed->exit_status);
            restore_fds();
            fail(0, 0);
            l_env = alloc_wrapper(0, 2, 0);
            alloc_wrap_env(l_env);
		    head = NULL;
            input = 0;
        }
	}
	return (0);
}

void ex()
{
    system("leaks minishell");
}

int     main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;

    atexit(ex);
    rl_catch_signals = 0;
    restore_fds();
	return (main_loop(0, envp));
}

