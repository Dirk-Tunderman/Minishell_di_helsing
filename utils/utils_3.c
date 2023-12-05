#include "../minishell.h"


int count_quotes(char *line)
{
    int i = 0;
    int count = 0;
    while (line[i])
    {
        if ((line[i] == '\"' || line[i] == '\''))
            count++;
        i++;
    }
    return count;
}

bool check_quotes_correctness(const char *line)
{
    char quote_stack[100];
    int stack_index;
    int i;

    stack_index = -1;
    i = 0;
    while(line[i])
    {
        if (line[i] == '\"' || line[i] == '\'')
        {
            if (stack_index >= 0 && quote_stack[stack_index] == line[i])
                stack_index--;
            else if (stack_index >= 0 && quote_stack[stack_index] != line[i])
            {
                stack_index++;
                quote_stack[stack_index] = line[i];
            }
            else
            {
                stack_index++;
                quote_stack[stack_index] = line[i];
            }
        }
        i++;
    }
    return stack_index == -1;
}

int	checker_quotes(char *line)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	k = 0;
	while (line[i]) // check even and uneven number of quotes
	{
		if (line[i] == '\"')
			j++;
		if (line[i] == '\'')
			k++;
		i++;
	}
	if (j % 2 != 0 || k % 2 != 0)
		return (0);
   // if (check_quotes_correctness(line) == false)
   //     return (0);
	return (1);
}

void    display_list(t_node *head)
{
    t_node *current = head;
    while (current != NULL)
    {
        printf("Data: %s, Type: %d, redirect: %d, path: %s\n space_after: %d\n, before_env: %s\n", current->data, current->type, current->redirect, current->path, current->space_after, current->before_env);
        current = current->next;
    }
}

void    free_node(t_node *node)
{
    if (node == NULL)
        return ;
    free(node->data);
    free(node);
}

void    free_list(t_node *head)
{
    t_node *next;

    while (head != NULL)
    {
        next = head->next;
        free_node(head);
        head = next;
    }
}


char** ft_split(char* str, char c) 
{
    int i, j, k;
    int wc = 0; // word count
    int len = strlen(str);

    // Counting the number of words
    for (i = 0; i < len; i++) {
        if (str[i] == c) wc++;
    }

    // Adding space for the null-terminator at the end of the array of strings
    char** result = (char**)malloc(sizeof(char*) * (wc + 2));

    i = 0;
    k = 0;

    while (str[i] == c) i++; // skip leading delimiters

    while (i < len) {
        j = 0;
        while (str[i] != c && i < len) {
            i++;
            j++;
        }
        result[k] = (char*)malloc(sizeof(char) * (j + 1)); // j+1 for the null-terminator
        strncpy(result[k], &str[i - j], j);
        result[k][j] = '\0'; // null-terminator for the string
        k++;
        while (str[i] == c && i < len) i++; // skip consecutive delimiters
    }

    result[k] = NULL; // null-terminator for the array of strings

    return result;
}

int is_command(char *data) 
{
    if (data && isalpha(data[0])) {  // If the first character is a letter
        return 1;
    }
    return 0;
}
