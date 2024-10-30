NAME=webserv
CC=c++

MAIN = Logger AMessage Response Request ConfigParser RequestParser MethodHandler ClientConnection Server main FileManager CGIExecutor Utils HeaderProcessor

OBJ_DIR= obj/
SRC_DIR= src/
LOG_DIR= log/

DEFAULT_PORT = 8080
DEFAULT_MAX_BODY_SIZE = 10280
DEFAULT_HOST = "127.0.0.1"
DEFAULT_ROOT = "path/from/root/"
DEFAULT_SERVER_NAME = "localhost"

SRCS = $(addprefix $(SRC_DIR), $(addsuffix .cpp, $(MAIN)))

OBJS = $(SRCS:src/%.cpp=$(OBJ_DIR)%.o)

FLAGS=-Wall -Werror -Wextra -std=c++98 -g
# FLAGS+= -DNDEBUG
INC= -I incl/

all: $(NAME)

$(NAME):  $(OBJS)
		$(CC) $(FLAGS) $(INC) $(OBJS) -o $(NAME) \
 		-D DEFAULT_HOST=$(DEFAULT_HOST) \
 		-D DEFAULT_MAX_BODY_SIZE=$(DEFAULT_MAX_BODY_SIZE) \
 		-D DEFAULT_PORT=$(DEFAULT_PORT) \
 		-D DEFAULT_ROOT=$(DEFAULT_ROOT) \
 		-D DEFAULT_SERVER_NAME=$(DEFAULT_SERVER_NAME) \

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
