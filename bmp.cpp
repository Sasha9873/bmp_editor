#include "bmp.h"

image* load_image(const char* file_name)
{
	std::ifstream file_stream(file_name, std::ifstream::binary);
	if(!file_stream)
	{
		std::cout << "Error in opening file \"" << file_name << "\"";
		exit(0);
	}

	image* new_image = new image();

	read(file_stream, new_image->file_header.file_type, sizeof(new_image->file_header.file_type));

	if(new_image->file_header.file_type != BMP_TYPE)
	{
		printf("This file is not bmp, it is \'%x \'(bmp is \'%x\')", new_image->file_header.file_type, BMP_TYPE);
		//std::cout << "This file is not bmp, it is \'" << new_image->file_header.file_type << "\'(bmp is \'" 
		//<< BMP_TYPE << "\')";
		exit(0);
	}
		
	read(file_stream, new_image->file_header.file_size, sizeof(new_image->file_header.file_size));
	read(file_stream, new_image->file_header.reserved_space, sizeof(new_image->file_header.reserved_space));
	read(file_stream, new_image->file_header.off_bits, sizeof(new_image->file_header.off_bits));

	printf("type = %x size = %x reserved_space = %x off_bits = %x\n", 
		new_image->file_header.file_type, new_image->file_header.file_size, new_image->file_header.reserved_space, 
		new_image->file_header.off_bits);

	
	read(file_stream, new_image->info_header.header_size, sizeof(new_image->info_header.header_size));
	read(file_stream, new_image->info_header.width, sizeof(new_image->info_header.width));
	read(file_stream, new_image->info_header.height, sizeof(new_image->info_header.height));
	read(file_stream, new_image->info_header.planes, sizeof(new_image->info_header.planes));
	read(file_stream, new_image->info_header.bits_per_pixel, sizeof(new_image->info_header.bits_per_pixel));
	read(file_stream, new_image->info_header.compression, sizeof(new_image->info_header.compression));
	read(file_stream, new_image->info_header.image_size, sizeof(new_image->info_header.image_size));
	read(file_stream, new_image->info_header.x_pixels_per_meter, sizeof(new_image->info_header.x_pixels_per_meter));
	read(file_stream, new_image->info_header.y_pixels_per_meter, sizeof(new_image->info_header.y_pixels_per_meter));
	read(file_stream, new_image->info_header.colors_in_color_table, sizeof(new_image->info_header.colors_in_color_table));
	read(file_stream, new_image->info_header.important_color_count, sizeof(new_image->info_header.important_color_count));

	printf("%x %x %x %x %x %x\n", new_image->info_header.header_size, new_image->info_header.width, 
		new_image->info_header.height, new_image->info_header.planes, new_image->info_header.bits_per_pixel,
		new_image->info_header.compression);
	std::cout << "width = " << new_image->info_header.width <<", height = " 
		<< new_image->info_header.height << "\n";
	
	if(new_image->info_header.height < 1 || new_image->info_header.width < 1)
	{
		std::cout << "Wrong width or height (width = " << new_image->info_header.width <<", height = " 
		<< new_image->info_header.height << ")\n";
		exit(0);
	}

	new_image->pixels = (pixel_image**)malloc(new_image->info_header.height * sizeof(pixel_image*));
	if(!new_image->pixels)
	{
		std::cout << "Not enough memory\n";
		exit(0);
	}


	std::cout << new_image->info_header.height << " " <<  new_image->info_header.width << std::endl;
	for(size_t i = 0; i < new_image->info_header.height; ++i)
	{
		new_image->pixels[i] = (pixel_image*)malloc(new_image->info_header.width * sizeof(pixel_image));
		if(!new_image->pixels[i])
		{
			for(size_t j = 0; j < i; ++j)
				delete[] new_image->pixels[j];

			delete[] new_image->pixels;

			std::cout << "Not enough memory\n";
			exit(0);
		}
	}


	if(new_image->info_header.bits_per_pixel == 32)   // with transparency
	{
		printf("with\n");
		size_t i = 0;
		size_t size_pixels = sizeof(new_image->pixels[i]);
		for(i = 0; i < new_image->info_header.height; ++i)
			read(file_stream, new_image->pixels[i], size_pixels);
	}
	else    		 // without transparency
	{
		printf("without  %d\n", new_image->info_header.bits_per_pixel);
		for(size_t i = 0; i < new_image->info_header.height; ++i)
			for(size_t j = 0; j < new_image->info_header.width; ++j)
				read(file_stream, new_image->pixels[i][j], 3);
	}

	return new_image;
}


void make_picture(image* save_image, const char* output_file)
{
	FILE* new_file = fopen("new_image.bmp", "wb");

	fprintf(new_file, "%c%c", save_image->file_header.file_type, *((char*)&save_image->file_header.file_type + 1));
	for(size_t i = 0; i < 4; ++i)
		fprintf(new_file, "%c", *((char*)&save_image->file_header.file_size + i));
	for(size_t i = 0; i < 4; ++i)
		fprintf(new_file, "%c", *((char*)&save_image->file_header.reserved_space + i));
	for(size_t i = 0; i < 4; ++i)
		fprintf(new_file, "%c", *((char*)&save_image->file_header.off_bits + i));



	for(size_t i = 0; i < 4; ++i)
		fprintf(new_file, "%c", *((char*)&save_image->info_header.header_size + i));
	for(size_t i = 0; i < 4; ++i)
		fprintf(new_file, "%c", *((char*)&save_image->info_header.width + i));
	for(size_t i = 0; i < 4; ++i)
		fprintf(new_file, "%c", *((char*)&save_image->info_header.height + i));

	for(size_t i = 0; i < 2; ++i)
		fprintf(new_file, "%c", *((char*)&save_image->info_header.planes + i));
	for(size_t i = 0; i < 2; ++i)
		fprintf(new_file, "%c", *((char*)&save_image->info_header.bits_per_pixel + i));

	for(size_t i = 0; i < 4; ++i)
		fprintf(new_file, "%c", *((char*)&save_image->info_header.compression + i));
	for(size_t i = 0; i < 4; ++i)
		fprintf(new_file, "%c", *((char*)&save_image->info_header.image_size + i));
	for(size_t i = 0; i < 4; ++i)
		fprintf(new_file, "%c", *((char*)&save_image->info_header.x_pixels_per_meter + i));
	for(size_t i = 0; i < 4; ++i)
		fprintf(new_file, "%c", *((char*)&save_image->info_header.y_pixels_per_meter + i));
	for(size_t i = 0; i < 4; ++i)
		fprintf(new_file, "%c", *((char*)&save_image->info_header.colors_in_color_table + i));
	for(size_t i = 0; i < 4; ++i)
		fprintf(new_file, "%c", *((char*)&save_image->info_header.important_color_count + i));

	/*fprintf(new_file, "%c%c%c", save_image->file_header, save_image->file_header, save_image->file_header);

	size_t size = sizeof(save_image->file_header);
	for(size_t i = 0; i < size; i += 8)
		fprintf(new_file, "%c", save_image->file_header + i);

	size = sizeof(save_image->info_header);
	for(size_t i = 0; i < size; i += 8)
		fprintf(new_file, "%c", save_image->info_header.header_size + i);
*/
	for(size_t i = 0; i < save_image->info_header.height; ++i)
		for(size_t j = 0; j < save_image->info_header.width; ++j)
			fprintf(new_file, "%c%c%c", save_image->pixels[i][j].r, save_image->pixels[i][j].g, save_image->pixels[i][j].b);

	fclose(new_file);


	/*fwrite(prev_image->file_header, 1, sizeof(prev_image->file_header), new_file);
	fwrite(prev_image->info_header, 1, sizeof(prev_image->info_header), new_file);

	for(size_t i = 0; i < new_image->info_header.height; ++i)
		for(size_t j = 0; j < new_image->info_header.width; ++j)
			fwrite(prev_image->pixels[i][j], 1, 3, new_file);*/

	/*std::ofstream fout("new_image.txt", std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
 
    if (!fout.is_open()) // если файл небыл открыт
    {
     	std::cout << "Файл не может быть открыт или создан\n"; // напечатать соответствующее сообщение
     	exit(0);
    }
    std::cout.unsetf(std::ios::dec);
	std::cout.setf(std::ios::hex);

	int x = 11;
	std::cout << x;

    fout.unsetf(std::ios::dec);
	fout.setf(std::ios::hex);

    fout << save_image->file_header.file_type << save_image->file_header.file_size << save_image->file_header.reserved_space;
    fout << save_image->file_header.off_bits << save_image->info_header.header_size << save_image->info_header.width;
    fout << save_image->info_header.height << save_image->info_header.planes << save_image->info_header.bits_per_pixel;
    fout << save_image->info_header.compression << save_image->info_header.image_size;
    fout << save_image->info_header.x_pixels_per_meter;
    fout << save_image->info_header.y_pixels_per_meter << save_image->info_header.colors_in_color_table;
    fout << save_image->info_header.important_color_count;

    for(size_t i = 0; i < save_image->info_header.height; ++i)
			for(size_t j = 0; j < save_image->info_header.width; ++j)
				fout << save_image->pixels[i][j].r << save_image->pixels[i][j].g << save_image->pixels[i][j].b;

    fout.close();*/

}/**/