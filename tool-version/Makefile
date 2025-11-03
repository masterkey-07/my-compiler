# Makefile to build flex lexers: each *.l -> corresponding executable
FLEX ?= flex
CC   ?= cc
CFLAGS ?= -O2 -Wall -Wextra
LDFLAGS ?=

LEX_SRCS := $(wildcard *.l)
C_SRCS   := $(patsubst %.l,%.c,$(LEX_SRCS))
BINS     := $(patsubst %.l,%,$(LEX_SRCS))

.PHONY: all clean distclean help

all: $(BINS)

# generate C source from .l using flex
%.c: %.l
	$(FLEX) -o $@ $<

# compile generated C into executable
%: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

clean:
	-rm -f $(C_SRCS)

distclean: clean
	-rm -f $(BINS)

help:
	@printf "Targets:\n"
	@printf "  all       - build all executables from .l files\n"
	@printf "  clean     - remove generated .c files\n"
	@printf "  distclean - remove generated .c files and binaries\n"