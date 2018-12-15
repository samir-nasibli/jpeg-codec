# Simple JPEG encoder/decoder

I implemented application which perform the following:
- reads JPEG image file as input;
- performs  decoding JPEG image;
- performs encoding of decoder’s output data to the JPEG format;
- stores encoded JPEG image to the file.

## Install
You will need to install _libJPEG_ first. 
```
 sudo apt install libjpeg-dev
```
Donwload this repository and run make file. It generates two console programs: ```invert``` and ```compress```.

## Interface
```invert``` -- console program with minimal interface, which reads __JPEG__ file, decodes JPEG image, compress with given number of quality and stores JPEG image to the file. 

```compress``` -- console program with minimal interface, which reads __JPEG__ file, decodes JPEG image, inverts image pixels, compress with given number of quality and stores JPEG image to the file.

* __invert [name of original jpeg file] [name of output jpeg file] [quality number - int]__
* __compress [name of original jpeg file] [name of output jpeg file] [quality number - int]__
* NOTE: not all exceptions are handled, you need to specify all parameters correct

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



## To-dos
* Write Tests
* Handle exceptions
* Update ReadMe

## Reference
* [apodeline.free.fr/DOC/libjpeg](http://apodeline.free.fr/DOC/libjpeg/libjpeg-1.html) - Using the IJG JPEG library
* [jpegclub.org/reference](https://jpegclub.org/reference/reference-sources/) - Independent JPEG Group Reference Site
* [src/libjpeg.doc]() - USING THE IJG JPEG LIBRARY
