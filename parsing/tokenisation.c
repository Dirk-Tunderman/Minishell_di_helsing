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
	while (input[temp] && input[temp] != '\'' && input[temp != '\"'] && input[temp] != '$')
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

char *get_before_env_varr(char *input, int temp)
{
	char *result;
	printf("temp: %d\n", temp);
	int start = temp;
	int i = 0;
	printf("input: %s\n", input);
	while ((input[temp] != ' ') && input[temp])
		temp++;
	printf("temp: %d\n", temp);
	result = malloc(temp - start + 1);
	if (!result)
		return (NULL);

	while (start < temp)
	{
		result[i] = input[start];
		i++;
		start++;
	}
	result[i] = '\0';
	return (result);
}





void process_token(char *input, int *i, t_token *prev_type, t_node **head, t_env *l_env)
{
	char *data;
	t_token type;
	int space_flag = 0;
	char *before_env;
	int temp = 0;

	before_env = NULL;
	if (input[*i] == '$') //for env without quotes
	{	
		temp = *i;
		before_env = get_before_env_varr(input, temp); // your parsing needs to work like this eamrati
		space_flag = spaceflag_env(input, i);
		data = get_env_var(input, i, l_env); // Get environment variable
		type = ARG;
	}
	else if (is_operator(input[*i])) // Check for operator
	{
		printf("go in operator\n");
		space_flag = spaceflag_op(input, i);
		// Check if next character is also '>', indicating a '>>' operator
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
		{
			data = char_to_str(input[*i]); // Convert char to string
		}
		type = OPERATOR;
		(*i)++; // Increment i to skip the operator character
	}
	else if (input[*i] == '\"') // Check for double quotes
	{
		printf("go in double quote\n");
		space_flag = spaceflag_d_quote(input, i);
		data = get_quoted_word(input, i, l_env, '\"'); // Get quoted word
		(*i)++; // Increment i to skip the closing double quote (if present
		type = QUOTE_ARG;
		
	}
	else if (input[*i] == '\'') // Check for single quotes
	{
		space_flag = space_flag_s_quote(input, i);
		data = get_quoted_word(input, i, l_env, '\''); // Get quoted word
		type = QUOTE_ARG;
	}
	else
	{
		space_flag = spaceflag_word(input, i);
		data = get_word(input, i); // Get word
		type = ARG;
	}
	append_node(head, data, type, space_flag, before_env); // Append node to list
	*prev_type = type;
}


char *get_before_env_var(char *input, int end, int start)
{
	char *result;
	int i = 0;

	result = malloc(end - start + 1);
	while (start < end)
	{
		result[i] = input[start];
		i++;
		start++;
	}
	result[i] = '\0';
	return result;
}


char *get_quoted_word(char *input, int *i, t_env *l_env, char quote_type)
{
    char *result;
	char *env_variable;
	char *before_env;
	int flag = 0;
	int before_env_end = 0;

	(*i)++;
	printf("go in quotedword\n");
	int start = *i;
    while(input[*i] && input[*i] != quote_type)
    {
		if (quote_type == '\"' && input[*i] == '$')
		{
			before_env_end = *i;
			flag  = 1;
        	env_variable = get_env_var(input, i, l_env);
		}
		if (input[*i] == '\"')
			break;
        (*i)++;
    }
	//get string from start to before_env_start
	if (flag == 1)
	{
		before_env = get_before_env_var(input, before_env_end, start);
		result = ft_strjoin(before_env, env_variable);
		if (before_env)
			free(before_env);
	}
	else
		result = ft_strndup(&input[start], *i - start);
	printf("result: %s\n", result);
    return (result);

}


char *remove_quotes(char *input)
{
	int i;
	int j;
	int len = 0;
	char *result;
	
	result = malloc(len + 1); // Allocating memory for the result string
	len = ft_strlen(input);
	i = 0;
	j = 0;
	if (result == NULL) 
	{
		printf("Failed to allocate memory\n");
		return NULL;
	}
	while (i < len)
	{
		if(input[i] != '\"' && input[i] != '\'')
		{
			result[j] = input[i];
			j++;
		}
		i++;
	}
	result[j] = '\0'; // Don't forget to null terminate the result string
	return result;
}

char *get_word(char *input, int *i)
{
	int start;
	char *word;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i]) && !is_operator(input[*i + 1]) && input[*i] != '\"' && input[*i] != '\'' && input[*i] != '$')
	{
		if (input[*i] == '\"')
		{
			(*i)++;
			while (input[*i] != '\"')
				(*i)++;
		}
		else if (input[*i] == '\'')
		{
			(*i)++;
			while (input[*i] != '\'')
				(*i)++;
		}
		(*i)++;
	} // to be rechecked for concurrent quotes with words
	if (input[*i] == '\"')
	{
		(*i)++;
		while (input[*i] != '\"')
			(*i)++;
	}
	else if (input[*i] == '\'')
	{
		(*i)++;
		while (input[*i] != '\'')
			(*i)++;
	}
	if (input[*i] != '$')
		(*i)++;

	// Check here with Medhi if he wants the "" included in the word or not
	// if (input[*i] == '\"')
	// {
	// 	(*i)++;
	// 	while (input[*i] != '\"')
	// 			(*i)++;
	// }
	// else if (input[*i] == '\'')
	// {
	// 	(*i)++;
	// 	while (input[*i] != '\'')
	// 		(*i)++;
	// }
	// if (input[*i] == '\'' || input[*i] == '\"')
	// 	(*i)++;
	
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

t_node	*create_node(char *str, t_token type) 
{
	t_node	*new_node;

	new_node = (t_node *)malloc(sizeof(t_node));
	new_node->data = ft_strdup(str);
	new_node->type = type;
	new_node->next = NULL;
	new_node->previous = NULL;
	return (new_node);
}

int	add_token_list(t_node **head, char *input)
{
		if (!*head)
			*head = create_node(input, OPERATOR);
		else
		{
		t_node *last;
		last = get_last_node(head);
		last->next = create_node(input, OPERATOR);
		}
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
	t_node *new_node = malloc(sizeof(t_node));
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
	if (space_flag == 1)
		new_node->space_after = 1;
	else
		new_node->space_after = 0;

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

char *get_env_var(char *input, int *i, t_env *l_env)
{   
	int non_delim_index;
	int end;
	int dollar;
	int start;
	char *delim;
	char *var_name;
	char *var_value;
	//char *non_delim_chars;
	char *result;
//	int temp;
	//int temp2 = *i;
	dollar = 0;

	// temp2--;
	// while (input[temp2] == ' ' && temp2 > 0)
	// 	temp2--;

	//printf("input[temp2]: %c\n", input[temp2]);


//------------ let's see later
	(*i)++;
	while (input[*i] == '$') // takes care of all $ in a row
	{
		(*i)++;
		dollar++;
	}
//------------

	// start of enviornment variable
	start = *i;
	printf("input[*i]: %c\n", input[*i]);

	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_')) // takes care of everything before $ 
		(*i)++;
	printf("input[*i]: %c\n", input[*i]);

//----------- let's see later
	non_delim_index = *i;
	while(input[*i] && !ft_isspace(input[*i]) && !is_operator(input[*i]) && input[*i] != '\"' && input[*i] != '$') // takes care of everything after $ until space or operator or "
		(*i)++;
//-----------

	end = *i; // guaranteed anyway

	delim = ft_substr(&input[non_delim_index], 0, end - non_delim_index);
	var_name = ft_strndup(&input[start], non_delim_index - start);
	var_value = find_env_var(var_name, l_env);
	printf("var_value: %s\n", var_value);
	if (var_value == NULL)
	{
		free(var_name);
		printf("var_value == NULL\n");
		return ("");
	}
//	temp = *i;
	if (var_value && dollar % 2 == 0)
	{
	//	while (input[temp] && !ft_isspace(input[*i]) && !is_operator(input[temp]))
	//		temp++;
	//	non_delim_chars = ft_strndup(&input[non_delim_index], temp - non_delim_index);
		result = malloc(ft_strlen(var_value) + 1 + ft_strlen(delim)); // + ft_strlen(non_delim_chars)
		ft_strcpy(result, var_value);
		ft_strcat(result, delim);
		printf("indicator 0 %s\n", result);
		return (result);//(free(non_delim_chars), result);
	}
	if (var_value && dollar % 2 == 1)
	{
		printf("indicator 1 %s\n", var_name);
		return (var_name);
	}
	else
		return ("");
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

char *expand_env_in_word(const char *word, t_env *l_env, bool is_single_quoted) 
{
	char *copy;
	char *token;
	char *expanded_word;
	int i;
	char *expanded_token;

	expanded_word = NULL;
	copy = ft_strdup(word);
	token = ft_strtok(copy, "$");
	while (token) 
	{
		i = 0;
		if (!is_single_quoted)
			expanded_token = get_env_var(token, &i, l_env);
		else
			expanded_token = ft_strdup(token);
		if (expanded_word) 
		{
			expanded_word = realloc(expanded_word, ft_strlen(expanded_word) + ft_strlen(expanded_token) + 1);
			ft_strcat(expanded_word, expanded_token);
		} 
		else
			expanded_word = ft_strdup(expanded_token);
		free(expanded_token);
		token = ft_strtok(NULL, "$");
	}
	free(copy);
	return expanded_word;
}
