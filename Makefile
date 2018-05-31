CC=gcc
CFLAGS=-Wall -Werror -O2

default: dreamscreend
debug: CFLAGS += -DDEBUG
debug: dreamscreend

dreamscreend.o: dreamscreend.c
	$(CC) $(CFLAGS) -c dreamscreend.c -o dreamscreend.o

dreamscreend: dreamscreend.o
	$(CC) $(CFLAGS) dreamscreend.o -o dreamscreend

clean:
	-rm -f dreamscreend.o
	-rm -f dreamscreend

install:
	-install -m 755 dreamscreend /usr/local/bin/
	-install -m 744 dreamscreen.service /usr/lib/systemd/system/
	-systemctl daemon-reload

uninstall:
	-systemctl stop dreamscreen
	-systemctl disable dreamscreen
	-rm -f /usr/local/bin/dreamscreend
	-rm -f /usr/lib/systemd/system/dreamscreen.service
	-systemctl daemon-reload
