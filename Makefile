CC = gcc
CFLAGS = -Wall
LDFLAGS = -lGL -lGLU -lglut

SOURCES = $(wildcard *.c)
TARGETS = $(SOURCES:.c=)

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) -lm

clean:
	rm -f $(TARGETS)


