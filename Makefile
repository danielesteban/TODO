SHELL = /bin/sh
CC = cc

CFLAGS = -std=c90 -pedantic -Wall -Wextra -march=native -ggdb3
DEBUGFLAGS = -O0 -D _DEBUG
RELEASEFLAGS = -O3 -D NDEBUG

TARGET = TODO
SOURCES = $(shell echo src/*.c)
OBJECTS = $(SOURCES:.c=.o)

PREFIX = $(DESTDIR)/usr/local
BINDIR = $(PREFIX)/bin

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(DEBUGFLAGS) -o $(TARGET) $(OBJECTS)

release: $(SOURCES)
	$(CC) $(CFLAGS) $(RELEASEFLAGS) -o $(TARGET) $(SOURCES)

profile: CFLAGS += -pg
profile: $(TARGET)

install: release
	install $(TARGET) $(BINDIR)/$(TARGET)

install-strip: release
	install -s $(TARGET) $(BINDIR)/$(TARGET)

uninstall:
	-rm $(BINDIR)/$(TARGET)

clean:
	-rm -f $(OBJECTS)
	-rm -f gmon.out

distclean: clean
	-rm -f $(TARGET)
	-rm -rf $(TARGET).dSYM

.PHONY: all profile release \
	install install-strip uninstall clean distclean
