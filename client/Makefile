#
# Makefile for New Relic C SDK Code Examples
#

# This Makefile presumes that libnewrelic.h and libnewrelic.a are at the following
# locations:

LIBNEWRELIC_A_LOCATION = ..
LIBNEWRELIC_H_LOCATION = ../include

# Pull in PCRE config.
PCRE_CFLAGS := $(shell pcre-config --cflags)
PCRE_LIBS := $(shell pcre-config --libs)

# Librarys
LDLIBS = -lcurl

EXAMPLES := client.out

all: $(EXAMPLES)

client.out: client.o newrelic_helper.o ../libnewrelic.a
	$(CC) -o $@ $< newrelic_helper.o -rdynamic -L$(LIBNEWRELIC_A_LOCATION) -lnewrelic $(PCRE_LIBS) $(LDLIBS) -pthread -lm

%.o: %.c
	$(CC) -c $< -o $@ -I$(LIBNEWRELIC_H_LOCATION) $(PCRE_CFLAGS) $(CFLAGS)

clean:
	rm -f *.o $(EXAMPLES) c_sdk.log
