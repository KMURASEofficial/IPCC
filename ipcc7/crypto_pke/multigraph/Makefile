
CC		= gcc
LDFLAGS=-lcrypto

SOURCES     = keygen.c encrypt.c decrypt.c rng.c invpoly.c 
HEADERS     = keygen.h encrypt.h decrypt.h params.h rng.h invpoly.h
PKE_SOURCES = $(SOURCES) pke.c
KAT_SOURCES = $(SOURCES) kattest.c

all : pke kattest

pke: $(HEADERS) $(PKE_SOURCES)
	$(CC) -o $@ $(PKE_SOURCES) $(LDFLAGS) $(CPPFLAGS)

kattest: $(HEADERS) $(KAT_SOURCES)
	$(CC) -o $@ $(KAT_SOURCES) $(LDFLAGS) $(CPPFLAGS)



clean:
	@rm -rf *.o pke kattest


