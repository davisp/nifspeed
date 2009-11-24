OTPROOT=/Users/davisp/tmp/otp_src_R13B03
ARCH=i386-apple-darwin10.0.0
INCLUDES = -I$(OTPROOT)/erts/include/$(ARCH) -I$(OTPROOT)/erts/emulator/beam/

# OS X flags.
GCCFLAGS = -O3 -fPIC -bundle -flat_namespace -undefined suppress -fno-common -Wall

# Linux Flags
#GCCFLAGS = -O3 -fPIC -shared -fno-common -Wall

CFLAGS = $(GCCFLAGS) $(INCLUDES)
LDFLAGS = $(GCCFLAGS)

DRIVERS = drvsize.so nifsize.so
BEAM = drvsize.beam nifsize.beam

all: check

clean: 
	rm -f *.o *.beam $(DRIVERS)

%.so: %.o
	gcc $(LDFLAGS) -o $@ $^

%.beam: %.erl
	erlc $^

check: $(BEAM) $(DRIVERS)
	$(OTPROOT)/bin/escript ./run_tests.es
