all: invert compress

invert: image.h image.cpp invert.cpp 
	g++ -std=c++11 image.cpp image.h invert.cpp -o invert -ljpeg -ltbb

compress: image.h image.cpp compress.cpp 
	g++ -std=c++11 image.cpp image.h compress.cpp -o compress -ljpeg -ltbb

clean:
	rm invert compress