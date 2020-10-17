CC=tcc
TARGET=out
CFLAGS=\
	-ggdb \
	-pipe \
	-Wall \
	-Wextra

FILES=\
	game.c \
	input.c \
	player.c \
	util.c

.PHONY: clean

all: $(TARGET)

$(TARGET): $(FILES)
	$(CC) -o $@ $^ $(CFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(TARGET)
