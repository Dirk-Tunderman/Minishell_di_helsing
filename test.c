#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/history.h>
#include <readline/readline.h>
void f ()
{
	system ("leaks a.out");
}

int main(int argc, char *argv[], char *envp[])
{
    while(*envp)
        printf("%s\n",*envp++);
}