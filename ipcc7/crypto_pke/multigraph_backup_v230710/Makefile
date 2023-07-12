
CC		= gcc
LDFLAGS=-lcrypto
LDFLAGS+="-L/usr/local/opt/openssl@3/lib"
CPPFLAGS="-I/usr/local/opt/openssl@3/include"

SOURCES=  pke.c keygen.c encrypt.c decrypt.c rng.c invpoly.c 
HEADERS=  keygen.h encrypt.h decrypt.h params.h rng.h invpoly.h


pke: $(HEADERS) $(SOURCES)
	$(CC) -o $@ $(SOURCES) $(LDFLAGS) $(CPPFLAGS)

clean:
	@rm -rf *.o pke
