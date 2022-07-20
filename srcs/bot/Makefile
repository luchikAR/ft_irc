NAME = bot

CC = clang++
CFLAGS = -Wall -Wextra -g -std=c++98

MAIN = main
MODELS = Bot ConvertBot
MESSAGE = Message

SRCS = $(addsuffix .cpp, $(MAIN)) \
	   $(addsuffix .cpp, $(MODELS)) \
	   $(addsuffix .cpp, $(MESSAGE))

OBJS_DIR = ./objs/
OBJS_SUB = $(addprefix $(OBJS_DIR), Models Message)
OBJS = $(addprefix $(OBJ_DIR), $(addsuffix .o, $(MAIN))) \
	  $(addprefix $(OBJ_DIR), $(addsuffix .o, $(MODELS))) \
	  $(addprefix $(OBJ_DIR), $(addsuffix .o, $(MESSAGE)))

OBJS_BUILD = $(addprefix $(OBJS_DIR), $(SRCS:.cpp=.o))

CLASSES = -I ./Models -I ./Message

all: $(NAME)

$(NAME): $(OBJS_DIR) $(OBJS_BUILD)
		$(CC) $(CFLAGS) $(CLASSES) $(OBJS_BUILD) -o $(NAME)

$(OBJS_DIR)%.o: %.cpp
				$(CC) $(CFLAGS) $(CLASSES) -c $< -o $@

$(OBJS_DIR):
			mkdir -p $(OBJS_DIR)
			mkdir -p $(OBJS_SUB)

clean:
	rm -rf $(OBJS_BUILD)
	rm -rf $(OBJS_SUB)
	rm -rf $(OBJS_DIR)

fclean: clean
		rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re