#include "../minishell.h"

void lexer(char *input, t_node **head, char **envp)
{
    int i = 0;
    t_token prev_type = OPERATOR;  // Initialize as OPERATOR

    while (input[i])
    {
        if (ft_isspace(input[i])) // skip spaces
        {
            i++;
            continue;
        }
        process_token(input, &i, &prev_type, head, envp); // Pass envp to process_token
        check_and_set_redirect(*head);
    }
}

void process_token(char *input, int *i, t_token *prev_type, t_node **head, char **envp)
{
    char *data;
    t_token type;

    if (input[*i] == '$') //for env without quotes
    {
        data = get_env_var(input, i, envp); // Get environment variable
        type = ARG;
    }
    else if (is_operator(input[*i])) // Check for operator
    {
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
        data = get_quoted_word(input, i, envp, '\"'); // Get quoted word
        type = QUOTE_ARG;
    }
    else if (input[*i] == '\'') // Check for single quotes
    {
        data = get_quoted_word(input, i, envp, '\''); // Get quoted word
        type = QUOTE_ARG;
    }
    else
    {
        data = get_word(input, i); // Get word
        type = ARG;
    }
    append_node(head, data, type); // Append node to list
    *prev_type = type;
}

char *get_quoted_word(char *input, int *i, char **envp, char quote_type)
{
    //bool is_single_quoted = quote_type == '\'';
    char *result;
    (*i)++; // Skip the quote at the beginning
    int start = *i; // Find the start of the word

    while(input[*i])
    {
        if (input[*i] == quote_type && input[*i + 1] == ' ')
        {
            (*i)++;
            break;
        }
        else
            (*i)++;
    }
    if (quote_type == '\"' && ft_strchr(&input[start], '$') != NULL)
    {
        result = get_env_var(input, &start, envp);
        (*i)++;
    }
    else
    {
        // Just duplicate the word without expanding
        result = ft_strndup(&input[start], *i - start - 1);

        if (input[*i] == quote_type)
         (*i)++; // Skip the closing quote
        result = remove_quotes(result);
    }
    return (result);
}


char *remove_quotes(char *input)
{
    int i, j = 0;
    int len = ft_strlen(input);
    char *result = malloc(len + 1); // Allocating memory for the result string

    if (result == NULL) {
        printf("Failed to allocate memory\n");
        return NULL;
    }

    for(i = 0; i < len; i++)
    {
        if(input[i] != '\"' && input[i] != '\'')
        {
            result[j] = input[i];
            j++;
        }
    }
    result[j] = '\0'; // Don't forget to null terminate the result string
    return result;
}

char *get_word(char *input, int *i)
{
    int start = *i;

    while (input[*i] && !ft_isspace(input[*i]) && !is_operator(input[*i]))
        (*i)++;

    // Create a new string from the start index to the current index
    char *word = ft_strndup(&input[start], *i - start);

    return word;
}


/* New function: check_end_token_quote */
int	check_end_token_quote(char *input, char character, int start)
{
	int	len;

	len = 0;
	do 
	{
		len++;
	} 
	while (input[start + len] && input[start + len] != character);
	len++; // Include the closing quote
	return (len);
}

/* New function: check_end_token_alnum */
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
	new_node->data = strdup(str);
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
    while (commands[i] != NULL) {
        if (ft_strcmp(str, commands[i]) == 0) {
            return (1);
        }
        i++;
    }
    return (0);
}

void append_node(t_node **head, char *data, t_token type)
{
    t_node *new_node = malloc(sizeof(t_node));
    if (!new_node)
    {
        printf("Error: Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    new_node->data = strdup(data);
    new_node->type = type;
    new_node->path = NULL;
    new_node->next = NULL;

    if (*head == NULL) // If the list is empty, make the new node the head
    {
        *head = new_node;
    }
    else // Else, traverse to the end of the list and add the new node
    {
        t_node *current = *head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = new_node;
    }
}

char *get_env_var(char *input, int *i, char **envp)
{
    (*i)++; // Skip the '$'
    
    int non_delim_index;
    int end;
    int dollar = 0;

    while (input[*i] == '$')
    {
        (*i)++;
        dollar++;
    }
    int start = *i;

    while (input[*i] && (isalnum(input[*i]) || input[*i] == '_')) // Check for non-delimiting characters
        (*i)++;

    non_delim_index = *i; // Save the index of the non-delimiting characters
    while(input[*i] && !ft_isspace(input[*i]) && !is_operator(input[*i]) && input[*i] != '\"') // Check for non-delimiting characters
        (*i)++;
    end = *i; // Save the index of the last non-delimiting character

    char *delim = ft_substr(&input[non_delim_index], 0, end - non_delim_index); // Get the delimiter later concatinate with other strings

    // Create a new string from the start index to the current index
    char *var_name = strndup(&input[start], non_delim_index - start);
    printf("var_name: %s\n", var_name);
    char *var_value = find_env_var(var_name, envp);


    if (var_value && dollar % 2 == 0)
    {

        while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '\0' && !is_operator(input[*i]))
            (*i)++;

        // Create a new string from the start index of the non-delimiting characters to the current index
        char *non_delim_chars = strndup(&input[non_delim_index], *i - non_delim_index);

        char *result = malloc(strlen(var_value) + strlen(non_delim_chars) + 1 + strlen(delim));
        strcpy(result, var_value);
        strcat(result, delim);
        //strcat(result, non_delim_chars);

        free(non_delim_chars); // Free the non-delimiting characters string

        return result; // Return a copy of the variable value
    }
    else if (var_value && dollar % 2 == 1)
    {
        return (var_name);
    }
    else
    {
        return (""); // If variable not found, return the variable name
    }
}

char *find_env_var(const char *var_name, char **envp)
{
    size_t len = strlen(var_name);

    for (char **env = envp; *env; env++)
    {
        if (strncmp(*env, var_name, len) == 0 && (*env)[len] == '=')
        {
            // Return a pointer to the value part of the "name=value" string
            return &(*env)[len + 1];
        }
    }

    // Return NULL if the variable name is not found in envp
    return NULL;
}

//for my enviornment variables i got a problem. whenever i put a delimiter 
void check_and_set_redirect(t_node *head) 
{
    t_node *current = head;
    while (current != NULL)
    {
        if (ft_strcmp(current->data, "<") == 0)
            current->type = STDIN_RD;
        else if (ft_strcmp(current->data, ">") == 0)
            current->type = STDOUT_RD;
        else if (ft_strcmp(current->data, ">>") == 0)
            current->type = APPEND;
        else if (ft_strcmp(current->data, "<<") == 0)
            current->type = HERDOC;

        current = current->next;
    }
}

char *expand_env_in_word(const char *word, char **envp, bool is_single_quoted) 
{
    char *copy = strdup(word); // Make a copy of word to prevent modifying the original string
    char *token = strtok(copy, "$"); // Split the string at $ sign
    char *expanded_word = NULL;
    
    while (token) {
        int i = 0;
        char *expanded_token;
        if (!is_single_quoted) {
            expanded_token = get_env_var(token, &i, envp); // Expand only if not single-quoted
        } else {
            expanded_token = strdup(token); // Otherwise, copy as is
        }

        if (expanded_word) {
            expanded_word = realloc(expanded_word, strlen(expanded_word) + strlen(expanded_token) + 1);
            strcat(expanded_word, expanded_token);
        } else {
            expanded_word = strdup(expanded_token);
        }
        free(expanded_token);
        token = strtok(NULL, "$");
    }
    
    free(copy); // Free the copy
    return expanded_word;
}
