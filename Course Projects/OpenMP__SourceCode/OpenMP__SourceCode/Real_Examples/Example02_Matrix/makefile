CC=g++

CFLAGS=-c -fopenmp

LNKFLAGS=-fopenmp

INCDIR=-I/opt/intel/composer_xe_2015.0.077/ipp/include

LIBDIR=-L/opt/intel/composer_xe_2015.0.077/ipp/lib

LIBFILE=-lippcore -lippcore-8.2

all: main

main: main.o 
	$(CC) $(LNKFLAGS) $(LIBDIR) $(LIBFILE) main.o -o main

main.o: main.cpp
	$(CC) $(CFLAGS) $(INCDIR) main.cpp

clean:
	rm -f *o main