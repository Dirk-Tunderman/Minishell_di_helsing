NAME := minishell

CC := cc

RM := rm -rf

SRC := main.c tokenisation.c error_handeling.c  \
		utils.c utils_2.c utils_3.c ft_itoa.c ft_atoi.c ft_isint.c resolute.c\
		env_in_list.c signals.c ft_split.c

LDFLAGS += -lreadline -L$(shell brew --prefix)/opt/readline/lib/
CPPFLAGS += -I$(shell brew --prefix)/opt/readline/include/ -I.
CFLAGS += -Wall -Wextra -Werror

SRC_EXEC := apply_redir.c exec.c fail.c final_parse.c ft_lstnew.c builtins.c call.c

VPATH := errors parsing execution utils signals execution/built_ins/

OBJS := $(SRC:.c=.o) $(SRC_EXEC:.c=.o)
all: $(NAME) 

$(NAME) : $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(NAME)

%.o : %.c minishell.h Makefile
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all
