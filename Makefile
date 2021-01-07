CC=gcc
CFLAGS=-Wall
DEPS=
OBJDIR=./obj
INCLUDE=./include
LIBS=./lib
LIB=

DEPS = $(patsubst %,$(IDIR)/%)

OBJ = $(patsubst %,$(OBJDIR)/%)

$(OBJDIR)/%.o: %.c $(DEPS)
	$CC -c -o $@ $< $(CFLAGS)

make: $(OBJ)
	$(CC) -o $@ $< $(CFLAGS) $(LIB)

.PHONY: clean

clean:
	rm -f $(OBJDIR)/*.o *~ core $(INCDIR)/*~
