OBJS := universe.o lib.o evolve_simple.o makegif.o evolve_bitwise.o readfile.o findstill.o \
	writefile.o textconv.o

CFLAGS += -O3
#CFLAGS += -ggdb
#LDFLAGS += -pg

all: test_universe test_readwrite test_stabilise mkstill bellman envelope

test_%: $(OBJS) test_%.o
	$(CC) $(LDFLAGS) -o $@ $^ -lgd

mkstill: $(OBJS) mkstill.o
	$(CC) $(LDFLAGS) -o $@ $^ -lgd

bellman: $(OBJS) bellman.o
	$(CC) $(LDFLAGS) -o $@ $^ -lgd

envelope: $(OBJS) envelope.o
	$(CC) $(LDFLAGS) -o $@ $^ -lgd
clean:
	rm -f *.o mkstill bellman envelope test_universe test_readwrite test_stabilise
