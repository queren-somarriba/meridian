NAME = outrenoir

CC = c++

INCLUDES_DIR = includes

CFLAGS = -Wall -Werror -Wextra -O2 -g -fsanitize=address -I./$(INCLUDES_DIR)

GLFLAGS = -lglfw -lGLEW -framework OpenGL #-lglut -lGLEW -lGLU -lGL

SRCS_DIR = srcs

SRCS =	$(SRCS_DIR)/outrenoir.cpp \
	$(SRCS_DIR)/noise.cpp \
	$(SRCS_DIR)/flowfield.cpp \
	$(SRCS_DIR)/engine.cpp \
	$(SRCS_DIR)/poissonDiskSampling.cpp \
	$(SRCS_DIR)/fileUtils.cpp \
	$(SRCS_DIR)/ffmpegRecorder.cpp	

OBJ_DIR = obj

OBJS = $(SRCS:$(SRCS_DIR)/%.cpp=$(OBJ_DIR)/%.o)

$(OBJ_DIR)/%.o: $(SRCS_DIR)/%.cpp | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

RM = rm -rf

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(CFLAGS) $(OBJS) $(GLFLAGS)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re