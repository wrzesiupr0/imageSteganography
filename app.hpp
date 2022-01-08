#ifndef IMAGESTEGANOGRAPHY_APP_HPP
#define IMAGESTEGANOGRAPHY_APP_HPP

#include <string>
#include <fstream>
#include <iomanip>
#include "sys/types.h"
#include "app.hpp"
#include <sys/stat.h>

namespace app {

    std::string getFileCreationTime(char const *path)
    {
        struct stat attr{};
        stat(path, &attr);
        return ctime(&attr.st_ctime);
    }

    std::string getFileLastModifyTime(char const *path)
    {
        struct stat attr{};
        stat(path, &attr);
        return ctime(&attr.st_mtime);
    }

    void printHelpInfo() {
        std::cout << "\n\n\t\tImage Steganography\n\nThis program is used for encoding and decoding messages in images.\n"
                     "Available file extensions:\t.png\t\t24-bit .bmp\n\n"
                     "Run program with one of the followed parameters:\n\n"
                     "\t-i --info [filePath]\t\t\tReturns file info: format, image size, disk size, last modify time, create time\n"
                     "\t-e --encrypt [filePath] [message]\tEncode given message in image file if possible\n"
                     "\t-d --decrypt [filePath]\t\t\tDecode hidden message from given image\n"
                     "\t-c --check [filePath] [message]\t\tChecks if given message can be encoded in an image file\n"
                     "\t-h --help\t\t\t\tShows this help prompt\n";
    }
}
#endif //IMAGESTEGANOGRAPHY_APP_HPP
