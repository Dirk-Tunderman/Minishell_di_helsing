/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 13:48:06 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/05 10:43:47 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "get_next_line.h"

void	*ft_calloc(size_t count, size_t size)
{
	unsigned long long		a;
	char					*ptr;

	a = 0;
	ptr = malloc(count * size);
	if (ptr == 0)
		return (0);
	while (a < count * size)
	{
		ptr[a++] = 0;
	}
	return (ptr);
}

void	ft_bzero(void *s, size_t n)
{
	size_t		a;
	char		*sc;

	a = 0;
	sc = (char *) s;
	while (a < n)
	{
		sc[a] = 0;
		a++;
	}
}

int is_builtin(char *cmd)
{
	if (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "pwd") 
	|| !ft_strcmp(cmd, "export")  || !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env")
	|| !ft_strcmp(cmd, "exit"))
		return (1);
	return (0);
}

void apply_garbage_redir(char ***garbage_redirects_arr)
{
	int x;
	int fd;

	x = 0;
	while (garbage_redirects_arr[0][x]) // OVERRIDE
	{
		fd = open(garbage_redirects_arr[0][x], O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
		if (fd == -1)
			printf("Error opening file %s\n", garbage_redirects_arr[0][x]);
		else
			close(fd);
		x++;
	}
	x = 0;
	while (garbage_redirects_arr[1][x]) // APPEND
	{
		fd = open(garbage_redirects_arr[1][x], O_WRONLY | O_CREAT, S_IRWXU);
		if (fd == -1)
			printf("Error opening file %s\n", garbage_redirects_arr[1][x]);
		else
			close(fd);
		x++;
	}
}

int det_subshell(t_comparsed *cmds)
{
	int subshell;
	
	subshell = 1;
	if (cmds->cmd_count == 1) // check for empty commandline
		subshell = 0;
//	if (x == cmds->cmd_count - 1 && is_builtin(cmds->exec_ready[x][0]))
	//	subshell = 0;
	return (subshell);
}

/*If the parent process does not close the write-end of the pipe,
then the child will block forever in the call to read waiting more more data.
(The read call will block if there's any open file descriptor associated with the write-end of the pipe.)
*/

void set_pipe(int *_pipe, int sv_next)
{
// if sv_next is set, it is meat for you, use it and set it again if there is someone following you
	if (sv_next != -1)
		close(_pipe[1]);
	if (pipe(_pipe))
		fail(0); //<<--- reset this
}

// pipes, just take the endpoint you need and forget the other
// redirectors, you have to dup the respective endpoint

void overridefds(t_comparsed *cmds, int x, int *_pipe, int sv_next, char *cmd)
{
	if (cmds->fds[x][3] != -1)
	{
		if(dup2(cmds->fds[x][3], STDOUT_FILENO) == -1)
			fail(0);// reset instead
	}
	else if (cmds->fds[x][4] != -1)
	{
		if (dup2(cmds->fds[x][4], STDOUT_FILENO) == -1)
			fail(0);
	}
	else if (x < cmds->cmd_count - 1)
	{
		if (dup2(_pipe[1], STDOUT_FILENO) == -1)
			fail(0);
	}
	if (cmds->fds[x][0] != -1)
	{
			//	printf("lol1");

		if(dup2(cmds->fds[x][0], STDIN_FILENO) == -1)
			fail(0);
	}
	else if (cmds->fds[x][1] != -1)
	{
			//	printf("lol2");
		if(dup2(cmds->fds[x][1], STDIN_FILENO) == -1)
			fail(0);
	}
	else if (x > 0)
	{
		//printf("lol3");
		if (dup2(sv_next, STDIN_FILENO) == -1)
			fail(0);
		//printf("passed");
	}
	(void) cmd;

}

void restore_fds()
{
	static int fd0;
	static int fd1;
	static int set;

	if(!set)
	{
		fd0 = dup(STDIN_FILENO);
		fd1 = dup(STDOUT_FILENO);
		set = 1;
	}
	else
	{
		dup2(fd0, STDIN_FILENO);
		dup2(fd1, STDOUT_FILENO);
	}
}

char *fetch_pathll(char *data, t_node *head)
{
	while (head)
	{
		if (head->data == data && head->path)
			return (head->path);
		else if (head->data == data)
			return (data);
		head = head->next;
	}
	return (data);
}

void    execute_list(t_comparsed *cmds, t_env *env, char **original_envp, t_node *head)
{
	int	parent_id;
	int x;
	int chld_pid;
	int sv_next;
	int _pipe[2];
	// check null cmd_full

	restore_fds(); // this should be called in main instead, for first set
	// mind signals ! DEC 4
	sv_next = -1;
	x = 0;
	parent_id = getpid();
	set_fds(cmds, &cmds->fds);
	while (cmds && x < cmds->cmd_count)// ->exec_ready[x]) // don't use command count!!!, a command can be empty!!
	{
		set_redirects(cmds->real_redirects[x], cmds->fds[x]);
		apply_garbage_redir(cmds->garbage_redirects_arr[x]);
		 // don't ever dereference exec_ready[x] before checking if it isn't null
		if (cmds->exec_ready[x])
		{
			//printf("running cmd %s\n",cmds->exec_ready[x][0]);
			set_pipe(_pipe, sv_next);
			overridefds(cmds, x, _pipe, sv_next, cmds->exec_ready[x][0]);
			if (is_builtin(cmds->exec_ready[x][0]) && !det_subshell(cmds))
			{
				call_respective(cmds->exec_ready[x], &cmds->exit_status, env);
				env_toarray(env, cmds, original_envp);
			}
			else
			{
				chld_pid = fork();
				if (chld_pid == -1)
					fail(0); //<<--- reset this//reset("Issue forking child!"); //
				if (getpid() != parent_id)
				{
					if (is_builtin(cmds->exec_ready[x][0]))
						call_respective(cmds->exec_ready[x], &cmds->exit_status, env);
					else if (execve(fetch_pathll(cmds->exec_ready[x][0], head), cmds->exec_ready[x], cmds->uptodate_env) == -1)
						fail_exit(); //set the valid return value here! Dec 3
						// if execve fails in child, the child's heap must be freed! Like we have duplicate minishells!
					fail_exit();
				}
			}
		}
		restore_fds();
		sv_next = _pipe[0]; // this can happen to be garbage
		x++;
	}
	while (wait(&cmds->exit_status) != -1) // this should be at the end as well! because the signal handler might screw things up!
		;// cmd not found etc...
//	reset();
}


/*  
// The execve syscall will make a copy of the args, no need to expect crash in child process
Redirectors that are overriden output NULL to the target they locked
pipes can be overriden by following input redirects!

EVEN BUILTINS ARE RUN IN A SUBSHELL WHEN THERE ARE PIPES!

RESET THE SIGNAL HANDLERS AFTER FORK!

fork:  The child process shall have its own copy of the parent's file descriptors. 
Each of the child's file descriptors shall refer to the same open file description 
with the corresponding file descriptor of the parent. 
execve: By default, file descriptors remain open across an execve(). 

the fore of a redirector is always a file or stream

redirector only captures what's immediately after it
it applies the redirection to the command in happens to be with

Builtin commands that are invoked as part of a pipeline are also executed 
in a subshell environment. Changes made to the subshell environment cannot affect the shell’s
 execution environment. 


The current envp is inherited in the subshell!


waitpid gets the ret value of process, don't worry!

if you don't wait for processes they fill the kernel table and you won't be able to launch processes anymore in the system !

waitpid is non-blocking
*/