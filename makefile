CC = gcc

CFLAGS = -Wall -std=c99 -O3
TARGET = tourists

SRCS = main.c file.c processFiles.c dijkstra.c heap.c

OBJS = main.o file.o processFiles.o dijkstra.o heap.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)