SRCDIR := src
BUILDDIR := build
BINDIR := bin
EXEC := cminus

CC := gcc
YACC := bison
LEX := flex

CFLAGS := -g -O2 -Wall -Wextra -I$(SRCDIR) -I$(BUILDDIR)
LDFLAGS := -lfl

OBJS := $(BUILDDIR)/parser.tab.o \
        $(BUILDDIR)/scanner.o \
        $(BUILDDIR)/semantic.o \
        $(BUILDDIR)/symbol.o \
        $(BUILDDIR)/main.o

.PHONY: all clean rebuild

all: $(BINDIR)/$(EXEC)

# final executable in bin/
$(BINDIR)/$(EXEC): $(OBJS) | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# ensure directories exist
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

# generate parser sources (parser.tab.c and parser.tab.h) into build/
$(BUILDDIR)/parser.tab.c $(BUILDDIR)/parser.tab.h: $(SRCDIR)/parser.y | $(BUILDDIR)
	$(YACC) -d -o $(BUILDDIR)/parser.tab.c $(SRCDIR)/parser.y

# generate scanner.c into build/ (depends on generated header)
$(BUILDDIR)/scanner.c: $(SRCDIR)/scanner.l $(BUILDDIR)/parser.tab.h | $(BUILDDIR)
	$(LEX) -o $(BUILDDIR)/scanner.c $(SRCDIR)/scanner.l

# compile generated C files in build/ to build/*.o
$(BUILDDIR)/%.o: $(BUILDDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# compile project C sources from src/ to build/*.o
$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILDDIR) $(BINDIR)

rebuild: clean all