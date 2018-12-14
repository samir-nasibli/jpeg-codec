
# Task

_libJPEG_ is open source C library (Win version is here if you don’t familiar with Linux) which allows to perform JPEG decode and encode capabilities. The library is distributed with console applications cjpeg and djpeg which use _libJPEG_ API and perform decoding JPEG files and encoding input bitmap files into JPEG files.

Implement application which perform the following:
- reads JPEG image file as input;
- performs  decoding JPEG image;
- performs encoding of decoder’s output data to the JPEG format;
- stores encoded JPEG image to the file.

Note: decoder’s output should be used as encoder input and avoid reading data from disk for encoding.