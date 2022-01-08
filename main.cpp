#include <iostream>
#include "bmp.hpp"




int main(){

    bmp::BMP sample("C:\\PJATK\\PJC\\sample3.bmp");
    sample.getFileMetadata();
    std::cout << sample.fileHeader.fileCode << std::endl;
//    std::cout << "\nTELL2\n" << sample.inputFile.tellg();
    sample.printBITMAPINFOHEADER();
    sample.getBMPPixels();
    std::cout << "\nOffset: " << sample.fileHeader.offset << ' ' << sample.bitmapInfoHeader.biSize;
    sample.inputFile.seekg(0, sample.inputFile.beg);
    std::cout << "VALIDATION: " << sample.isFileBMP() << ' ' << sample.isFile24bpp()
    << '\n' << sample.inputFile.tellg();


    return 0;
}