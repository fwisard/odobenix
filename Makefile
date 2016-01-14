# cw Makefile, by HB9FXW - Based on qrq Makefile by DJ1YFK

VERSION=1.0.1
DESTDIR?=/usr

# set to YES if you want to use PulseAudio instead of OSS
USE_PA=YES


CFLAGS:=$(CFLAGS) -D DESTDIR=\"$(DESTDIR)\" -D VERSION=\"$(VERSION)\" -I.
CC=gcc

ifeq ($(USE_PA), YES)
		CFLAGS:=$(CFLAGS) -D PA 
		LDFLAGS:=$(LDFLAGS) -lpulse-simple -lpulse 
		OBJECTS=cw.o pulseaudio.o
else
		OBJECTS=cw.o oss.o
		LDFLAGS:=$(LDFLAGS)
		CFLAGS:=$(CFLAGS) -D OSS
endif	

all: cw

cw: $(OBJECTS)
	$(CC) -Wall -o $@ $^ -lm $(LDFLAGS)
	
.c.o:
	$(CC) -Wall $(CFLAGS) -c $<

clean:
	rm -f cw *~ *.o

