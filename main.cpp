#include <iostream>
#include "bmp.hpp"
#include "png.hpp"
#include <fstream>
#include <iomanip>
#include "sys/types.h"
#include "app.hpp"
#include <sys/stat.h>




int main(int argc, char *argv[]){
    std::cout << '\n';

    if (argc > 4) {
        std::cerr << "Too many arguments! Run program with flag -h or --help to get more information.\n";
        exit(-1);
    }

    if (argc == 1 || (argc == 2 && (argv[1] == std::string("-h") || argv[1] == std::string("--help")))) {
        app::printHelpInfo();
    }

    else if (argc == 3 && (argv[1] == std::string("-i") || argv[1] == std::string("--info"))) {
        std::string path = argv[2];
        if (bmp::isFileBMPExtension(path)) {
            bmp::BMP bmpFile(path);
            bmpFile.printFileInfo();
            bmpFile.inputFile.close();
        } else if (png::isFilePNGExtension(path)) {
            png::PNG pngFile(path);
            pngFile.printFileInfo();
            pngFile.inputFile.close();
        } else {
            std::cerr << "Wrong file! Check if file is .png or .bmp format\n";
            exit(-1);
        }
    }

    else if (argc == 4 && (argv[1] == std::string("-e") || argv[1] == std::string("--encrypt"))) {
        std::string path = argv[2];
        std::string message = argv[3];
        if (bmp::isFileBMPExtension(path)) {
            bmp::BMP bmpFile(path);
            if (!bmpFile.isFileRealBMP()) {
                std::cerr << "Given file is not real bitmap!\n";
                exit(-1);
            }
            if (!bmpFile.isFile24bpp()) {
                std::cerr << "Given bitmap file is not in 24 bpp format!\n";
                exit(-1);
            }
            if (!bmpFile.canMessageBeCoded(message)) {
                std::cerr << "Given message is too long for this image file!\n";
                exit(-1);
            }
            bmpFile.codeMessageToImage(message);
            bmpFile.writeImage("C:\\PJATK\\PJC\\xd.bmp");
        } else if (png::isFilePNGExtension(path)) {
            png::PNG pngFile(path);
            pngFile.codeMessageToImage(message);
            pngFile.writeImage("C:\\PJATK\\PJC\\xd.png");
        } else {
            std::cerr << "Wrong file! Check if file is .png or .bmp format\n";
            exit(-1);
        }
    }

    else if (argc == 3 && (argv[1] == std::string("-d") || argv[1] == std::string("--decrypt"))) {
        std::string path = argv[2];
        if (bmp::isFileBMPExtension(path)) {
            bmp::BMP bmpFile(path);
            if (!bmpFile.isFileRealBMP()) {
                std::cerr << "Given file is not real bitmap!\n";
                exit(-1);
            }
            if (!bmpFile.isFile24bpp()) {
                std::cerr << "Given bitmap file is not in 24 bpp format!\n";
                exit(-1);
            }
            if (!bmpFile.doesImageHaveMessage()) {
                std::cerr << "Given image file does not contain any message\n";
                exit(-1);
            }
            std::cout << "Decrypted message:\n\n\t" << bmpFile.decodeMessageFromImage() << '\n';
        } else if (png::isFilePNGExtension(path)) {
            png::PNG pngFile(path);
            if (!pngFile.doesImageHaveMessage()) {
                std::cerr << "Given image file does not contain any message\n";
                exit(-1);
            }
            std::cout << "Decrypted message:\n\n\t" << pngFile.decodeMessageFromImage() << '\n';
        } else {
            std::cerr << "Wrong file! Check if file is .png or .bmp format\n";
            exit(-1);
        }
    }
    else if (argc == 4 && (argv[1] == std::string("-c") || argv[1] == std::string("--check"))) {
        std::string path = argv[2];
        std::string message = argv[3];
        if (bmp::isFileBMPExtension(path)) {
            bmp::BMP bmpFile(path);
            if (!bmpFile.isFileRealBMP()) {
                std::cerr << "Given file is not real bitmap!\n";
                exit(-1);
            }
            if (!bmpFile.isFile24bpp()) {
                std::cerr << "Given bitmap file is not in 24 bpp format!\n";
                exit(-1);
            }
            if (!bmpFile.canMessageBeCoded(message)) {
                std::cerr << "Given message is too long for this file!\n";
                exit(-1);
            }
            std::cout << "Given message can be encrypted in this file!\n";
        } else if (png::isFilePNGExtension(path)) {
            png::PNG pngFile(path);
            std::cout << "Given message can be encrypted in this file!\n";
        } else {
            std::cerr << "Wrong file! Check if file is .png or .bmp format\n";
            exit(-1);
        }
    }
    else {
        std::cerr << "Invalid parameters! Run program with flag -h or --help to get more information.\n";
        exit(-1);
    }
    return 0;
}

