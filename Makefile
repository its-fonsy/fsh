# directory structure
SRC_DIR = src
INC_DIR = inc
OBJ_DIR = obj

# compiler and target
CC	= cc
TARGET	= fsh

# flags
CFLAGS	= -Wall -Wextra -Werror -pedantic -g -O0 -I$(INC_DIR)
LFLAGS	= -Wall -Wextra -Werror

# declaration variables
SRCS := $(wildcard $(SRC_DIR)/*.c)
INCS := $(wildcard $(INC_DIR)/*.h)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Linking
$(TARGET): $(OBJS)
	$(CC) $(LFLAGS) -o $@ $(OBJS) 

# Compilation
$(OBJS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

# clean
clean:
	@rm -rf $(OBJS) $(OBJ_DIR) $(TARGET)

all: $(TARGET)

.PHONY: clean all
