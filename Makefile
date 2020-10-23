CC=tcc
CFLAGS=\
	-ggdb \
	-pipe \
	-Wall \
	-Wextra

FILES=\
	armor.c \
	formation.c \
	game.c \
	input.c \
	player.c \
	util.c \
	weapon.c

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
