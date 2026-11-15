CC=gcc
CFLAGS=-g -Wall -Wextra -Iinclude
LIBS=-lm
TARGET=template_editor

SRC_DIR = src
OBJ_DIR = build
INC_DIR = include

_DEPS = types.h
_OBJS = main.o

DEPS = $(patsubst %,$(INC_DIR)/%,$(_DEPS))
OBJS = $(patsubst %,$(OBJ_DIR)/%,$(_OBJS))


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIBS)

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS)