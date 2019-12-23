CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -Wunused-result -lm `pkg-config --cflags --libs gtk+-3.0`
OBJFILES=main.o clues.o crossword.o crossword_editor.o dictionary.o template_editor.o
TARGET=crossword-maker

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES)

clean:
	rm -f $(OBJFILES) $(TARGET)
