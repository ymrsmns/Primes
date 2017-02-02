CC=g++ -O2 -Wall -std=gnu++0x
# --enable-static
CFLAGS=-c -Wall 
#g++ -O2 -g kpppch_16_do_33.cpp -lprimesieve -c -Wall -std=gnu++0x -o func
LDFLAGS=-static -I. -L.  -lprimesieve -lpthread -lm -lgomp

SOURCES=kpppch_16_do_33.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=func
binaries=func
all: clean $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
.PHONY: clean
clean:
	rm -f $(binaries)
