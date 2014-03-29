CC=gcc
CFLAGS=-g

ODIR=obj

LIBS=-lcurl

_OBJ = dh-cli.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

dh-cli: $(OBJ)
	gcc -o $@ $< $(LIBS)

$(ODIR)/%.o: %.c
	mkdir -p $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f dh-cli core $(ODIR)/*.o *~
	rmdir $(ODIR)
