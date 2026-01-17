TARGET = sv.exe

SRCS = src/main.c
SRCS += src/renderer/renderer.c
SRCS += src/window/window.c

CC = x86_64-w64-mingw32-gcc
LFLAGS = -lSDL3 -lSDL3_ttf -lm -lutf8proc
CFLAGS  = -Wall -Wextra -Wpedantic -O1 -std=c23
DEBUG_CFLAGS = -fsanitize=address,undefined,leak -fno-omit-frame-pointer -Wshadow -Wconversion -Wnull-dereference -Wdouble-promotion -g

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o  $(TARGET) $(SRCS) $(LFLAGS)

clean:
	rm $(TARGET)
