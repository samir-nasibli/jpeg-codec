all: invert compress

invert: image.h image.cpp invert.cpp 
	g++ -std=c++17 image.cpp image.h invert.cpp -o invert -ljpeg -ltbb -pthread
compress: image.h image.cpp compress.cpp 
	g++ -std=c++17 image.cpp image.h compress.cpp -o compress -ljpeg -ltbb -pthread

clean:
	rm invert compress