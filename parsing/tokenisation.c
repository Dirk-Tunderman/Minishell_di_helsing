#include "../minishell.h"

void lexer(char *input, t_node **head, t_env *l_env)
{
	int i = 0;
	t_token prev_type = OPERATOR;
	while (input[i])
	{
		if (ft_isspace(input[i]))
			i++;
		else 
			process_token(input, &i, &prev_type, head, l_env);
	}
}

void minishell_no_alzheimer(t_node *head, char *data)
{
	while(head->next)
		head = head->next;
	head->exp_nosplit = data;
}

void process_token(char *input, int *i, t_token *prev_type, t_node **head, t_env *l_env)
{
	char 	*data;
	char	**quoted;
	char	**split_env;
	int 		x;
	t_token type;
	int space_flag = 0;
	char *before_env;
	int temp = 0;

	before_env = NULL;
	if (input[*i] == '$')
	{	
		temp = *i;
		before_env = get_env_var_ex(input, *i);
		data = get_env_var(input, i, l_env);
		space_flag = ft_isspace(input[*i]);
		type = ARG;
	}
	else if (is_operator(input[*i]))
	{
		printf("go in operator\n");
		if (input[*i] == '>' && input[*i + 1] == '>')
		{
			data = ft_strdup(">>");
			type = APPEND;
			(*i)++;
		}
		else if (input[*i] == '<' && input[*i + 1] == '<')
		{
			data = ft_strdup("<<");
			type = HERDOC;
			(*i)++;
		}
		else
		{
			if (input[*i] == '<')
				type = STDIN_RD;
			else if (input[*i] == '>')
				type = STDOUT_RD;
			else
				type = OPERATOR;
			data = char_to_str(input[*i]);
		}
		(*i)++; 
		space_flag = ft_isspace(input[*i]);
	}
	else if (input[*i] == '\"')
	{
		quoted = get_quoted_word(input, i, l_env, '\"', 0);
		data = quoted[1];
		before_env = quoted[0];
		if (input[*i])
			(*i)++;
		space_flag = ft_isspace(input[*i]);
		type = QUOTE_ARG;
	}
	else if (input[*i] == '\'')
	{
		quoted = get_quoted_word(input, i, l_env, '\'', 0);
		data = quoted[1];
		before_env = quoted[0];
		if (input[*i])
			(*i)++;
		space_flag = ft_isspace(input[*i]);
		type = QUOTE_ARG;
	}
	else
	{
		data = get_word(input, i);
		space_flag = ft_isspace(input[*i]);
		type = ARG;
	}
	if (type == ARG && before_env)
	{
		split_env = ft_split(data);
		x = 0;
		while (split_env[x])
		{
			if (!split_env[x+1])
				append_node(head, split_env[x], type, space_flag, before_env);
			else
				append_node(head, split_env[x], type, 2, before_env);
			minishell_no_alzheimer(*head, data);
			x++;
		}
		if (!split_env[0])
		{
			append_node(head, split_env[0], type, space_flag, before_env);
			minishell_no_alzheimer(*head, data);
		}
		*prev_type = type;
		return;
	}
	append_node(head, data, type, space_flag, before_env);
	*prev_type = type;
}

char **get_quoted_word(char *input, int *i, t_env *l_env, char quote_type, int different)
{
    char **result;
	char *env_variable;
	char *before_env;

	(*i)++;
	int start = *i;
	result = ft_calloc(sizeof(char *), 2);
	result[0] = ft_calloc(sizeof(char), 1);
	result[1] = ft_calloc(sizeof(char), 1);
	while(input[*i] && (input[*i] != quote_type || different))
    {
		if ((quote_type == '\"' || different) && input[*i] == '$')
		{
			before_env = get_env_var_ex(input, *i);
        	env_variable = get_env_var(input, i, l_env);
			result[0] = ft_strjoin(result[0], before_env);
			result[1] = ft_strjoin(result[1], env_variable);
			start = *i;
		}
		else
		{
			result[0] = ft_strjoin(result[0], ft_strndup(&input[*i], 1));
			result[1] = ft_strjoin(result[1], ft_strndup(&input[*i], 1));
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
	word = ft_strndup(&input[start], *i - start);
	return word;
}

void append_node(t_node **head, char *data, t_token type, int space_flag, char *before_env)
{
	t_node *new_node = ft_calloc(sizeof(t_node), 1);
	new_node->data = ft_strdup(data);
	if (before_env)
		new_node->before_env = ft_strdup(before_env);
	else
		new_node->before_env = NULL;
	new_node->type = type;
	new_node->path = NULL;
	new_node->space_after = space_flag;
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
	while(input[i] && !ft_isspace(input[i]) && !is_operator(input[i]) && input[i] != '\"' && input[i] != '\'' && input[i] != '$')
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
	start = *i;
	if (input[*i] == '?')
	{
		(*i)++;
		non_delim_index = *i;
		while(input[*i] && !ft_isspace(input[*i]) && !is_operator(input[*i]) && input[*i] != '\"' && input[*i] != '\'' && input[*i] != '$')
			(*i)++;
		end = *i;
		delim = ft_substr(&input[non_delim_index], 0, end - non_delim_index);
		var_value = ft_itoa(exit_status(YIELD));
	}
	else
	{
		while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_' ))
			(*i)++;
		non_delim_index = *i;
		while(input[*i] && !ft_isspace(input[*i]) && !is_operator(input[*i]) && input[*i] != '\"' && input[*i] != '\'' && input[*i] != '$')
			(*i)++;
		end = *i; 
		if (end == start - dollar)
			return (ft_strndup(&input[start - dollar - 1], dollar + 1));
		delim = ft_substr(&input[non_delim_index], 0, end - non_delim_index);
		var_name = ft_strndup(&input[start], non_delim_index - start);
		var_value = find_env_var(var_name, l_env);
	}

	if (var_value == NULL)
	{
		if (non_delim_index - start)
		{
			if ((!end - non_delim_index))
				return (ft_strndup(&input[start - dollar], dollar));
			else
				return (ft_strjoin(ft_strndup(&input[start - dollar], dollar), delim));
		}
		else
			return (ft_strjoin(ft_strndup(&input[start - dollar - 1], dollar + 1), delim));
	}
	result = ft_calloc(sizeof(char), ft_strlen(var_value) + 1 + ft_strlen(delim) + dollar);
	ft_strcat(result, ft_strndup(&input[start - dollar], dollar));
	ft_strcat(result, var_value);
	ft_strcat(result, delim);
	return (result);
}

char *find_env_var(const char *var_name, t_env *l_env)
{
    size_t len = ft_strlen(var_name);
    t_env *current = l_env;

	while (current) {
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
