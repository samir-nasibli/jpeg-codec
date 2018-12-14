#include "image.h"

#include <iostream>

int main( int argc, char* argv[] )
{
    int QualityNumber = 0; 
    std::string nameOfOrig;
    std::string nameOfSavedImg = "inverted";

    if ( argc < 2 )
    {
        std::cout << "No jpeg file specified\n";
        return 1;
    }
    else if(argc < 3)
    {
        std::cout << "No Quality Number specified\n";
        return 1;
    }
    else
    {
        std::vector<std::string> allArgs(argv + 1, argv + 4);
        nameOfOrig = allArgs[0];
        nameOfSavedImg = allArgs[1];
        QualityNumber = std::stoi( allArgs[2] ); 
        //Note that, no error handler here if for third arg 
        //throw an instance of 'std::invalid_argument'
    }
    try
    {
        // Constructor expects a filename to load:
        jpegImage::Image imgOriginal( argv[1] );

        // Print some image info
        std::cout << "\nImage height: " << imgOriginal.getHeight();
        std::cout << "\nImage width : " << imgOriginal.getWidth();
        std::cout << "\nImage px sz : " << imgOriginal.getPixelSize();
        std::cout << std::endl;

        // image invert 
        imgOriginal.invert();

        std::cout<<"Invert succesuflly " <<std::endl;

        // saving image
        imgOriginal.save(nameOfSavedImg, QualityNumber);

        return 0;
    }
    catch(const std::runtime_error& re)
    { 
        std::cout << "runtime Main() error handler: ";
        std::cout << re.what() << std::endl;
        return 1;

    }
}