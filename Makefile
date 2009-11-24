OTPROOT=/Users/davisp/tmp/otp_src_R13B03
ARCH=i386-apple-darwin10.0.0
INCLUDES = -I$(OTPROOT)/erts/include/$(ARCH) -I$(OTPROOT)/erts/emulator/beam/

# OS X flags.
GCCFLAGS = -O3 -fPIC -bundle -flat_namespace -undefined suppress -fno-common -Wall

# Linux Flags
#GCCFLAGS = -O3 -fPIC -shared -fno-common -Wall

ICULIBS = $(shell icu-config --ldflags-libsonly)
ICULIBDIR = $(shell icu-config --ldflags-searchpath)

CFLAGS = $(GCCFLAGS) $(INCLUDES)
LDFLAGS = $(GCCFLAGS) $(ICULIBDIR) $(ICULIBS)

DRIVERS = couch_uca.so nif_uca.so
BEAM = couch_uca.beam nif_uca.beam

all: check

clean: 
	rm -f *.o *.beam $(DRIVERS)

%.so: %.o
	gcc $(LDFLAGS) -o $@ $^

%.beam: %.erl
	erlc $^

check: $(BEAM) $(DRIVERS)
	$(OTPROOT)/bin/escript ./run_tests.es
