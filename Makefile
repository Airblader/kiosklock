TARGET = kiosklock
SDIR = src
IDIR = include
ODIR = obj

INSTALL = install
PREFIX = /usr/bin

MANDIR = /usr/share/man/man1

CC = gcc
CFLAGS += -I$(IDIR)
CFLAGS += -std=gnu99
CFLAGS += -Wall -Wundef -Wshadow
CFLAGS += $(shell pkg-config --cflags cairo pango xcb xcb-atom xcb-aux xcb-dpms xcb-randr)
LIBS   += $(shell pkg-config --libs   cairo pango xcb xcb-atom xcb-aux xcb-dpms xcb-randr)
LIBS   += -lev

INCS = $(wildcard $(IDIR)/*.h)
SRCS = $(wildcard $(SDIR)/*.c)
OBJS = $(patsubst %,$(ODIR)/%,$(notdir $(SRCS:.c=.o)))

MANS = man/kiosklock.1

.NOTPARALLEL:

.PHONY: all
all: clean $(TARGET) mans

.PHONY: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

$(ODIR)/%.o: $(SDIR)/%.c $(INCS)
	$(CC) -D'__VERSION="$(shell git describe --all --long --always)"' $(CFLAGS) -o $@ -c $<

.PHONY: install
install: $(TARGET)
	$(INSTALL) -Dm 0755 $(TARGET) $(DESTDIR)$(PREFIX)/$(TARGET)
	$(INSTALL) -Dm 0644 man/kiosklock.1 $(DESTDIR)$(MANDIR)/kiosklock.1

.PHONY: uninstall
uninstall:
	$(RM) $(DESTDIR)$(PREFIX)/$(TARGET)
	$(RM) $(DESTDIR)$(MANDIR)/kiosklock.1

.PHONY: mans
mans: $(MANS)

$(MANS): %.1: %.man
	a2x -f manpage $<

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS)
	$(RM) man/*.1 man/*.xml
