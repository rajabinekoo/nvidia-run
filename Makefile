CC ?= cc
INSTALL ?= install
RM ?= rm -f
STRIP ?= strip

TARGET := run-nvidia
SOURCE := run-nvidia.c
MANPAGE := run-nvidia.1
VERSION ?= $(shell cat VERSION)

PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin
MANDIR ?= $(PREFIX)/share/man
MAN1DIR ?= $(MANDIR)/man1
DESTDIR ?=

CPPFLAGS ?=
CPPFLAGS += -DRUN_NVIDIA_VERSION=\"$(VERSION)\"

CFLAGS ?= -O2
CFLAGS += -std=c11 -Wall -Wextra -Wpedantic -Wshadow -Wconversion

LDFLAGS ?=

.PHONY: all build install install-strip uninstall clean check info strip release

all: build

build: $(TARGET)

$(TARGET): $(SOURCE) VERSION
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SOURCE) $(LDFLAGS) -o $(TARGET)

install: $(TARGET)
	$(INSTALL) -d "$(DESTDIR)$(BINDIR)"
	$(INSTALL) -m 0755 $(TARGET) "$(DESTDIR)$(BINDIR)/$(TARGET)"
	$(INSTALL) -d "$(DESTDIR)$(MAN1DIR)"
	$(INSTALL) -m 0644 $(MANPAGE) "$(DESTDIR)$(MAN1DIR)/$(MANPAGE)"

install-strip: strip install

uninstall:
	$(RM) "$(DESTDIR)$(BINDIR)/$(TARGET)"
	$(RM) "$(DESTDIR)$(MAN1DIR)/$(MANPAGE)"

clean:
	$(RM) $(TARGET)

check: $(TARGET)
	./$(TARGET) --version
	./$(TARGET) --help >/dev/null
	./$(TARGET) sh -c 'test "$$__NV_PRIME_RENDER_OFFLOAD" = "1" && test "$$__GLX_VENDOR_LIBRARY_NAME" = "nvidia" && test "$$__VK_LAYER_NV_optimus" = "NVIDIA_only"'
	@./$(TARGET) __run_nvidia_missing_command_for_test__ >/dev/null 2>&1; status=$$?; test $$status -eq 127

info: $(TARGET)
	./$(TARGET) glxinfo -B

strip: $(TARGET)
	$(STRIP) $(TARGET)

release: clean build check strip
