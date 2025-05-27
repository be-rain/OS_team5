CC = gcc
CFLAG = -fcommon -D _GNU_SOURCE #-fsanitize=address -g3
RM = rm -rf
NAME = TEAM5_LINUX

SRC_DIR = source
OBJ_DIR = object
INCS = include

SRCS = directory.c instruction.c main.c touch.c \
		stack.c time.c user.c cat.c cd.c thread.c \
		chmod.c chown.c system.c find.c mkdir.c directorytree.c \
		rm.c pwd.c ls.c permission.c grep.c userlist.c
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAG) -o $@ $(OBJS) -I$(INCS) -lpthread

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAG) -c $< -o $@ -I$(INCS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean
	make all

.PHONY: all clean fclean re
