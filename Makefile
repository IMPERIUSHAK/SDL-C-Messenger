CC = gcc
CFLAGS = -Wall -g -lSDL2 -Iinclude
SRC = src/main.c src/client.c src/gui.c
OBJ = $(SRC:.c=.o)
EXEC = messenger

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(CFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJ) $(EXEC)