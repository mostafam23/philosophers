# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mel-moha <mel-moha@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/09 09:51:30 by mel-moha          #+#    #+#              #
#    Updated: 2025/09/09 09:51:31 by mel-moha         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= philo

SRCS	= main.c \
		  init.c \
		  simulation.c \
		  actions.c \
		  utils1.c \
		  utils2.c

OBJS	= $(SRCS:.c=.o)

CC		= cc
CFLAGS	= -Wall -Wextra -Werror -pthread -g
RM		= rm -f

# Colors for output
RED		= \033[0;31m
GREEN	= \033[0;32m
YELLOW	= \033[0;33m
NC		= \033[0m

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(YELLOW)Linking $(NAME)...$(NC)"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
	@echo "$(GREEN)$(NAME) created successfully!$(NC)"

%.o: %.c philo.h
	@echo "$(YELLOW)Compiling $<...$(NC)"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "$(RED)Removing object files...$(NC)"
	@$(RM) $(OBJS)

fclean: clean
	@echo "$(RED)Removing $(NAME)...$(NC)"
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re