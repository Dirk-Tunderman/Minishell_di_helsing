#include "../minishell.h"

void sig_init(void)
{
    signal(SIGINT, sig_handler);
    signal(SIGQUIT, sig_handler);
}


void    sig_handler(int signo)
{
    if (signo == SIGINT)
    {
        printf("\n");
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
    else if (signo == SIGQUIT)
    {
        printf("Quit: 3\n");
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
}
