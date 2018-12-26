#include "image.h"

#include <iostream>

void printTemplateForCorrectRequest()
{
    std::cout << "Please, specify the correct request parameters shown below\n" 
        << "[name of original jpeg file] [name of output jpeg file] [quality number - integer number between 0 and 100]" << std::endl;

}

int main( int argc, char* argv[] )
{
    int QualityNumber = 50; // default
    std::string nameOfOrig;
    std::string nameOfSavedImg;

    if (argc != 4)
    {
        printTemplateForCorrectRequest();
        return 1;
    }
    else
    {
        std::vector<std::string> allArgs(argv + 1, argv + 4);
        nameOfOrig = allArgs[0];
        nameOfSavedImg = allArgs[1];
        try
        {
            QualityNumber = std::stoi( allArgs[2] ); 
        }
        catch(std::invalid_argument& e){
            // if no conversion could be performed
            std::cout << "No conversion could be performed\n";
            printTemplateForCorrectRequest();
            return 1;
        }
        catch(std::out_of_range& e){
            // if the converted value would fall out of the range of the result type 
            std::cout << "The converted value would fall out of the range of the result type\n";
            printTemplateForCorrectRequest();
            return 1;
        }
    }
    try
    {
        // Constructor expects a filename to load:
        jpegImage::Image imgOriginal(nameOfOrig);

        // Print some image info
        imgOriginal.printImageProperties();

        // Save the image with new quality number
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