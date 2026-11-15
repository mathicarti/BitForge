CC       = gcc
RAYLIB   = $(shell brew --prefix raylib)

CFLAGS   = -g -std=c11 -Wall -Wextra -Iinclude -I$(RAYLIB)/include
LDFLAGS  = -L$(RAYLIB)/lib
LIBS     = -lraylib -lm \
           -framework IOKit -framework Cocoa -framework OpenGL

TARGET   = bitforge
SRC_DIR  = src
OBJ_DIR  = build
INC_DIR  = include

_DEPS    = types.h
_OBJS    = main.o

DEPS     = $(patsubst %,$(INC_DIR)/%,$(_DEPS))
OBJS     = $(patsubst %,$(OBJ_DIR)/%,$(_OBJS))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS) $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS) | $(OBJ_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ_DIR):
	mkdir -p $@

clean:
	$(RM) $(TARGET) $(OBJS)