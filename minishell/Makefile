NAME = minishell
CC = cc

RM = rm -rf

FLAGS = -Wall -Wextra -Werror

LDFLAGS = -lreadline

SRC = main.c tokenisation.c error_handeling.c  \
		error_nodes.c utils.c utils_2.c utils_3.c resolute.c init_cmd.c\
		env_in_list.c\

VPATH := errors parsing executor utils
OBJS = $(SRC:.c=.o)

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
