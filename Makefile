CC=gcc
CFLAGS=-Wall -Wextra -Werror -O2

default: all
debug: CFLAGS += -DDEBUG
debug: all
all: dreamscreen-cli dreamscreend
cli: dreamscreen-cli

dreamscreen-cli.o: dreamscreen-cli.c
	$(CC) $(CFLAGS) -c dreamscreen-cli.c -o dreamscreen-cli.o

dreamscreen-cli: dreamscreen-cli.o
	$(CC) $(CFLAGS) dreamscreen-cli.o -o dreamscreen-cli

dreamscreend.o: dreamscreend.c
	$(CC) $(CFLAGS) -c dreamscreend.c -o dreamscreend.o

dreamscreend: dreamscreend.o
	$(CC) $(CFLAGS) dreamscreend.o -o dreamscreend

clean:
	-rm -f dreamscreend.o
	-rm -f dreamscreend
	-rm -f dreamscreen-cli.o
	-rm -f dreamscreen-cli

install:
ifneq (,$(wildcard ./dreamscreend))
	-install -m 755 dreamscreend /usr/local/bin/
	-install -m 744 dreamscreend.service /usr/lib/systemd/system/
	-systemctl daemon-reload
endif

ifneq (,$(wildcard ./dreamscreen-cli))
	-install -m 755 dreamscreen-cli /usr/local/bin/
endif

uninstall:
ifneq (,$(wildcard /usr/local/bin/dreamscreend))
	-systemctl stop dreamscreend
	-systemctl disable dreamscreend
	-rm -f /usr/local/bin/dreamscreend
	-rm -f /usr/lib/systemd/system/dreamscreend.service
	-systemctl daemon-reload
endif

ifneq (,$(wildcard /usr/local/bin/dreamscreen-cli))
	-rm -f /usr/local/bin/dreamscreen-cli
endif

	
