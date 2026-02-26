CC = gcc
CFLAGS = -Wall -g -Iinclude `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs` -lSDL2_ttf -lcjson
SRC = src/main.c src/client.c src/gui.c src/messages.c
OBJ = $(SRC:.c=.o)
EXEC = messenger

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJ) $(EXEC)