#colors
RED		=\033[1;31m
GREEN	=\033[1;32m
YELLOW	=\033[1;33m
CYAN	=\033[1;36m
RESET	=\033[0m

NAME= ircserv

SOURCEFILES=	main.cpp \
				server/Server.cpp \
				server/RegistedCommands.cpp \
				server/ping_pong.cpp \
				user/User.cpp \
				user/error_modul.cpp \
				user/sendReply.cpp \
				channel/Channel.cpp \
				channel/ChannelCommands.cpp \
				utils_commands/utils.cpp

#BOTFOLDER = bot/

SOURCEFOLDER= srcs/

OSOURCEFOLDER= objects/

INCLUDEFOLDER= include/

SOURCE= $(addprefix $(SOURCEFOLDER), $(SOURCEFILES))

OSOURCE= $(addprefix $(OSOURCEFOLDER), $(SOURCEFILES:.cpp=.o))

all:  $(NAME) #bot

$(OSOURCEFOLDER):
	@printf "$(YELLOW)"
	mkdir objects
	mkdir objects/server
	mkdir objects/user
	mkdir objects/channel
	mkdir objects/utils_commands
	@printf "$(RESET)"

$(OSOURCEFOLDER)%.o: $(SOURCEFOLDER)%.cpp
	@printf "$(GREEN)"
	clang++ -Wall -Werror -Wextra -c $< -o $@ -std=c++98 -I $(INCLUDEFOLDER)
	@printf "$(RESET)"

#bot:
#	$(MAKE) -C $(BOTFOLDER) all

$(NAME): $(OSOURCEFOLDER) $(OSOURCE)
	@printf "$(GREEN)"
	clang++ $(OSOURCE) -o $(NAME)
	@printf "$(RESET)"

clean:
#	$(MAKE) -C $(BOTFOLDER) clean
	@rm -rf $(OSOURCEFOLDER)
	@echo "$(RED) [INFO] Objects removed! $(RESET)"

fclean: clean
#	$(MAKE) -C $(BOTFOLDER) fclean
	@rm -rf $(NAME)
	@echo "$(RED) [INFO] $(NAME) removed! $(RESET)"

re: fclean all

.PHONY: clean fclean re all bot libjson