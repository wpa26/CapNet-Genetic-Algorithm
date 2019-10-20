CCFLAGS = -Wall -Wshadow -O2
LFLAGS = -lm

.PHONY = all clean

all: cap

cap: source.o tinyexpr.o
	$(CC) $(CCFLAGS) -o $@ $^ $(LFLAGS)
	
clean:
	rm -f *.o *.exe cap test
