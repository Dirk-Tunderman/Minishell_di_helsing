#include "minishell.h"

int g_signal_rec;

//https://stackoverflow.com/questions/53165704/readline-c-force-return-of-certain-text-in-readline

void sig_handler(int signo) 
{
    signo = 0;
    rl_replace_line("", 0);
    rl_on_new_line();
    printf("\n");
    rl_redisplay();
    exit_status(1);
    g_signal_rec = 1;
}

void sig_init(void)
{
    struct sigaction sa;

    sa.sa_handler = sig_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_NODEFER;
    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction");
    }
    signal(SIGQUIT, SIG_IGN);
}