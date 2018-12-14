#include "image.h"

#include <iostream>

int main( int argc, char* argv[] )
{
    int QualityNumber = 50; // default
    std::string nameOfOrig;
    std::string nameOfSavedImg = "compressed";

    if ( argc < 2 )
    {
        std::cout << "No jpeg file specified\n";
        return 1;
    }

    else if(argc < 3)
    {
        std::vector<std::string> allArgs(argv + 1, argv + 4);
        nameOfOrig = allArgs[0];
        nameOfSavedImg += nameOfOrig;
    }
    else
    {
        std::vector<std::string> allArgs(argv + 1, argv + 4);
        nameOfOrig = allArgs[0];
        nameOfSavedImg = allArgs[1];
        QualityNumber = std::stoi( allArgs[2] ); 
        //Note that here have no error handler if for third arg 
        //throw an instance of 'std::invalid_argument'
    }
    try
    {
        
        // Constructor expects a filename to load:
        jpegImage::Image imgOriginal(nameOfOrig);

        // Print some image info
        std::cout << "\nImage height: " << imgOriginal.getHeight();
        std::cout << "\nImage width : " << imgOriginal.getWidth();
        std::cout << "\nImage px sz : " << imgOriginal.getPixelSize();
        std::cout << std::endl;

        // saving copy of original image with new quality number
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