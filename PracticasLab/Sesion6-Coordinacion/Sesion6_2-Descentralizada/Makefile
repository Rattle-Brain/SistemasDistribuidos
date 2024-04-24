CFLAGS=-Wall
LDFLAGS=-pthread

OBJETIVOS=filodist2
all: $(OBJETIVOS)

%: %.o
    $(CC) -o $@ $< $(LDFLAGS)
%: %.c
    $(CC) -o $@ $(CFLAGS) $< $(LDFLAGS)
clean:
    -rm *~ *.o

cleanall: clean
    -rm $(OBJETIVOS)