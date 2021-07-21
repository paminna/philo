SRCS = philo.c utils.c lifecycle.c main.c

OBJS = ${SRCS:.c=.o}

NAME = philo

HEADER = philo.h

RM = rm -f

CC = gcc

%.o: %.c
		$(CC) -Wall -Wextra -Werror -g -c $< -o $@

$(NAME): $(OBJS) $(HEADER)
		 $(CC) $(OBJS) -o ${NAME}

all:	 ${NAME}

clean:
		 ${RM} ${OBJS}

fclean:	 clean
		 ${RM} ${NAME}

re:		 fclean all

.PHONY:	 all bonus clean fclean re