NAME = ircserv

CC = g++
CFLAGS = -Wall -Wextra -Werror -std=c++98 

SRC = main.cpp \
	  src/Server.cpp \
	  src/User.cpp \
	  src/channel.cpp
	  
OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)
fclean: clean
	rm -f $(NAME)
re: fclean all

.PHONY: all clean fclean re

