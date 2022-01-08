#ifndef IMAGESTEGANOGRAPHY_BMP_HPP
#define IMAGESTEGANOGRAPHY_BMP_HPP

#include <iostream>
#include <fstream>
#include <cstdint>
#include <algorithm>
#include <string>
#include <vector>

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
//
//    struct pixel {
//        std::uint8_t B;
//        std::uint8_t G;
//        std::uint8_t R;
//    };
#pragma pack(pop)

    struct BMP {
        std::ifstream inputFile;
        BITMAPFILEHEADER fileHeader;
        BITMAPINFOHEADER bitmapInfoHeader;
        std::vector<uint8_t> pixels;
        std::vector<char> rawMetadata;
        uint32_t row_stride{ 0 };

        BMP(const std::string& path) {
            inputFile = std::ifstream(path, std::ios::in|std::ios::binary);
        }

        void getFileMetadata() {
            inputFile.read((char *) &fileHeader, sizeof(fileHeader));
            inputFile.read((char *) &bitmapInfoHeader, sizeof(bitmapInfoHeader));

        }

        void getBMPPixels() {
            inputFile.seekg(fileHeader.offset, std::ifstream::beg);
            std::cout << "\n Pixels: " << pixels.size() << '\t';
            pixels.resize(bitmapInfoHeader.biSizeImage);
            std::cout << "Pixels resize: " << pixels.size() << '\n' << pixels.size() << '\n';
            char c;
            int sum = 0;
            while (inputFile.get(c)) {
//                std::cout << "C: " << (uint8_t)c << '\t';
//                char d = 0;
                for (int i = 7; i >= 0; i--)
                    std::cout << ((c >> i) & 1);

                if (sum == 2){
                    sum = 0;
                    std::cout << '\n';
                } else {
                    sum++;
                    std::cout << ' ';
                }
                std::cout << "Pos: " << inputFile.tellg() << ' ';
            }
//            if (bitmapInfoHeader.biWidth % 4 == 0) {
//                inputFile.read((char*)pixels.data(), pixels.size());
////                fileHeader.fileSize += static_cast<uint32_t>(pixels.size());
//            }

        }

        bool isFileBMP() {
            return fileHeader.fileCode == 0x4D42;
        }

        bool isFile24bpp() {
            return bitmapInfoHeader.biBitCount == 24;
        }

        void saveRawMetadata() {
            inputFile.seekg(0, std::ifstream::beg);
            rawMetadata.resize(fileHeader.offset);
//            std::cout << "SIZE: " << sizeof(rawMetadata)/sizeof(*rawMetadata) << std::endl;
            inputFile.read((char * )rawMetadata.data(), rawMetadata.size());
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
