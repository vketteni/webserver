NAME=webserv
CC=c++

MAIN = main Server ClientHandler HTTPRequest HTTPResponse

OBJ_DIR= obj/
SRC_DIR= src/

SRCS = $(addprefix $(SRC_DIR), $(addsuffix .cpp, $(MAIN))) 

OBJS = $(SRCS:src/%.cpp=$(OBJ_DIR)%.o)

FLAGS=-Wall -Wextra -Werror -std=c++98 -g
# FLAGS+= -DNDEBUG
INC= -I incl/

all: $(NAME)

$(NAME):  $(OBJS)
		$(CC) $(FLAGS) $(INC) $(OBJS) -o $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp | $(OBJ_DIR)
	$(CC) $(FLAGS) $(INC) -c $< -o $@

clean:
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean re