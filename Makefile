#Default rule
TARGETS := libtetracrypto.a tests gen_ks
all: $(TARGETS)

CFLAGS := -Wall -O3 -g

#Compiler
CC 	= gcc
LD	= $(CC)

%.o:	%.c
	$(CC) $(CFLAGS) -c $< -o $@

libtetracrypto.a: hurdle.o tea1.o tea2.o tea3.o taa1.o common.o
	ar rcs $@ $^

tests: libtetracrypto.a tests.o
	$(LD) -o $@ tests.o -ltetracrypto -L.

gen_ks: libtetracrypto.a gen_ks.o
	$(LD) -o $@ gen_ks.o -ltetracrypto -L.

clean:
	rm -f *.o *.a $(TARGETS)
