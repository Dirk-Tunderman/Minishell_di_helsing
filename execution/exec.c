/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 13:48:06 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/14 20:59:17 by eamrati          ###   ########.fr       */
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

int inc_val(t_env *env, char *arg)
{
	char *key;
	char *value;

	split_env_var(arg, &key, &value); // check for malloc fail
	//(free(key), -1); // NOT VALUE THEN add key, but do not append it to env! (maybe using a flag or smth)
	while (env && _ft_strcmp(env->key, key))
		env = env->next;
	if (env)
	{
	//	free(env->value); This will be garbage collected later!!!
		env->value = env->value + 1;
	}
	else
		return (0);//(free(value), free(key), 0);
	return (1);//(free(key), 1);
}

int is_builtin(char *cmd)
{
	if (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "pwd") 
	|| !ft_strcmp(cmd, "export")  || !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env")
	|| !ft_strcmp(cmd, "exit"))
		return (1);
	return (0);
}

int apply_garbage_redir(char ***garbage_redirects_arr)
{
	int x;
	int fd;

	x = 0;
	while (garbage_redirects_arr[0][x]) // OVERRIDE
	{
		fd = open(garbage_redirects_arr[0][x], O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
		if (fd == -1)
		{
			printf("Error opening file %s\n", garbage_redirects_arr[0][x]);
			return (ABORTCURRENTCMD);
		}
		else
			close(fd);
		x++;
	}
	x = 0;
	while (garbage_redirects_arr[1][x]) // APPEND
	{
		fd = open(garbage_redirects_arr[1][x], O_WRONLY | O_CREAT, S_IRWXU);
		if (fd == -1)
		{
			printf("Error opening file %s\n", garbage_redirects_arr[1][x]);
			return (ABORTCURRENTCMD);
		}
		else
			close(fd);
		x++;
	}
	x = 0;
	while (garbage_redirects_arr[2][x]) // HEREDOC
	{
		fd = open(garbage_redirects_arr[2][x], O_RDONLY, S_IRWXU);
		if (fd == -1)
		{
			printf("Error opening file %s\n", garbage_redirects_arr[2][x]);
			return (ABORTCURRENTCMD);
		}
		else
			close(fd);
		x++;
	}
	return (0);
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

int set_pipe(int *_pipe, int x, int cmd_count)
{
// if sv_next is set, it is meat for you, use it and set it again if there is someone following you
	//if (sv_next != -1)
	//{
	//	printf("%d\n", _pipe[1]);
	//	close(_pipe[1]);
	//}
	if (cmd_count > 1 && x < cmd_count - 1 && pipe(_pipe))
		return (SYSCALLFAIL);
	return (0);
}

// pipes, just take the endpoint you need and forget the other
// redirectors, you have to dup the respective endpoint

int overridefds(t_comparsed *cmds, int x, int *_pipe, int sv_next)
{
// STDOUT
	int flag;

	flag = 0;
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
			return (close(_pipe[0]), SYSCALLFAIL);
		close(_pipe[0]);
	}
// STDIN
	if (cmds->fds[x][0] != -1)
	{
		//if ()
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
		if (sv_next == -2)
			close(STDIN_FILENO); // DEC 11 test this later
		else
		{
			if (dup2(sv_next, STDIN_FILENO) == -1)
				return (close(_pipe[0]), SYSCALLFAIL); // this should be different for child process exit!!!!
			close(_pipe[0]);
		}
	}
	//else
	//{
	//	if (dup2(dup(STDIN_FILENO), STDIN_FILENO) == -1)
	//		return (SYSCALLFAIL);		
	//}
	if (cmds->fds[x][3] == -1 && cmds->real_redirects[x][2])
	{
		close(STDOUT_FILENO);
	}
	if (cmds->fds[x][4] == -1 && cmds->real_redirects[x][3])
	{
		close(STDOUT_FILENO);
	}
	// cmds->fds[x][1] == -1 && cmds->real_redirects[x][1]
	if (cmds->fds[x][0] == -1 && cmds->real_redirects[x][0]) // unsuccessful opens
	{	
		close(STDIN_FILENO);
	}
	return (0);
}

void restore_fds(int isclose)
{
	static int fd0;
	static int fd1;
	static int set;
	
	if (isclose)
	{
		close(fd0);
		close(fd1);
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		return ;
	}
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


void close_fds(int *fds)
{
	if (fds[0] != -1)
		close(fds[0]);
	if (fds[1] != -1)
		close(fds[1]);
	if (fds[3] != -1)
		close(fds[3]);
	if (fds[4] != -1)
		close(fds[4]);
}

int is_in(char *cmd, char c)
{
	int x;
	
	x = 0;
	while (cmd && cmd[x])
	{
		if (cmd[x] == c)
			return (1);
		x++;
	}
	return (0);
}

// https://unix.stackexchange.com/questions/673855/i-dont-understand-cat-behaviour-when-running-cat-ls
int execute_list(t_comparsed *cmds, t_env **env, char **original_envp, t_node *head)
{
	(void) head;
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
	(_pipe[0] = -1) && (_pipe[1] = -1);
	if (set_fds(cmds, &cmds->fds) == SYSCALLFAIL)
		return (signal(SIGINT, SIG_IGN), restore_fds(0), SYSCALLFAIL);
	cmds->exit_status = exit_status(YIELD);
	signal(SIGINT, SIG_IGN);
	restore_fds(0);
	while (x < cmds->cmd_count)
	{
		if (cmds->exec_ready[x])
			last_proc = cmds->exec_ready[x];
		x++;
	}
	x = 0;
	while (cmds && x < cmds->cmd_count) // ->exec_ready[x]) // don't use command count!!!, a command can be empty!!
	{
		if (apply_garbage_redir(cmds->garbage_redirects_arr[x]) == ABORTCURRENTCMD)
		{
			close_fds(cmds->fds[x]); // DEC 11, CHECK THIS !!!
			close(_pipe[0]);
			close(_pipe[1]);
			sv_next = -2;
		}
		else
		{
			if (set_redirects(cmds->real_redirects[x], cmds->fds[x]) == ABORTCURRENTCMD)
			{
			//	close_fds(cmds->fds[x]); // dec 13
				close(_pipe[0]); // check this later, added with Hamza
				close(_pipe[1]);
				sv_next = -2;
			}
			else if (cmds->exec_ready[x] && cmds->exec_ready[x][0])
			{
				// setup of program ends here
				if (is_builtin(cmds->exec_ready[x][0]) && !det_subshell(cmds))
				{
					if (overridefds(cmds, x, _pipe, sv_next) == SYSCALLFAIL)
						return (close_fds(cmds->fds[x]), close(_pipe[0]), close(_pipe[1]), SYSCALLFAIL);
					call_respective(cmds->exec_ready[x], &cmds->exit_status, env);
				}
				else
				{
					if(set_pipe(_pipe, x, cmds->cmd_count) == SYSCALLFAIL)
						return (close_fds(cmds->fds[x]), close(_pipe[0]), close(_pipe[1]), SYSCALLFAIL);
					cmds->uptodate_env = env_toarray(*env);
					chld_pid = fork();
					if (chld_pid == -1)
						return (SYSCALLFAIL);	//<<--- reset this//reset("Issue forking child!"); //
					if (cmds->exec_ready[x] == last_proc)
						pid_last = chld_pid;
					if (!chld_pid)
					{
						inc_val(*env, "SHLVL=whocares");
						if (overridefds(cmds, x, _pipe, sv_next) == SYSCALLFAIL)
						{
							exit_status(SYSCALLFAIL);
							fail_exit();
						}
						//close(STDERR_FILENO);
						signal(SIGINT, SIG_DFL);
						signal(SIGQUIT, SIG_DFL);
					//	write(2, cmds->exec_ready[x][0], 2);
					//	write(2, cmds->exec_ready[x][1], 2);
						if (is_builtin(cmds->exec_ready[x][0]))
						{
							call_respective(cmds->exec_ready[x], &cmds->exit_status, env);
							exit_status(cmds->exit_status);
							fail_exit();
						}
						else if (find_executable(cmds->exec_ready[x][0], *env))
						{
						 	if (execve(find_executable(cmds->exec_ready[x][0], *env), cmds->exec_ready[x], cmds->uptodate_env) == -1)
							{
								exit_status(ACCESS_DENIED);
								write(2, "Permission issue.\n", ft_strlen("Permission issue.\n"));
								fail_exit();
							}
						}
						else if(is_in(cmds->exec_ready[x][0], '/') && execve(cmds->exec_ready[x][0], cmds->exec_ready[x], cmds->uptodate_env) == -1)
						{ 
							if ( // this is requisite because without /, no lookup is done. All commands must have path specifiers to be executed. Access checks current path and can mess things up
								!access(cmds->exec_ready[x][0], F_OK) && access(cmds->exec_ready[x][0], X_OK))
							{
								exit_status(ACCESS_DENIED);
								write(2, "Permission issue.\n", ft_strlen("Permission issue.\n"));
							}
							else
							{
								exit_status(127);
       		 					write(2, "Command not found.\n", ft_strlen("Command not found.\n"));
							}
							fail_exit();
						}
						exit_status(127);
       		 			write(2, "Command not found.\n", ft_strlen("Command not found.\n"));
						fail_exit();
					}
				}
				if (sv_next != -1)
					close(sv_next);
				close(_pipe[1]);
				sv_next = _pipe[0]; 
			}
			close_fds(cmds->fds[x]);
		}
		restore_fds(0);
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
		///// raises the result of what was just evaluated
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