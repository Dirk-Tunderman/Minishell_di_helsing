# Instuction for running Minishell

1. copy repo
2. run make
3. ./minishell

# Notions

- I put the environment in a linked list
- the following I specified in enum :
   typedef enum s_token {
    OPERATOR, specified: |
    ARG, semi-specified all arguments without "" or ''
    QUOTE_ARG, specified all arguments with "" or ''
    NONE, 
    CMD, not-specified
    BUILT_IN, not-specified -> for excecution
    STDIN_RD, specified
    STDOUT_RD, specified
    HERDOC, specified
    APPEND, specified   
    REPROMPT, not specified
} t_token;
