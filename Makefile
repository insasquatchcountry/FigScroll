# Makefile for fs (FigScroll), inspired by sl by Toyoda Masashi

CC = gcc
CFLAGS = -O
LDFLAGS = -lncurses
TARGET = fs
SOURCES = fs.c
OBJECTS = $(SOURCES:.c=.o)
MANPAGE = fs.1
PREFIX = /usr/local
MANDIR = $(PREFIX)/share/man/man1

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJECTS) $(TARGET)

install: $(TARGET) $(MANPAGE)
	install -d $(PREFIX)/bin
	install -m 755 $(TARGET) $(PREFIX)/bin
	install -d $(MANDIR)
	install -m 644 $(MANPAGE) $(MANDIR)

uninstall:
	rm -f $(PREFIX)/bin/$(TARGET)
	rm -f $(MANDIR)/$(MANPAGE)

.PHONY: all clean install uninstall
