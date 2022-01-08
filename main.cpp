#include <iostream>
#include "bmp.hpp"





int main(){

    bmp::BMP sample("C:\\PJATK\\PJC\\sample10.bmp");
    sample.getFileMetadata();
    sample.setBMPPixelsBinary();
    sample.printBITMAPINFOHEADER();
    std::cout << "\nFILE HEADER\n" << sample.fileHeader.reserved1
    << ' ' << sample.fileHeader.reserved2 << '\n'
    << ' ' << sample.fileHeader.fileCode << ' '
    << sample.fileHeader.fileSize;
    std::cout << '\n' << '\n' << sample.hasImageHasMessage() << '\n';
    int x = sample.getCodedMessageLength();
    std::cout << "LENGTH: " << x << '\n';
//    sample.codeMessageToImage("XD");
//    sample.setDoesFileHaveMessageFlag();
//    sample.writeImage("C:\\PJATK\\PJC\\sample10.bmp");



    return 0;
}