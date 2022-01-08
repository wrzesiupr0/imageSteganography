#ifndef IMAGESTEGANOGRAPHY_PNG_HPP
#define IMAGESTEGANOGRAPHY_PNG_HPP

#include <iostream>
#include <fstream>
#include <cstdint>
#include <algorithm>
#include <string>
#include <vector>
#include <bitset>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <winsock.h>

namespace png {

    static bool isFilePNGExtension (const std::string& path) {
        std::string extension = path.substr(path.find_last_of('.') + 1);
        std::for_each(extension.begin(), extension.end(), [](char &c) {
            c = std::tolower(c);
        });
        return extension == "png";
    }

    struct PNG {
        std::string path;
        std::ifstream inputFile;
        std::vector<uint8_t> fileData;
        uint32_t width;
        uint32_t height;
        int fileSize;


        explicit PNG(const std::string& path) {
            if (!isFilePNGExtension(path)) {
                std::cerr << "File extension is not png!";
                exit(10);
            } else {
                this->path = path;
                inputFile = std::ifstream(path, std::ios::in | std::ios::binary | std::ios::ate);
                if (!inputFile) {
                    std::cerr << "Cannot open file!\n";
                    exit(-1);
                }
                fileSize = inputFile.tellg();
                fileData.resize(fileSize);
                inputFile.seekg(0, std::ios::beg);
                inputFile.read((char * ) fileData.data(), fileData.size());
                inputFile.seekg(16, std::ios::beg);
                std::uint8_t dimensions[8];
                inputFile.read(reinterpret_cast<char *>( &dimensions), 8);
                width = (dimensions[0] << 24) + (dimensions[1] << 16) + (dimensions[2] << 8) + dimensions[3];
                height = (dimensions[4] << 24) + (dimensions[5] << 16) + (dimensions[6] << 8) + dimensions[7];
            }
        }

        void printFileInfo() {
            const char* pathChar = path.c_str();
            std::string fileName;
            size_t i = path.rfind('\\', path.length());
            fileName = path.substr(i+1, path.length() - i);
            std::cout << "File name:\t\t" << fileName
                      << "\nFile disk size:\t\t" << fileSize << " B"
                      << "\nImage dimensions\t" << width << " x " << height
                      << "\nCreated:\t\t" << app::getFileCreationTime(pathChar)
                      << "Last modify:\t\t" << app::getFileLastModifyTime(pathChar);
        }

        bool doesImageHaveMessage() {
            std::vector<std::uint8_t> endFilePattern{0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82};
            auto res = std::search(std::begin(fileData), std::end(fileData), std::begin(endFilePattern),
                                   std::end(endFilePattern));
            if (res+8 == std::end(fileData))
                return false;
            return true;
        }

        std::string decodeMessageFromImage() {
            std::vector<std::uint8_t> endFilePattern{0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82};
            auto res = std::search(std::begin(fileData), std::end(fileData), std::begin(endFilePattern),
                                   std::end(endFilePattern));
            using Iter = std::vector<std::uint8_t>::const_iterator;
            std::stringstream ss;
            for (Iter it = res + 8; it != fileData.end(); it++) {
                ss << *it;
            }
            return ss.str();
        }

        void codeMessageToImage(const std::string &msg) {
            for (unsigned char c : msg) {
                fileData.push_back(c);
            }
        }

        void writeImage(const std::string& pathOut) {
            inputFile.close();
            std::ofstream outputFile(pathOut, std::ios::out | std::ios::binary);
            if (outputFile) {
                outputFile.write((char * ) fileData.data(), fileData.size());
                outputFile.close();
            } else {
                std::cerr << "Unable to open output file!";
                exit(12);
            }
        }
    };
}

#endif //IMAGESTEGANOGRAPHY_PNG_HPP
