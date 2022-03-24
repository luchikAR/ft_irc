#colors
RED=\033[1;31m
GREEN=\033[1;32m
YELLOW=\033[1;33m
CYAN=\033[1;36m
RESET=\033[0m


CC		= clang++ 
RM		= rm -f
DEBUG	= -g
FLAGS	= -Wall -Wextra -Werror -MMD -std=c++98
INCLUDES= -I ./includes

NAME	= ircserv

SRCS_FT		= main.cpp
OBJS		= $(SRCS_FT:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS) $(hpp)
	@$(CXX) $(OBJS) -o $@

%.o: %.cpp
	@printf "\x1b[32m"
	$(CXX) $(CXXFLAGS) -o $@ -c $<
	@printf "\033[0m"

clean:
	@rm -f $(OBJS)
	@echo "\033[31m [INFO] Objects removed! \033[0m"

fclean: clean
	@rm -f $(NAME)
	@echo "\033[31m [INFO] $(NAME) removed! \033[0m"

re:     fclean all

.PHONY: all clean fclean re