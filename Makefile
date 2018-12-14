all: invert compress

invert: image.h image.cpp invert.cpp 
	g++ image.cpp image.h invert.cpp -o invert -ljpeg

compress: image.h image.cpp compress.cpp 
	g++ image.cpp image.h compress.cpp -o compress -ljpeg

clean:
	rm invert compress