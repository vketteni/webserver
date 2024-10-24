NAME=webserv
CC=c++

MAIN = Logger AMessage Response Request ConfigParser RequestParser MethodHandler ClientConnection Server main FileManager StatusCode CGIExecutor Utils HeaderProcessor

OBJ_DIR= obj/
SRC_DIR= src/
LOG_DIR= log/

SRCS = $(addprefix $(SRC_DIR), $(addsuffix .cpp, $(MAIN)))

OBJS = $(SRCS:src/%.cpp=$(OBJ_DIR)%.o)

FLAGS=-Wall -Werror -Wextra -std=c++98 -g
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
	$(RM) -r $(LOG_DIR)

re: fclean all

# Neue Regel, um nach "// CHANGEME:" zu suchen
check:
	@git grep -C 1 "// CHANGEME:"  -- '*.cpp' '*.h' || echo "Keine 'CHANGEME'-Kommentare gefunden."

.PHONY: all clean re check
