TARGET = sv.exe

SRCS = src/main.c
SRCS += src/renderer/renderer.c
SRCS += src/window/window.c
SRCS += src/buffer/create.c
SRCS += src/maps.c
SRCS += src/util.c

CC = gcc
LFLAGS = -lSDL3 -lSDL3_ttf -lm -lutf8proc
CFLAGS  = -Wall -Wextra -Wpedantic -O0 -std=c23
DEBUG_CFLAGS = -Wshadow -Wconversion -Wnull-dereference -Wdouble-promotion -g

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o  $(TARGET) $(SRCS) $(LFLAGS) $(DEBUG_CFLAGS)

clean:
	rm $(TARGET)
