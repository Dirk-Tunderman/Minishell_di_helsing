#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>
#include <dirent.h>
#include <signal.h>

#define MAX_ARGS 1024
#define SYSCALLFAIL 666
#define WHOCARES 1337
#define YIELD 256
#define ABORTCURRENTCMD 555
#define ACCESS_DENIED 126

extern int g_signal_rec;

typedef struct s_data t_data;
typedef struct s_env t_env;
typedef struct s_hrd t_hrd;
typedef struct s_cmd_full t_cmd_full;
typedef struct s_command t_command;
typedef struct s_cmd t_cmd;
typedef struct s_redirect t_redirect;


 typedef enum s_token {
	OPERATOR,
	ARG,
	QUOTE_ARG,
	NONE,
	CMD,
	BUILT_IN,
	STDIN_RD,
	STDOUT_RD,
	HERDOC,
	APPEND,    
	REPROMPT,
} t_token;

typedef struct s_data
{
	t_cmd *cmd;
	char *line;
	t_env *env;
} t_data;

typedef struct s_cmd
{ 
	char *cmd;
	char **argv;
	char *opt;
	int x;
	t_hrd *hrd;
	struct s_cmd *next;
} t_cmd;

typedef struct s_command {
	char			**single_cmd;
	unsigned int	num_of_arguments;
	t_redirect		*redirect_arr;
	int				redirect_count;
}	t_command;

typedef struct s_cmd_full
{
	t_command *cmd_arr;
	int cmd_count;
	int exit_status;
	t_env *env;
} t_cmd_full;

typedef struct s_env
{
	char *key;
	char *value;
	int		only_export;
	struct s_env *next;
} t_env;

typedef struct s_node {
	char *data;
	t_token type;
	char *before_expansion;
	char *before_env;
	char	*exp_nosplit;
	int redirect;
	int space_after;
	int flag;
	char *path;
	struct s_node	*next;
	struct s_node *previous; 
} t_node;

typedef struct s_excute {
	char *line;
	char **cmd;
	char **RD;
	struct s_excute *next;

} t_excute;


typedef struct s_lexi
{
	char			*str;
	int				type;
	int				quotes;
	struct s_lexi	*next;
}		t_lexi;


typedef struct s_hrd
{
	char *cmd;
	char **f_name;
	int x;
} t_hrd;

//Environment
char get_env_total();

//Tokenisation
void lexer(char *input, t_node **head, t_env *l_env);
void process_token(char *input, int *i, t_token *prev_type, t_node **head, t_env *l_env);
char **get_quoted_word(char *input, int *i, t_env *l_env, char quote_type, int different);
char *remove_quotes(char *input);
char *get_word(char *input, int *i);
char *get_before_env_varr(char *input, int temp);

char *get_before_env_var(char *input, int end, int start);

char *find_env_var(const char *var_name, t_env *l_env);

t_node *create_node(char *str, t_token type);
int	add_token_list(t_node **head, char *input);

int is_shell_command(char *str);
void append_node(t_node **head, char *data, t_token type, int space_flag, char *before_env);
char *get_env_var(char *input, int *i, t_env *l_env);
char *get_env_var_ex(char *input, int i);
char *expand_env_in_word(const char *word, t_env *l_env, bool is_single_quoted);

//Error handling
int error_all_check(t_node *head);
int check_operator_errors(t_node *head);
int check_operator_start_end(t_node *current, t_node *previous);
int check_operator_no_cmd(t_node *current, t_node *previous, t_node *next);
int check_redirect_pipe_error(t_node *current, t_node *previous);
int check_redirection_errors(t_node *head);
int check_filename_errors(t_node *head);
int check_pipe_errors(t_node *head);
int check_invalid_chars(char *data, char *invalid_chars);
int	checker_quotes(char *line);

// error_nodes
void  set_redirect_in_nodes(t_node *head);
int check_redirect(t_node *head);
int check_redirect_pipe(t_node *head);

void display_excute_list(t_excute *head);
int	set_and_check_redirect(t_node *head);

//Resolution
char *find_executable(char *command,t_env *l_env);
void resolve_path(t_node *head, t_env *l_env);
void resolve_commands(t_node *head, t_env *l_env);

// Utils
int ft_isspace(int c);
int is_operator(char c);
char *char_to_str(char c);
int ft_isalnum(char c);
t_node *get_last_node(t_node **head);
void add_node(t_node **head, char *data, t_token type);
int check_if_present(char c, char *str);
int incl_char(char c, char *str);
char *ft_substr(char *s, int start, int len);
char *del_substr(char *input, int start, int len);
char *ft_strjoin(char const *s1, char const *s2);
char *ft_strndup(const char *str, size_t n);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
char *ft_strcpy(char *dest, const char *src);
char *ft_strcat(char *dest, const char *src);
char *ft_strtok(char *str, const char *delim);
char *ft_strncpy(char *dest, const char *src, size_t n) ;

//Signals
void    sig_handler(int signo);
void	sig_init(void);
void sig_reset();

// utils_2
int ft_strlen(const char *s);
char *ft_strchr(const char *s, int c);
int ft_strcmp(const char *str1, const char *str2);
char	*m_ft_strdup(const char *s1);
char	*ft_strdup(const char *s1);
char *ft_strtok_c(char *str, char delim) ;

// utils_3
int checker_quotes(char *line);
void display_list(t_node *head);
char	**ft_split(char const *s);

//Env_to_list
void split_env_var(char *env_var, char **key, char **value);
t_env *new_env_node(char *env_var);
void append_env_node(t_env **head, char *env_var);
t_env *envp_to_linked_list(char **envp);

typedef struct s_quickfix
{
	void	*allocation;
	int		mode;
}	t_quickfix;

typedef struct s_cd
{
    char *cwd;
    char *prevwd;
} t_cd;

typedef struct s_alloc
{
    void *allocation;
    struct s_alloc *next; 
}   t_alloc;

typedef struct s_opens
{
    int fd;
    struct s_opens *next;
}   t_opens;

// real [STDIN, HERDOC, STDOUT, APPEND] 
// fake [[OVERRIDE, OVERRIDE, 0], [NOP, NOP, 0] [HEREDOC, HEREDOC, 0]]
// fds [STDIN_FD <, HERDOC_RD, HERDOC_WR, STDOUT_FD >, APPEND_FD >>]

typedef struct s_comparsed
{
    char ***exec_ready;
    char ***real_redirects;
    char ****garbage_redirects_arr;
    int     **fds; 
    int     cmd_count;
	t_node	*all_thestuff;
	t_env	*environment;
    char    **uptodate_env;
    int     exit_status;
	char	**heredocs;
	char 	**fake_heredocs;
} t_comparsed;

void *alloc_wrapper(void *allocation, int mode, void *additional);

#define	FAIL	fail(0, printf("File %s:\nLine %d: Function %s\n", __FILE__, __LINE__, __func__));
#define OUT_OF_MEMORY 255

void	*ft_calloc(size_t count, size_t size);
void call_respective(char **cmd, int *exit_stat, t_env **env);
int execute_list(t_comparsed *cmds, t_env **env, char **original_envp, t_node *head);
int     check_buildin(char *cmd);
int set_fds(t_comparsed *cmds, int ***fds);
int set_redirects(char **redirects, int *fds);
t_alloc	*ft_lstnew(void *content);
void	ft_lstadd_back(t_alloc **lst, t_alloc *new);
t_alloc	*ft_lstlast(t_alloc *lst);
t_opens	*ft_lstlast_fd(t_opens *lst);
t_opens	*ft_lstnew_fd(int content);
void	ft_lstadd_back_fd(t_opens **lst, t_opens *new);

// final parse
t_comparsed *parsed_single_cmd(t_node *linked_list, int cmd_count, int exit_stat, char **envp);
char **env_toarray(t_env *env);
unsigned char _ft_strcmp(char *str1, char *str2);
char *skip_expand_split(t_node **linked_list);
int is_in(char *cmd, char c);

// to be used with append_env_node!
int exit_status(int value);
void *fail(void *ret, int smth);
void restore_fds();
void fail_exit();
void save_alloc(void *sv);
void *alloc_wrap(void *arg);
t_env *duplicate_env(t_env *env);

// builtins
int unset(char **args, t_env **env, int *exit_stat);
int export(char **args, t_env **env, int *exit_stat);
int print_wd(char **args, t_env *env, int *exit_stat);
int echo(char **args, t_env *env, int *exit_stat);
int change_dir(char **args, t_env *env, int *exit_stat);
void print_env(char **args, t_env *head, int *exit_stat); 
void    c_exit(char **args, int *exit_status);
int main_loop(t_node *head, char **envpp);
int exit_status(int value);


int	ft_atoi(const char *str);
char	*ft_itoa(int n);
void print_env_list(t_env *head);
void	ft_bzero(void *s, size_t n);
int	is_int0(char *arg);

#endif