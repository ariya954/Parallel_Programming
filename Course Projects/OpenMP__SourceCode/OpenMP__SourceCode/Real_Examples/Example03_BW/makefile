CC=g++

CFLAGS=-c -fopenmp

LNKFLAGS=-fopenmp

INCDIR=-I/opt/intel/composer_xe_2015.0.077/ipp/include -I/usr/local/Cellar/opencv3/3.3.0_3/include

LIBDIR=-L/opt/intel/composer_xe_2015.0.077/ipp/lib -L/usr/local/Cellar/opencv3/3.3.0_3/lib -L/usr/local/opt/jpeg/lib/

LIBFILE=-lippcore -lippcore-8.2 -lopencv_ccalib -lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_stitching -lopencv_superres -lopencv_video -lopencv_videostab -lopencv_imgcodecs -ljpeg.9

all: main

main: main.o 
	$(CC) $(LNKFLAGS) $(LIBDIR) $(LIBFILE) main.o -o main

main.o: main.cpp
	$(CC) $(CFLAGS) $(INCDIR) main.cpp

clean:
	rm -f *o main