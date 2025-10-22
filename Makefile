CC = gcc
CFLAGS = -Wall -std=c99 $(shell pkg-config --cflags raylib -lm)
LDFLAGS = $(shell pkg-config --libs raylib)

SRC = main.c
OUT = mathboi

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC) $(LDFLAGS)

run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT)
