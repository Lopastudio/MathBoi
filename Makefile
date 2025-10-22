CC = gcc
CFLAGS = -Wall -std=c99 $(shell pkg-config --cflags raylib)
LDFLAGS = $(shell pkg-config --libs raylib) -lm

SRC = main.c
OUT = ./bin/mathboi

all: $(OUT)

$(OUT): $(SRC)
	@mkdir -p ./bin
	$(CC) $(CFLAGS) -o $(OUT) $(SRC) $(LDFLAGS)

run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT)
