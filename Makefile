NAME= ircserv

SOURCEFILES=	main.cpp \
				Server.cpp \
				User.cpp

#BOTFOLDER = bot/

SOURCEFOLDER= srcs/

OSOURCEFOLDER= objects/

INCLUDEFOLDER= include/

SOURCE= $(addprefix $(SOURCEFOLDER), $(SOURCEFILES))

OSOURCE= $(addprefix $(OSOURCEFOLDER), $(SOURCEFILES:.cpp=.o))

all:  $(NAME) #bot

$(OSOURCEFOLDER):
	mkdir objects
	mkdir objects/commands

$(OSOURCEFOLDER)%.o: $(SOURCEFOLDER)%.cpp
	clang++ -Wall -Werror -Wextra -c $< -o $@ -std=c++98 -I $(INCLUDEFOLDER)

#bot:
#	$(MAKE) -C $(BOTFOLDER) all

$(NAME): $(OSOURCEFOLDER) $(OSOURCE)
	clang++ $(OSOURCE) -o $(NAME)

clean:
#	$(MAKE) -C $(BOTFOLDER) clean
	rm -rf $(OSOURCEFOLDER)

fclean: clean
#	$(MAKE) -C $(BOTFOLDER) fclean
	rm -rf $(NAME)

re: fclean all

.PHONY: clean fclean re all bot libjson