C = clang

NAME		= philo

SRCS		= ./srcs/main.c \
			  ./srcs/thread.c \
			  ./srcs/util.c \
			  ./srcs/init.c \

OBJS		= $(SRCS:.c=.o)
RM			= rm -f
CC			= gcc
CFLAGS		= -Wall -Wextra -Werror #-fsanitize=address

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) -lpthread $(CFLAGS) ${OBJS} -o ${NAME}

fclean : clean
	$(RM) $(NAME) $(bonus)

clean :
	$(RM) $(OBJS)

re : fclean all

bonus : all

.PHONY: all clean fclean re bonus .c.o    