#pragma once

#include <iostream>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <list>
#include <type_traits>
#include <algorithm>


static size_t MAX_FILE_NAME = 100;
static uint16_t BMP_TYPE = 0x4D42;
static size_t MAX_COMAND = 100;

template <typename Type>
void read(std::ifstream &fp, Type &result, std::size_t size) {
    fp.read(reinterpret_cast<char*>(&result), size);
}

enum comands{
	NEGATIVE 		= 1,
	REPLACE_COLOR 	= 2,
	IMPROVE_CLARITY = 3,
};


class bit_map_file_header{
public:	

	uint16_t 	file_type;          // "bm"
    uint32_t 	file_size;         
    uint32_t 	reserved_space;     // space with zeroes
    uint32_t  	off_bits;      		// offset to field with data
                             		// commonly 54 = 16 + header_size
}__attribute__((packed));

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
}__attribute__((packed));

class pixel_image{
public:	
	pixel_image(){}

	pixel_image& operator=(const pixel_image& other)
	{
		if(std::addressof(other) == this)
		{
			return *this;
		}

		r = other.r;
		g = other.g;
		b = other.b;
		a = other.a;

		return *this;
	}

	/*bool operator==(pixel_image& first, pixel_image& second)
	{
		if(std::addressof(first) == std::addressof(second))
		{
			return true;
		}

		if(first.b == second.b && first.g == second.g && first.r == second.r && first.a == second.a)
			return true;

		return false;
	}*/


	~pixel_image(){}

	uint8_t 	r, g, b, a;   //a - transparency   // in fact you have bgr or bgra    r = b g = g b = r  or  r = b g = g b = r a = a
};


class image{
public:
	image(){}

	~image(){}

	bit_map_file_header file_header;
	bit_map_info_header info_header;

	pixel_image** pixels;
};



class all_operations{
public:
	all_operations(){}

	virtual void make(image* im)
	{

	}
};

class negative: public all_operations{
public:
	negative(){}

	void make(image* im);

	~negative(){}
};


class cut: public all_operations{
public:
	cut(){}

	cut(size_t x, size_t y, size_t width, size_t height): x(x), y(y), width(width), height(height){}

	void make(image* im) override
	{
		if(!im)
			return;
		if(x + width >= im->info_header.width || y + height >= im->info_header.height)
			return;

		pixel_image** new_pixels = (pixel_image**)malloc(sizeof(pixel_image*) * height);
		if(!new_pixels)
		{
			std::cout << "Not enough memory\n";
			exit(0);
		}

		for(size_t i = 0; i < height; ++i)
		{
			new_pixels[i] = (pixel_image*)malloc(sizeof(pixel_image) * width);
			if(!new_pixels[i])
			{
				for(size_t j = 0; j < i; ++j)
					free(new_pixels[j]);
				free(new_pixels);

				std::cout << "Not enough memory\n";
				exit(0);
			}
		}

		for(size_t i = 0; i < height; ++i)
		{
			for(size_t j = 0; j < width; ++j)
			{
				new_pixels[i][j] = im->pixels[x + i][y + j];
			}
		}

		for(size_t i = 0; i < im->info_header.height; ++i)
			free(im->pixels[i]);
		free(im->pixels);

		im->pixels = new_pixels;
		int k_bytes_in_pix = im->info_header.bits_per_pixel / 8;
		size_t sub_bytes = (im->info_header.height * im->info_header.width - width * height) * k_bytes_in_pix;
		im->info_header.height = height;
		im->info_header.width = width;
		im->info_header.image_size -= sub_bytes;
		im->file_header.file_size -= sub_bytes;
	}

	~cut(){}

private:
	size_t x;
	size_t y;
	size_t width;
	size_t height;
};


class replace_color: public all_operations{
public:
	replace_color(){}

	replace_color(const pixel_image& old, const pixel_image& new_one)
	{
		//printf("Create\n");
		old_color = new pixel_image[1];
		new_color = new pixel_image[1];

		*old_color = old;
		*new_color = new_one;
	}

	void make(image* im) override
	{
		std::cout << std::endl;
		if(!im->pixels)
			return;

		if(!old_color || !new_color)
		{
			printf("Null\n");
			return;
		}

		if(im->info_header.bits_per_pixel == 32)
		{
			for(int i = 0; i < im->info_header.height; i++)
			    for(int j = 0; j < im->info_header.width; j++)
			        if(im->pixels[i][j].r == old_color->r && im->pixels[i][j].g == old_color->g && im->pixels[i][j].b == old_color->b  && im->pixels[i][j].a == old_color->a)
		                im->pixels[i][j] = *new_color;
		}
		else
		{
			new_color->a = 0;

			for(int i = 0; i < im->info_header.height; i++)
		        for(int j = 0; j < im->info_header.width; j++)
		            if(im->pixels[i][j].r == old_color->r || im->pixels[i][j].g == old_color->g || im->pixels[i][j].b == old_color->b)
		            {
		                im->pixels[i][j] = *new_color;
		            }
		}
	}


	~replace_color()
	{
		if(old_color)
			delete[] old_color;
		if(new_color)
			delete[] new_color;
	}

private:

	pixel_image* old_color;	
	pixel_image* new_color;
};

class grey: public all_operations{
public:
	grey(){}

 	void make(image* im);
 	
 	~grey(){}
 };

class sepia: public all_operations{
public:
	sepia(){}

	void make(image* im);

	~sepia(){}
};



class improve_clarity: public all_operations{
public:
	improve_clarity(){}

	void make(image* im);

	~improve_clarity(){}
};

class less_noise: public all_operations{
public:
	less_noise(){}

	void make(image* im);

	~less_noise(){}
};


image* load_image(const char* file_name);
void make_picture(image* save_image, const char* output_file);
//void replace_color(pixel_image** pixels, pixel_image& old_color, pixel_image& new_color, uint32_t width, uint32_t height, uint16_t bits_per_pixel) ;
//void improve_clarity(pixel_image** pixels, uint32_t width, uint32_t height, uint16_t bits_per_pixel);
//void negative(pixel_image** pixels, uint32_t width, uint32_t height);
image* delete_all(image* im);
void read_comands(image* im);
void read_flags(image* im, int argc, char* argv[]);
void use_filters(image* im, std::list <all_operations*> comands_order);
//void grey_filter(image* im);
void docs();