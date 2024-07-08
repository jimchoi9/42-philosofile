# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jimchoi <jimchoi@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/22 11:45:19 by jimchoi           #+#    #+#              #
#    Updated: 2024/07/08 13:17:20 by jimchoi          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS =
#  -Wall -Werror -Wextra
NAME = philo

ifdef DEBUG
	CFLAGS += -g3 -fsanitize=thread
endif
ifdef DEBUG_S
	CFLAGS += -g3 -fsanitize=address
endif

SRCS = p_main.c p_utils.c 

OBJS = $(SRCS:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS)  -c $< -o $@ 

all: $(NAME)

$(NAME): $(OBJS)
	$(CC)  $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re