CC       = gcc
RAYLIB   = $(shell brew --prefix raylib)

CFLAGS   = -g -std=c11 -Wall -Wextra -I$(INC_DIR) -I$(RAYLIB)/include
LDFLAGS  = -L$(RAYLIB)/lib
LIBS     = -lraylib -lm \
           -framework IOKit -framework Cocoa -framework OpenGL -framework CoreVideo

TARGET   = bitforge
SRC_DIR  = src
OBJ_DIR  = build
INC_DIR  = include

_DEPS    = types.h storage.h bitmap.h
_OBJS    = main.o storage.o bitmap.o

DEPS     = $(patsubst %,$(INC_DIR)/%,$(_DEPS))
OBJS     = $(patsubst %,$(OBJ_DIR)/%,$(_OBJS))

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS) $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS) | $(OBJ_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ_DIR):
	mkdir -p $@

run: all
	./$(TARGET) bin.bin

clean:
	$(RM) $(TARGET) $(OBJS)