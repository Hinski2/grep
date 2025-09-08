SRC_DIRS = . structs utils
TARGET = my_grep

CC = cc 
FLAGS = -std=c17 -Wall -Wextra -g -O0 -fsanitize=address

SRC = $(foreach d, $(SRC_DIRS), $(wildcard $(d)/*.c))
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)