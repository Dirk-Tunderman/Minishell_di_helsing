NAME = minishell
CC = cc

RM = rm -rf

FLAGS = -Wall -Wextra -Werror -I. -fsanitize=address -g

LDFLAGS = -lreadline

SRC = main.c tokenisation.c error_handeling.c  \
		error_nodes.c utils.c utils_2.c utils_3.c resolute.c init_cmd.c\
		env_in_list.c

SRC_EXEC = apply_redir.c exec.c fail.c final_parse.c ft_lstadd_back.c ft_lstlast.c ft_lstnew.c get_next_line.c get_next_line_utils.c execution/built_ins/builtins.c  execution/built_ins/call.c

VPATH := errors parsing execution utils

OBJS = $(SRC:.c=.o) $(SRC_EXEC:.c=.o)

all: $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(FLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

%.o : %.c minishell.h
	$(CC) $(FLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all
