/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 13:48:06 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/09 16:24:51 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_calloc(size_t count, size_t size)
{
	unsigned long long		a;
	char					*ptr;

	a = 0;
	ptr = alloc_wrap(malloc(count * size));
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

int set_pipe(int *_pipe, int sv_next)
{
// if sv_next is set, it is meat for you, use it and set it again if there is someone following you
	if (sv_next != -1)
		close(_pipe[1]);
	if (pipe(_pipe))
		return (SYSCALLFAIL);
	return (0);
}

// pipes, just take the endpoint you need and forget the other
// redirectors, you have to dup the respective endpoint

int overridefds(t_comparsed *cmds, int x, int *_pipe, int sv_next)
{
// STDOUT
	if (cmds->fds[x][3] != -1)
	{
		if(dup2(cmds->fds[x][3], STDOUT_FILENO) == -1)
			return (SYSCALLFAIL);// reset instead
	}
	else if (cmds->fds[x][4] != -1)
	{
		if (dup2(cmds->fds[x][4], STDOUT_FILENO) == -1)
			return (SYSCALLFAIL);
	}
	else if (x < cmds->cmd_count - 1)
	{
		if (dup2(_pipe[1], STDOUT_FILENO) == -1)
			return (SYSCALLFAIL);
	}
// STDIN
	if (cmds->fds[x][0] != -1)
	{
		if(dup2(cmds->fds[x][0], STDIN_FILENO) == -1)
			return (SYSCALLFAIL);
	}
	else if (cmds->fds[x][1] != -1)
	{
		if(dup2(cmds->fds[x][1], STDIN_FILENO) == -1)
			return (SYSCALLFAIL);
	}
	else if (x > 0)
	{
		if (dup2(sv_next, STDIN_FILENO) == -1)
			return (SYSCALLFAIL);
	}
	if (cmds->fds[x][3] == -1 && cmds->real_redirects[x][2])
		close(STDOUT_FILENO);
	if (cmds->fds[x][4] == -1 && cmds->real_redirects[x][3])
		close(STDOUT_FILENO);
	if (cmds->fds[x][0] == -1 && cmds->real_redirects[x][0])
		close(STDIN_FILENO);
	return (0);
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
		if (!ft_strcmp(head->data, data) && head->path)
			return (head->path);
		else if (!ft_strcmp(head->data, data))
			return (data);
		head = head->next;
	}
	return (data);
}

void _void()
{
	return ;
}

int execute_list(t_comparsed *cmds, t_env *env, char **original_envp, t_node *head)
{
	int x;
	int pid_wait;
	int pid_last;
	char **last_proc;
	int chld_pid;
	int sv_next;
	int _pipe[2];
	(void) original_envp;
	// check null cmd_full
	 // this should be called in main instead, for first set
	// mind signals ! DEC 4
	sv_next = -1;
	x = 0;
	last_proc = 0;
	pid_last = -1;
	if (set_fds(cmds, &cmds->fds) == SYSCALLFAIL)
		return (signal(SIGINT, SIG_IGN), restore_fds(), SYSCALLFAIL);
	signal(SIGINT, SIG_IGN);
	restore_fds();
	while (x < cmds->cmd_count)
	{
		if (cmds->exec_ready[x])
			last_proc = cmds->exec_ready[x];
		x++;
	}
	x = 0;
	while (cmds && x < cmds->cmd_count) // ->exec_ready[x]) // don't use command count!!!, a command can be empty!!
	{
		set_redirects(cmds->real_redirects[x], cmds->fds[x]);
		apply_garbage_redir(cmds->garbage_redirects_arr[x]); // who cares syscallfail
		 // don't ever dereference exec_ready[x] before checking if it isn't null
		if (cmds->exec_ready[x])
		{
			//printf("running cmd %s\n",cmds->exec_ready[x][0]);
			if(set_pipe(_pipe, sv_next) == SYSCALLFAIL)
				return (SYSCALLFAIL);
			if (overridefds(cmds, x, _pipe, sv_next) == SYSCALLFAIL)
				return (SYSCALLFAIL);
			// setup of program ends here
			if (is_builtin(cmds->exec_ready[x][0]) && !det_subshell(cmds))
				call_respective(cmds->exec_ready[x], &cmds->exit_status, env);
			else
			{
				cmds->uptodate_env = env_toarray(env);
				chld_pid = fork();
				if (chld_pid == -1)
					return (SYSCALLFAIL);	//<<--- reset this//reset("Issue forking child!"); //
				if (cmds->exec_ready[x] == last_proc)
					pid_last = chld_pid;
				if (!chld_pid)
				{
					close(STDERR_FILENO);
					signal(SIGINT, SIG_DFL);
					signal(SIGQUIT, SIG_DFL);
					if (is_builtin(cmds->exec_ready[x][0]))
					{
						call_respective(cmds->exec_ready[x], &cmds->exit_status, env);
						exit_status(cmds->exit_status);
						fail_exit();
					}
					else if (execve(fetch_pathll(cmds->exec_ready[x][0], head), cmds->exec_ready[x], cmds->uptodate_env) == -1)
					{
						exit_status(127);
						if (!access(fetch_pathll(cmds->exec_ready[x][0], head), F_OK))
						{
							exit_status(126);
							perror(0);
							fail_exit();
						}
						perror(0);
						fail_exit();
					}
				}					
			}
			//printf("RAN %s %d\n", cmds->exec_ready[x][0], cmds->exit_status);
		}
		restore_fds();
		sv_next = _pipe[0]; // this can happen to be garbage
		x++;
	}
	pid_wait = waitpid(-1, &cmds->exit_status, 0);
	while (pid_wait > 0)
	{
		if(last_proc && pid_last == pid_wait && WIFEXITED(cmds->exit_status))
			cmds->exit_status = WEXITSTATUS(cmds->exit_status);
		else if (last_proc && pid_last == pid_wait && WIFSIGNALED(cmds->exit_status))
			cmds->exit_status = 128 + WTERMSIG(cmds->exit_status);
		// this should be at the end as well! because the signal handler might screw things up!
		if (last_proc && pid_last == pid_wait && cmds->exit_status > 255)
			cmds->exit_status = 255;
		pid_wait = waitpid(-1, &cmds->exit_status, 0);
	}
	return (0);
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