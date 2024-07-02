# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jimchoi <jimchoi@student.42seoul.kr>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/22 11:45:19 by jimchoi           #+#    #+#              #
#    Updated: 2024/07/02 13:01:04 by jimchoi          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = 
#-Wall -Werror -Wextra
NAME = philosophers

ifdef DEBUG
	CFLAGS += -g3 -fsanitize=thread
endif
ifdef DEBUG_S
	CFLAGS += -g3 -fsanitize=address
endif

SRCS = p_mian.c p_utils.c 
# philo.c philo_utils.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS)  -c $< -o $@ 

$(NAME): $(OBJS)
	$(CC)  $(CFLAGS) -o $@ $(OBJS)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re