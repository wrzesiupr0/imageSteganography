#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>

int main(){
    struct BMPFileHeader {
        uint16_t file_type;          // File type always BM which is 0x4D42
        int file_size{0};               // Size of the file (in bytes)
        int reserved1{0};               // Reserved, always 0
        int reserved2{0};               // Reserved, always 0
        int offset_data{0};             // Start position of pixel data (bytes from the beginning of the file)
    };

    BMPFileHeader file_header;


    std::ifstream ifs ("C:\\PJATK\\C++\\smallbmp.bmp", std::ios::in|std::ios::binary|std::ios::ate);

    char* oData;

    std::ifstream::pos_type size = ifs.tellg();

    std::cout << "Size of file: " << size;
    ifs.seekg(0, std::ios::beg);

    oData = new char[size];

    ifs.read(oData, size);

    std::cout << "\noData size: " << strlen(oData) << std::endl;

    for ( int i = 0; i < strlen(oData); i++ )
	{
		std::cout << "oData["<<i<<"] " << oData[i];
		std::cout << "\n";
		std::cout << oData[i] << " + 'a' = " << ( (int)oData[i] );
		std::cout << "\n\n";

	}


    return 0;
}