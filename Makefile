CFLAGS=-Wall -g
LIBS=-pthread

m: m.o bt.o critnib.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f m *.o
