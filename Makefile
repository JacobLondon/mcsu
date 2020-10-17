CC=tcc
CFLAGS=\
	-pipe \
	-Wall \
	-Wextra

FILES=\
	game.c \
	input.c \
	player.c \
	util.c

ifeq ($(OS),Windows_NT)
	RM = del /f
	TARGET=out.exe
else
	RM = rm -rf
	TARGET=out
endif

.PHONY: clean

all: $(TARGET)

$(TARGET): $(FILES)
	$(CC) -o $@ $^ $(CFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	$(RM) $(TARGET)
