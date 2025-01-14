BUILDLOCATION=../build
FILENAME=Debian_Server
OUTPUTNAME=$(BUILDLOCATION)/$(FILENAME)

IDIR =../include
CC=gcc
CFLAGS=-g -I$(IDIR)

ODIR=../build
LDIR =../lib

LIBS=-lm -z noexecstack

_DEPS = server.h inputHandler.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
_OBJ = server.o inputHandler.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


$(OUTPUTNAME): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 

cleanall:
	rm -f $(ODIR)/*