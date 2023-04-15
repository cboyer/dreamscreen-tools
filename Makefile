CFLAGS = -Wall -Wextra -Werror -O2
OS = $(shell uname)
ALLTARGETS = dreamscreen-cli dreamscreend

ifdef DEBUG
	CFLAGS := $(CFLAGS) -DDEBUG
endif

ifeq ($(OS),Linux)
	CC = gcc
endif

ifeq ($(OS),FreeBSD)
	CC = clang
	ALLTARGETS = dreamscreen-cli
endif

.PHONY: all clean install uninstall
.INTERMEDIATE: dreamscreen-cli.o dreamscreend.o

all: clean $(ALLTARGETS)
cli: dreamscreen-cli
daemon: dreamscreend

dreamscreen-cli.o: dreamscreen-cli.c
	$(CC) $(CFLAGS) -c dreamscreen-cli.c -o dreamscreen-cli.o

dreamscreen-cli: dreamscreen-cli.o
	$(CC) $(CFLAGS) dreamscreen-cli.o -o dreamscreen-cli

dreamscreend.o: dreamscreend.c
	$(CC) $(CFLAGS) -c dreamscreend.c -o dreamscreend.o

dreamscreend: dreamscreend.o
	$(CC) $(CFLAGS) dreamscreend.o -o dreamscreend

clean:
	$(RM) *.o dreamscreend dreamscreen-cli

install:
	install -m 755 dreamscreen-cli /usr/local/bin/
	if test -f dreamscreend ; then install -m 755 dreamscreend /usr/local/bin/ ; fi
	if test -f dreamscreend ; then install -m 744 dreamscreend.service /usr/lib/systemd/system/ ; fi
	if test -f dreamscreend ; then systemctl daemon-reload ; fi

uninstall:
	if test -f /usr/lib/systemd/system/dreamscreend.service ; then systemctl stop dreamscreend && systemctl disable dreamscreend && $(RM) /usr/lib/systemd/system/dreamscreend.service && systemctl daemon-reload ; fi
	$(RM) /usr/local/bin/dreamscreen-cli /usr/local/bin/dreamscreend

