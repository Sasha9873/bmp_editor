#pragma once

#include <iostream>
#include <cstring>
#include <fstream>
#include <iomanip>


static size_t MAX_FILE_NAME = 100;
static uint16_t BMP_TYPE = 0x4D42;

template <typename Type>
void read(std::ifstream &fp, Type &result, std::size_t size) {
    fp.read(reinterpret_cast<char*>(&result), size);
}

class bit_map_file_header{
public:	

	uint16_t 	file_type;          // "bm"
    uint32_t 	file_size;         
    uint32_t 	reserved_space;     // space with zeroes
    uint32_t  	off_bits;      		// offset to field with data
                             		// commonly 54 = 16 + header_size
};

class bit_map_info_header{
public:	

	uint32_t	header_size; 	// size of the struct in bytes
                           		// 40(BITMAPINFOHEADER) or 108(BITMAPV4HEADER)
    uint32_t 	width;			// width in dots
    uint32_t 	height;
    uint16_t 	planes;			// = 1
    uint16_t 	bits_per_pixel;
    uint32_t 	compression;
   	uint32_t 	image_size;
   	uint32_t 	x_pixels_per_meter;
    uint32_t 	y_pixels_per_meter;
    uint32_t 	colors_in_color_table;
    uint32_t 	important_color_count;
};

class pixel_image{
public:	
	uint8_t 	r, g, b, a = 0;   //a - transparency
};


class image{
public:
	image(){}

	bit_map_file_header file_header;
	bit_map_info_header info_header;

	pixel_image** pixels;
};



image* load_image(const char* file_name);
void make_picture(image* save_image, const char* output_file);