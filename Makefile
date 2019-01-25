all: invert compress

invert: image.h image.cpp invert.cpp 
	g++ -std=c++17 image.cpp image.h invert.cpp -o invert -L/usr/lib/x86_64-linux-gnu/libjpeg.so.* -ljpeg -ltbb -pthread
compress: image.h image.cpp compress.cpp 
	g++ -std=c++17 image.cpp image.h compress.cpp -o compress -L/usr/lib/x86_64-linux-gnu/libjpeg.so.* -ljpeg -ltbb -pthread

clean:
	rm invert compress