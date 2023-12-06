#include "../minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

void sig_init(void)
{
    sigaction(SIGINT, sig_handler); // sigaction
    signal(SIGQUIT, sig_handler);
}


void    sig_handler(int signo)
{
    if (signo == SIGINT)
    {
       //printf("\n");
       //rl_replace_line("", 0);
       //rl_on_new_line();
       //rl_redisplay();
        FAIL
    }
    else if (signo == SIGQUIT)
    {
        printf("Quit: 3\n");

    }
}


