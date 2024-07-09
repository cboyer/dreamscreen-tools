CFLAGS = -Wall -Wextra -Werror -O2 -I./lib
OS = $(shell uname)
BUILD_DIR = build
ALL_TARGETS = dreamscreen-cli dreamscreend

ifdef DEBUG
	CFLAGS := $(CFLAGS) -DDEBUG
endif

ifeq ($(OS),Linux)
	CC = gcc
endif

ifeq ($(OS),FreeBSD)
	CC = clang
	ALL_TARGETS = dreamscreen-cli
endif

.PHONY: all clean install uninstall cli daemon

all: $(ALL_TARGETS)
cli: dreamscreen-cli
daemon: dreamscreend

build:
	@mkdir -p $(BUILD_DIR)

%: dreamscreen-cli/%.c build
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

%: dreamscreend/%.c build
	$(CC) $(CFLAGS) $< -o $(BUILD_DIR)/$@

clean:
	$(RM) -r $(BUILD_DIR)

install:
	install -m 755 $(BUILD_DIR)/dreamscreen-cli /usr/local/bin/
	if test -f $(BUILD_DIR)/dreamscreend ; then install -m 755 $(BUILD_DIR)/dreamscreend /usr/local/bin/ ; fi
	if test -f $(BUILD_DIR)/dreamscreend ; then install -m 744 $(BUILD_DIR)/dreamscreend.service /usr/lib/systemd/system/ ; fi
	if test -f $(BUILD_DIR)/dreamscreend ; then systemctl daemon-reload ; fi

uninstall:
	if test -f /usr/lib/systemd/system/dreamscreend.service ; then systemctl stop dreamscreend && systemctl disable dreamscreend && $(RM) /usr/lib/systemd/system/dreamscreend.service && systemctl daemon-reload ; fi
	$(RM) /usr/local/bin/dreamscreen-cli /usr/local/bin/dreamscreend

