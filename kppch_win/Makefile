# This should work on Linux.  Modify as needed for other platforms.

BOINC_DIR = ../..
BOINC_API_DIR = $(BOINC_DIR)/api
BOINC_LIB_DIR = $(BOINC_DIR)/lib

CXXFLAGS = -g \
    -static -Wall \
# -W -Wshadow -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -fno-common \
    -DAPP_GRAPHICS \
    -I $(BOINC_DIR) \
    -I $(BOINC_LIB_DIR) \
    -I $(BOINC_API_DIR) \
    -I ./h \
    -I ./ \
    -L /usr/X11R6/lib \
    -L.

# the following should be freeglut; use nm to check
# you may have to change the paths for your system
#CC='gcc -m32'
#CXX='g++ -m32'
CXX=i586-mingw32msvc-g++
ifeq ($(wildcard /usr/local/lib/libglut.a),)
	LIBGLUT = /usr/lib/libglut.a
	LIBGLU = /usr/lib/libGLU.a
	LIBJPEG = /usr/lib/libjpeg.a
else
	LIBGLUT = /usr/local/lib/libglut.a
	LIBGLU = /usr/local/lib/libGLU.a
	LIBJPEG = /usr/local/lib/libjpeg.a
endif

PROGS = primes \
# primes_graphics
# make this optional so compile doesn't break on systems without OpenGL

all: $(PROGS)

libstdc++.a:
	ln -s `g++ -print-file-name=libstdc++.a`

clean:
	/bin/rm -f $(PROGS) *.o libstdc++.a

distclean:
	/bin/rm -f $(PROGS) *.o libstdc++.a

install: primes

# specify library paths explicitly (rather than -l)
# because otherwise you might get a version in /usr/lib etc.

primes: primes.o libstdc++.a $(BOINC_API_DIR)/libboinc_api.a $(BOINC_LIB_DIR)/libboinc.a
	$(CXX) $(CXXFLAGS)  -lprimesieve -s -static -o primes.exe primes.o   \
	$(BOINC_API_DIR)/libboinc_api.a \
	$(BOINC_LIB_DIR)/libboinc.a

