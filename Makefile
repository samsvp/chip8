CC = gcc
CFLAGS = -std=c99 -O2 \
		 -Wall -Wextra -Werror -Iinclude -Wstrict-prototypes \
		 -pedantic-errors -lraylib -lm

MAIN_FILE = main.c
SRC_FILES = $(wildcard src/*.c)
ALL_SRC = $(MAIN_FILE) $(SRC_FILES)
TARGET := chip-8.out

all: $(OBJ_DIR) $(TARGET)

$(TARGET): $(ALL_SRC)
	$(CC) $(ALL_SRC) $(CFLAGS) -o $@

run: $(TARGET)
	./$(TARGET) $(ROM_NAME)

clean:
	rm -f $(TARGET)

.PHONY: all clean
