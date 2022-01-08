#ifndef IMAGESTEGANOGRAPHY_BMP_HPP
#define IMAGESTEGANOGRAPHY_BMP_HPP

#include <iostream>
#include <fstream>
#include <cstdint>
#include <algorithm>
#include <string>
#include <vector>
#include <bitset>
#include <sstream>
#include "app.hpp"

namespace bmp {
#pragma pack(push, 1)
    struct BITMAPFILEHEADER {
        std::uint16_t fileCode;
        std::uint32_t fileSize;
        std::uint16_t reserved1;
        std::uint16_t reserved2;
        std::uint32_t offset;
    };

    struct BITMAPINFOHEADER {
        std::uint32_t biSize;
        std::uint32_t biWidth;
        std::uint32_t biHeight;
        std::uint16_t biPlanes;
        std::uint16_t biBitCount;
        std::uint32_t biCompression;
        std::uint32_t biSizeImage;
        std::uint32_t biXPelsPerMeter;
        std::uint32_t biYPelsPerMeter;
        std::uint32_t biClrUsed;
        std::uint8_t biClrImportant;
        std::uint8_t biClrRotation;
        std::uint16_t biReserved;
    };

#pragma pack(pop)

    bool isFileBMPExtension (const std::string& path) {
        std::string extension = path.substr(path.find_last_of('.') + 1);
        std::for_each(extension.begin(), extension.end(), [](char &c) {
            c = std::tolower(c);
        });
        return extension == "bmp";
    }

    struct BMP {
        std::string path;
        std::ifstream inputFile;
        BITMAPFILEHEADER fileHeader{};
        BITMAPINFOHEADER bitmapInfoHeader{};
        std::vector<std::bitset<8>> pixels;

        explicit BMP(const std::string& path) {
            if (!isFileBMPExtension(path)) {
                std::cerr << "File extension is not bmp!";
                exit(10);
            } else {
                inputFile = std::ifstream(path, std::ios::in | std::ios::binary);
                if (inputFile) {
                    this->path = path;
                    getFileMetadata();
                    setBMPPixelsBinary();
                } else {
                    std::cerr << "Cannot open file!\n";
                    exit(-1);
                }
            }
        }

        void printFileInfo() {
            const char* pathChar = path.c_str();
            std::string fileName;
            size_t i = path.rfind('\\', path.length());
            fileName = path.substr(i+1, path.length() - i);
            std::cout << "\nFile name:\t\t" << fileName
                        << "\nFile disk size:\t\t" << fileHeader.fileSize << " B"
                        << "\nImage dimensions\t" << bitmapInfoHeader.biWidth << " x " << bitmapInfoHeader.biHeight
                        << "\nCreated:\t\t" << app::getFileCreationTime(pathChar)
                        << "Last modify:\t\t" << app::getFileLastModifyTime(pathChar);
        }

        void getFileMetadata() {
            inputFile.read((char *) &fileHeader, sizeof(fileHeader));
            inputFile.read((char *) &bitmapInfoHeader, sizeof(bitmapInfoHeader));
            if (!isFileRealBMP()) {
                std::cerr << "BMP file is invalid!";
                exit(11);
            }
        }

        unsigned int getNumberOfAvailableBits () {
            return bitmapInfoHeader.biWidth * bitmapInfoHeader.biHeight - 16;
        }

        std::vector<std::bitset<8>> getBinaryOfString(const std::string& msg) {
            std::vector<std::bitset<8>> result;
            for (std::size_t i = 0; i < msg.size(); ++i)
            {
                result.push_back(std::bitset<8>(msg.c_str()[i]));
            }
            return result;
        }

        bool canMessageBeCoded(const std::string& msg) {
            return msg.size() * 8 <= getNumberOfAvailableBits();
        }

        void setDoesFileHaveMessageFlag() {
            fileHeader.reserved1 = 2137;
            fileHeader.reserved2 = 7312;
        }

        void codeMessageLength(const std::string& msg) {
            std::bitset<16> msgSizeBits(msg.size());
            for (int i = 0; i < 16; i++) {
                pixels[i*3][0] = msgSizeBits[i];
            }
        }

        int decodeMessageLength() {
            std::bitset<16> msgSizeBits;
            for (int i = 0; i < 16; i++) {
                msgSizeBits[i] = pixels[i*3][0];
            }
            int msgLength;
            msgLength = (int)(msgSizeBits.to_ulong());
            return msgLength;
        }
        void codeMessageToImage(const std::string& msg) {
            codeMessageLength(msg);
            std::vector<std::bitset<8>> binaryMsg;
            binaryMsg = getBinaryOfString(msg);
            for (int i = 0; i < binaryMsg.size(); i++) {
                for (int j = 0; j < 8; j++) {
                    pixels[j*3 + 48 + 24*i][0] = binaryMsg[i][j];
                }
            }
            setDoesFileHaveMessageFlag();
        }

        std::string decodeMessageFromImage() {
            int msgLength = decodeMessageLength();
            std::vector<std::bitset<8>> binaryMsg(msgLength);
            std::stringstream messageStream;
            unsigned long tempLong;
            unsigned char c;
            for (int i = 0; i < msgLength; i++) {
                for (int j = 0; j < 8; j++) {
                    binaryMsg[i][j] = pixels[j*3 + 48 + 24*i][0];
                }
                tempLong = binaryMsg[i].to_ulong();
                c = static_cast<unsigned char>(tempLong);
                messageStream << c;
            }
            return messageStream.str();
        }

        bool doesImageHaveMessage() {
            return (fileHeader.reserved1 == 2137 && fileHeader.reserved2 == 7312);
        }

        void writeImage(const std::string& pathOut) {
            inputFile.close();
            std::ofstream outputFile(pathOut, std::ios::out | std::ios::binary);
            unsigned long tempLong;
            unsigned char c;
            if (outputFile) {
                outputFile.write((char * ) &fileHeader, sizeof(fileHeader));
                outputFile.write((char * ) &bitmapInfoHeader, sizeof(bitmapInfoHeader));
                for (auto & pixel : pixels) {
                    tempLong = pixel.to_ulong();
                    c = static_cast<unsigned char>(tempLong);
                    outputFile.put(c);
                }
                outputFile.close();
                std::cout << "Message encoded successfully!\n";
            } else {
                std::cerr << "Unable to open output file!";
                exit(-1);
            }
        }

        void setBMPPixelsBinary() {
            inputFile.seekg(fileHeader.offset, std::ifstream::beg);
            char c;

            while (inputFile.get(c)) {
                std::bitset<8> byte(c);
                pixels.push_back(byte);
            }
            for (auto i = 0; i < 200; i++){
                if (i % 3 != 0)
                    continue;
            }
        }

        bool isFileRealBMP() {
            return fileHeader.fileCode == 0x4D42;
        }

        bool isFile24bpp() {
            return bitmapInfoHeader.biBitCount == 24;
        }

        void printBITMAPINFOHEADER() {
            std::cout << "\nBITMAPINFOHEADER\n" <<bitmapInfoHeader.biSize << ' '
                      << bitmapInfoHeader.biWidth << ' ' << bitmapInfoHeader.biHeight
                      << ' ' << bitmapInfoHeader.biPlanes << ' ' << bitmapInfoHeader.biBitCount
                      << ' ' << bitmapInfoHeader.biCompression << ' ' << bitmapInfoHeader.biSizeImage
                      << ' ' << bitmapInfoHeader.biXPelsPerMeter << ' ' << bitmapInfoHeader.biYPelsPerMeter
                      << ' ' << bitmapInfoHeader.biClrUsed << ' ' << bitmapInfoHeader.biClrImportant << ' '
                      << bitmapInfoHeader.biClrRotation << ' ' << bitmapInfoHeader.biReserved;

        }
    };
}

#endif //IMAGESTEGANOGRAPHY_BMP_HPP
