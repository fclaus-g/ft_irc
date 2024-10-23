NAME = ircserv

SRC_DIR = src

SRCS = $(wildcard $(SRC_DIR)/*.cpp)

INC_DIR = inc

OBJ_DIR = obj

OBJS = $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.cpp=.o)))

CC = c++

FLAGS = -Wall -Werror -Wextra -std=c++98

RM = rm -f

GREEN = "\033[32m"
YELLOW = "\033[33m"
RED = "\033[31m"
NOCOLOR = "\033[0m"

all: $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@$(CC) $(FLAGS) -c $< -o $@

$(NAME): $(OBJS) $(INC_DIR)/ft_irc.hpp
	@echo $(GREEN)Compiling...$(NOCOLOR)
	@$(CC) $(FLAGS) $(SRCS) -o $(NAME)
	@echo $(GREEN)Program ircserv ready$(NOCOLOR)

clean:
	@$(RM) $(OBJ_DIR)/$(OBJS)
	@rm -r $(OBJ_DIR)
	@echo ircserv objects files $(RED)cleaned$(NOCOLOR)

fclean: clean
	@$(RM) $(NAME)
	@echo ircserv executable file $(RED)cleaned$(NOCOLOR)

re: fclean all
	@echo $(RED)Cleaned$(NOCOLOR) and $(GREEN)rebuilt$(NOCOLOR) ircserv

.PHONY: all clean fclean re
