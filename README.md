# Simple JPEG encoder/decoder

I implemented application which perform the following:
- reads JPEG image file as input;
- performs  decoding JPEG image;
- performs encoding of decoder’s output data to the JPEG format;
- stores encoded JPEG image to the file.

## Install
You will need to install [libJPEG](http://libjpeg.sourceforge.net/) first. 
```
 sudo apt install libjpeg-dev
```
Also you need to install [Intel TBB](https://software.intel.com/en-us/get-started-with-tbb) library for parallelism.

Donwload this repository and run make file. It generates two console programs: ```invert``` and ```compress```.

## Interface
```compress``` -- console program with minimal interface, which reads __JPEG__ file, decodes JPEG image, compress with given number of quality and stores JPEG image to the file. 

```invert``` -- console program with minimal interface, which reads __JPEG__ file, decodes JPEG image, inverts image pixels, compress with given number of quality and stores JPEG image to the file.

* __invert [name of original jpeg file] [name of output jpeg file] [quality number - int]__
* __compress [name of original jpeg file] [name of output jpeg file] [quality number - int]__
* NOTE: you need to specify all parameters correct

Examples:
```
invert cars.jpeg invertCars.jpeg 34
```
```
compress cars.jpeg comprCars.jpeg 24
```

Original image            |  Inverted image
:-------------------------:|:-------------------------:
![](cars.jpeg)  |  ![](invertCars.jpeg)


## Updates
__27.12.2018__:
* now class ```Image``` uses one single vector instead of vector of vectors for bit map image;
* ```parallelInvert()``` method was added in the class Image. It does the same as ```invert()```, only twice (1.5-2.0 times) as fast on average. This result is achieved through the use TBB library functions and template classes. (full description will be added)
* some exceptions are handled (full description will be added later)
* other (full description will be added later) 

__25.01.2019__:
* ```stdInvert()``` was added, that uses std::threads for multithreading.

## To-dos
* Write Google Tests
* Handle exceptions
* Update ReadMe

## Reference
* [apodeline.free.fr/DOC/libjpeg](http://apodeline.free.fr/DOC/libjpeg/libjpeg-1.html) - Using the IJG JPEG library
* [jpegclub.org/reference](https://jpegclub.org/reference/reference-sources/) - Independent JPEG Group Reference Site
* [src/libjpeg.doc]() - USING THE IJG JPEG LIBRARY
* [https://www.threadingbuildingblocks.org/docs/](https://www.threadingbuildingblocks.org/docs/) - Threading Building Blocks (TBB)
* [https://software.intel.com/en-us/get-started-with-tbb](https://software.intel.com/en-us/get-started-with-tbb) - Getting Started with Intel® Threading Building Blocks (Intel® TBB)