#include "../minishell.h"

void lexer(char *input, t_node **head, t_env *l_env)
{
	int i = 0;
	t_token prev_type = OPERATOR;  // Initialize as OPERATOR
	while (input[i])
	{
		if (ft_isspace(input[i])) // skip spaces
			i++;
		else 
		{
			process_token(input, &i, &prev_type, head, l_env); // Pass envp to process_token
			check_and_set_redirect(*head);
		}
	}
}

int spaceflag_d_quote(char *input, int *i)
{
	int temp = *i;
	int space_flag = 0;
	temp++;
	while (input[temp] && input[temp] != '\"')
		temp++;
	if (input[temp + 1] == ' ')
		space_flag = 1;
	printf("space_flag: %d\n", space_flag);
	return (space_flag);
}

int	spaceflag_word(char *input, int *i)
{
	int temp = *i;
	int space_flag = 0;
	temp++;
	while (input[temp] && input[temp] != '\"' && input[temp] != '\'')
		temp++;
	if (input[temp + 1] == ' ')
		space_flag = 1;
	printf("space_flag: %d\n", space_flag);
	return (space_flag);
}

int space_flag_s_quote(char *input, int *i)
{
	int temp = *i;
	int space_flag = 0;
	temp++;
	while (input[temp] && input[temp] != '\'')
		temp++;
	if (input[temp + 1] == ' ')
		space_flag = 1;
	printf("space_flag: %d\n", space_flag);
	return (space_flag);
}

int	spaceflag_env(char *input, int *i)
{
	int temp = *i;
	int space_flag = 0;

	temp++;
	while (input[temp] && input[temp] != '\'' && input[temp] != '\"' && input[temp] != '$')
		temp++;
	if (input[temp + 1] == ' ')
		space_flag = 1;
	printf("space_flag: %d\n", space_flag);
	return (space_flag);
}

int	spaceflag_op(char *input, int *i)
{
	int temp = *i;
	int space_flag = 0;
	//temp++;
	printf("input %s\n", input);
	printf("temp: %d\n", temp);
	printf("before while sp_op");
	while (input[temp] == '<' || input[temp] == '>' || input[temp] == '|')
		temp++;
	printf("after while sp_op");
	if (input[temp] == ' ')
		space_flag = 1;
	printf("space_flag: %d\n", space_flag);
	return (space_flag);
}

int	check_redirect_before_$(char *input, int *i)
{
	int temp = *i;
	int flag = 0;

	while (input[temp] == ' ')
		temp--;
	while (input[temp] && input[temp] != '$')
	{
		if (input[temp] == '>' || input[temp] == '<')
			flag = 1;
		temp--;
	}
	return (flag);
}

void process_token(char *input, int *i, t_token *prev_type, t_node **head, t_env *l_env)
{
	char *data;
	char	**quoted;
	t_token type;
	int space_flag = 0;
	char *before_env;
	int temp = 0;

	before_env = NULL;
	if (input[*i] == '$') //for env without quotes
	{	
		temp = *i;
		before_env = get_env_var_ex(input, *i); // won't need this but good!
		data = get_env_var(input, i, l_env); // Get environment variable
		space_flag = ft_isspace(input[*i]);
		type = ARG;
	}
	else if (is_operator(input[*i])) // Check for operator
	{
		printf("go in operator\n");
		if (input[*i] == '>' && input[*i + 1] == '>')
		{
			data = ft_strdup(">>");
			(*i)++; // Increment i to skip the next '>'
		}
		else if (input[*i] == '<' && input[*i + 1] == '<')
		{
			data = ft_strdup("<<");
			(*i)++; // Increment i to skip the next '<'
		}
		else
			data = char_to_str(input[*i]); // Convert char to string
		(*i)++; // Increment i to skip the operator character
		space_flag = ft_isspace(input[*i]);
		type = OPERATOR;
	}
	else if (input[*i] == '\"') // Check for double quotes
	{
		quoted = get_quoted_word(input, i, l_env, '\"'); // Get quoted word
		data = quoted[1];
		before_env = quoted[0];
		(*i)++; // Increment i to skip the closing double quote (if present
		space_flag = ft_isspace(input[*i]);
		type = QUOTE_ARG;
	}
	else if (input[*i] == '\'') // Check for single quotes
	{
		quoted = get_quoted_word(input, i, l_env, '\''); // Get quoted word
		data = quoted[1];
		before_env = quoted[0];
		(*i)++;;
		space_flag = ft_isspace(input[*i]);
		type = QUOTE_ARG;
	}
	else
	{
		data = get_word(input, i); // Get word
		space_flag = ft_isspace(input[*i]);
		type = ARG;
	}
	append_node(head, data, type, space_flag, before_env); // Append node to list
	*prev_type = type;
}

char **get_quoted_word(char *input, int *i, t_env *l_env, char quote_type)
{
    char **result;
	char *env_variable;
	char *before_env;

	(*i)++;
	int start = *i;
	result = ft_calloc(sizeof(char *), 2);
	result[0] = ft_calloc(sizeof(char), 1);
	result[1] = ft_calloc(sizeof(char), 1);
	while(input[*i] && input[*i] != quote_type)
    {
		if (quote_type == '\"' && input[*i] == '$')// issue
		{
			before_env = get_env_var_ex(input, *i);
        	env_variable = get_env_var(input, i, l_env);
			result[0] = ft_strjoin(result[0], before_env);
			result[1] = ft_strjoin(result[1], env_variable);
			start = *i;
		}
		else
		{
			result[0] = ft_strjoin(result[0], ft_strndup(&input[*i], 1)); // LEAKS
			result[1] = ft_strjoin(result[1], ft_strndup(&input[*i], 1)); // LEAKs
			(*i)++;
		}
    }
    return (result);
}

char *get_word(char *input, int *i)
{
	int start;
	char *word;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i]) && !is_operator(input[*i]) && input[*i] != '\"' && input[*i] != '\'' && input[*i] != '$')
		(*i)++;
// to be rechecked for concurrent quotes with words

	word = ft_strndup(&input[start], *i - start);
	return word;
}

int	check_end_token_quote(char *input, char character, int start)
{
	int	len;

	len = 0;
	do
		len++;
	while (input[start + len] && input[start + len] != character);
	len++;
	return (len);
}

int	check_end_token_alnum(char *input, int start)
{
	int	len;

	len = 0;
	while (input[start + len] && (ft_isalnum(input[start + len]) 
			|| incl_char(input[start + len], ".$-/_")))
		len++;
	return (len);
}

int	check_end_token(char *input, char character, int start) 
{
	if (check_if_present(character, ">|<"))
		return (1);
	else if (ft_isalnum(character) || incl_char(character, ".$-/_"))
		return (check_end_token_alnum(input, start));
	else if (check_if_present(character, "\'\""))
		return (check_end_token_quote(input, character, start));
	return (0);
}

int is_shell_command(char *str)
{
	char *commands[] = {"echo", "pwd", "export", "cd", "unset", "env", "exit", NULL};
	int i;

	i = 0;
	while (commands[i] != NULL) 
	{
		if (ft_strcmp(str, commands[i]) == 0) 
			return (1);
		i++;
	}
	return (0);
}

void append_node(t_node **head, char *data, t_token type, int space_flag, char *before_env)
{
	t_node *new_node = ft_calloc(sizeof(t_node), 1);
	if (!new_node)
	{
		printf("Error: Memory allocation failed.\n");
		exit(EXIT_FAILURE);
	}
	new_node->data = ft_strdup(data);
	if (before_env)
		new_node->before_env = ft_strdup(before_env);
	else
		new_node->before_env = NULL;
	printf("new_node->before_env: %s\n", new_node->before_env);
	new_node->type = type;
	new_node->path = NULL;
	new_node->space_after = space_flag;

	new_node->next = NULL;
	if (*head == NULL)
		*head = new_node;
	else
	{
		t_node *current = *head;
		while (current->next != NULL)
			current = current->next;
		current->next = new_node;
	}
}

char *get_env_var_ex(char *input, int i)
{   
	int start;
	char *var_name;
	int dollar;

	dollar = 0;
	start = i;
	while (input[i] == '$')
	{
		i++;
		dollar++;
	}
	while(input[i] && !ft_isspace(input[i]) && !is_operator(input[i]) && input[i] != '\"' && input[i] != '\'' && input[i] != '$') // just tell me if there are spaces between dollars and it's fine!
		i++;
	var_name = ft_strndup(&input[start], i - start);
	return (var_name);
}

char *get_env_var(char *input, int *i, t_env *l_env)
{   
	int non_delim_index;
	int end;
	int dollar;
	int start;
	char *delim;
	char *var_name;
	char *var_value;
	char *result;
	(*i)++;
	dollar = 0;
	while (input[*i] == '$')
	{
		(*i)++;
		dollar++;
	}
//------------
	start = *i;
	// start of enviornment variable
	printf("input[*i]: %c\n", input[*i]);

	if (input[*i] == '?')
	{
		(*i)++;
		non_delim_index = *i;
		while(input[*i] && !ft_isspace(input[*i]) && !is_operator(input[*i]) && input[*i] != '\"' && input[*i] != '\'' && input[*i] != '$') // takes care of everything after $ until space or operator or "
			(*i)++;
		end = *i;
		delim = ft_substr(&input[non_delim_index], 0, end - non_delim_index);
		var_value = ft_itoa(exit_status(1001));
	}
	else
	{
		while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_' )) // takes care of everything before $ 
			(*i)++;
		printf("input[*i]: %c\n", input[*i]);
		non_delim_index = *i;
		while(input[*i] && !ft_isspace(input[*i]) && !is_operator(input[*i]) && input[*i] != '\"' && input[*i] != '\'' && input[*i] != '$') // takes care of everything after $ until space or operator or "
			(*i)++;
		end = *i; 
		if (end == start - dollar)
			return (ft_strndup(&input[start - dollar - 1], dollar + 1));
		delim = ft_substr(&input[non_delim_index], 0, end - non_delim_index);
		var_name = ft_strndup(&input[start], non_delim_index - start);
		var_value = find_env_var(var_name, l_env);
	}

	printf("var_value: %s\n", var_value);
	if (var_value == NULL)
	{
		printf("var_value == NULL\n");
		return (ft_strjoin(ft_strndup(&input[start - dollar - 1], dollar + 1), delim));
	}
	result = ft_calloc(sizeof(char), ft_strlen(var_value) + 1 + ft_strlen(delim) + dollar);
	ft_strcat(result, ft_strndup(&input[start - dollar], dollar));
	ft_strcat(result, var_value);
	ft_strcat(result, delim);
	printf("indicator 0 %s\n", result);
	return (result);
}

char *find_env_var(const char *var_name, t_env *l_env)
{
    size_t len = ft_strlen(var_name);
    t_env *current = l_env;
	//printf("var_name: %s\n", var_name);
	while (current) {
        //printf("Key: %s, Value: %s\n", current->key, current->value ? current->value : "NULL");
        current = current->next;
    }

    while (l_env)
    {
        if (ft_strncmp(l_env->key, var_name, len) == 0 && l_env->key[len] == '\0')
            return l_env->value;
        l_env = l_env->next;
    }
    return NULL;
}

void check_and_set_redirect(t_node *head) // check here also what the type is, if quote arg then dont transform into other type.
{
	t_node *current;

	current = head;
	while (current != NULL)
	{
		if ((ft_strcmp(current->data, "<") == 0) && current->type != QUOTE_ARG)
			current->type = STDIN_RD;
		else if ((ft_strcmp(current->data, ">") == 0) && current->type != QUOTE_ARG)
			current->type = STDOUT_RD;
		else if ((ft_strcmp(current->data, ">>") == 0) && current->type != QUOTE_ARG)
			current->type = APPEND;
		else if ((ft_strcmp(current->data, "<<") == 0) && current->type != QUOTE_ARG)
			current->type = HERDOC;
		current = current->next;
	}
}
