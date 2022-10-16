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

	/*read(file_stream, new_image->file_header.file_type, sizeof(new_image->file_header.file_type));

	if(new_image->file_header.file_type != BMP_TYPE)
	{
		printf("This file is not bmp, it is \'%x \'(bmp is \'%x\')", new_image->file_header.file_type, BMP_TYPE);
		exit(0);
	}
		
	read(file_stream, new_image->file_header.file_size, sizeof(new_image->file_header.file_size));
	read(file_stream, new_image->file_header.reserved_space, sizeof(new_image->file_header.reserved_space));
	read(file_stream, new_image->file_header.off_bits, sizeof(new_image->file_header.off_bits));*/

	read(file_stream, new_image->file_header, sizeof(new_image->file_header));
	read(file_stream, new_image->info_header, sizeof(new_image->info_header));

	/*printf("type = %x size = %x reserved_space = %x off_bits = %x\n", 
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

	printf("header_size = %d width = %d height = %d image_size = %d %d file_size = %d\n", new_image->info_header.header_size, new_image->info_header.width, 
		new_image->info_header.height, new_image->info_header.image_size, new_image->info_header.x_pixels_per_meter,
		new_image->file_header.file_size);
	std::cout << "width = " << new_image->info_header.width <<", height = " 
		<< new_image->info_header.height << "\n";*/
	
	if(new_image->info_header.height < 1 || new_image->info_header.width < 1)
	{
		std::cout << "Wrong width or height (width = " << new_image->info_header.width <<", height = " 
		<< new_image->info_header.height << ")\n";
		exit(0);
	}

	file_stream.seekg(new_image->file_header.off_bits, std::ios::beg);

	new_image->pixels = (pixel_image**)malloc(new_image->info_header.height * sizeof(pixel_image*));
	if(!new_image->pixels)
	{
		std::cout << "Not enough memory\n";
		exit(0);
	}


	std::cout << new_image->info_header.height << " " <<  new_image->info_header.width << std::endl;
	for(size_t i = 0; i < new_image->info_header.height; ++i)
	{
		new_image->pixels[i] = (pixel_image*)malloc((new_image->info_header.width) * sizeof(pixel_image));
		if(!new_image->pixels[i])
		{
			for(size_t j = 0; j < i; ++j)
				delete[] new_image->pixels[j];

			delete[] new_image->pixels;

			std::cout << "Not enough memory\n";
			exit(0);
		}
	}

	int k_bytes_in_pix = new_image->info_header.bits_per_pixel / 8;  //quantity of bytes in one pixel
	int add_bytes = (4 - (new_image->info_header.width * k_bytes_in_pix) % 4) % 4;

	char trash[3];

	for(size_t i = 0; i < new_image->info_header.height; ++i)
	{
		for(size_t j = 0; j < new_image->info_header.width; ++j)
			read(file_stream, new_image->pixels[i][j], k_bytes_in_pix);
		read(file_stream, trash, add_bytes);
	}


	file_stream.close();

	return new_image;
}


void make_picture(image* save_image, const char* output_file)
{
	FILE* save_file = fopen(output_file, "wb");

	fwrite(&save_image->file_header.file_type, sizeof(save_image->file_header.file_type), 1, save_file);
	fwrite(&save_image->file_header.file_size, sizeof(save_image->file_header.file_size), 1, save_file);
	fwrite(&save_image->file_header.reserved_space, sizeof(save_image->file_header.reserved_space), 1, save_file);
	fwrite(&save_image->file_header.off_bits, sizeof(save_image->file_header.off_bits), 1, save_file);


	fwrite(&save_image->info_header.header_size, sizeof(save_image->info_header.header_size), 1, save_file);
	fwrite(&save_image->info_header.width, sizeof(save_image->info_header.width), 1, save_file);
	fwrite(&save_image->info_header.height, sizeof(save_image->info_header.height), 1, save_file);
	fwrite(&save_image->info_header.planes, sizeof(save_image->info_header.planes), 1, save_file);
	fwrite(&save_image->info_header.bits_per_pixel, sizeof(save_image->info_header.bits_per_pixel), 1, save_file);
	fwrite(&save_image->info_header.compression, sizeof(save_image->info_header.compression), 1, save_file);
	fwrite(&save_image->info_header.image_size, sizeof(save_image->info_header.image_size), 1, save_file);
	fwrite(&save_image->info_header.x_pixels_per_meter, sizeof(save_image->info_header.x_pixels_per_meter), 1, save_file);
	fwrite(&save_image->info_header.y_pixels_per_meter, sizeof(save_image->info_header.y_pixels_per_meter), 1, save_file);
	fwrite(&save_image->info_header.colors_in_color_table, sizeof(save_image->info_header.colors_in_color_table), 1, save_file);
	fwrite(&save_image->info_header.important_color_count, sizeof(save_image->info_header.important_color_count), 1, save_file);

	int k_bytes_in_pix = save_image->info_header.bits_per_pixel / 8;  //quantity of bytes in one pixel
	int add_bytes = (4 - (save_image->info_header.width * k_bytes_in_pix) % 4) % 4;
	char trash[3] = {0, 0, 0};

	fseek(save_file, save_image->file_header.off_bits, 0);

	for(size_t i = 0; i < save_image->info_header.height; ++i)
	{
		for(size_t j = 0; j < save_image->info_header.width; ++j)
			fwrite(&save_image->pixels[i][j], k_bytes_in_pix, 1, save_file); 

		fwrite(trash, 1, add_bytes, save_file); 

	}
	
	fclose(save_file);

}/**/


image* delete_all(image* im)
{
	if(!im)
		return im;
	if(!im->pixels)
		return nullptr;
	
	for(size_t i = 0; i < im->info_header.height; ++i)
		if(im->pixels[i])
			free(im->pixels[i]);
	free(im->pixels);

	return nullptr;
}


void read_comands(image* im)
{
	char* comand = new char[MAX_COMAND];

	std::list <all_operations*> comands_order;

    for(size_t i = 0; i < 100; ++i)
    {
    	std::cin >> comand;
    	if(!strcmp(comand, "stop"))
    	{
			im = delete_all(im);
    		break;
    	}

    	if(!strcmp(comand, "replace"))
    	{
            static pixel_image old_color, new_color;

    		if(im->info_header.bits_per_pixel != 32)
    		{
    			std::cout << "Please write color you want to change(r g b):\n";
                int r = 0, g = 0, b = 0;
                std::cin >> b >> g >> r;
                old_color.r = static_cast<uint8_t> (r);
                old_color.g = static_cast<uint8_t> (g);
                old_color.b = static_cast<uint8_t> (b);

	    		std::cout << "Now write color to what change previous(r g b):\n";
	    		std::cin >> b >> g >> r;
                new_color.r = static_cast<uint8_t> (r);
                new_color.g = static_cast<uint8_t> (g);
                new_color.b = static_cast<uint8_t> (b);
    		}
    		else
    		{
    			std::cout << "Please write color you want to change(b g r transparency):\n";
	    		int a = 0, r = 0, g = 0, b = 0;
                std::cin >> b >> g >> r >> a;
                old_color.a = static_cast<uint8_t> (a);
                old_color.r = static_cast<uint8_t> (r);
                old_color.g = static_cast<uint8_t> (g);
                old_color.b = static_cast<uint8_t> (b);

	    		std::cout << "Now write color to what change previous(b g r transparency):\n";
	    		std::cin >> b >> g >> r >> a;
                new_color.a = static_cast<uint8_t> (a);
                new_color.r = static_cast<uint8_t> (r);
                new_color.g = static_cast<uint8_t> (g);
                new_color.b = static_cast<uint8_t> (b);
    		}
    		
    		static replace_color rep(old_color, new_color);
        	all_operations* oper = &rep;
        	comands_order.push_back(oper);
    		//replace_color(im->pixels, old_color, new_color, im->info_header.width, im->info_header.height, im->info_header.bits_per_pixel);
    	}

    	else if(!strcmp(comand, "sepia"))
    	{
    		static sepia sep;
    		all_operations* oper = &sep;
    		comands_order.push_back(oper);
    	}

    	else if(!strcmp(comand, "cut"))
    	{
    		std::cout << "Write x y width height:\n";
    		size_t x = 0, y = 0, width = 0, height = 0;
    		std::cin >> x >> y >> width >> height;

    		static cut ct(x, y, width, height);
    		all_operations* oper = &ct;
    		comands_order.push_back(oper);
    	}

    	else if(!strcmp(comand, "less_noise"))
    	{
    		static less_noise noise;
        	all_operations* oper = &noise;

        	comands_order.push_back(oper);
    	}

        else if(!strcmp(comand, "negative"))
        {
        	static negative neg;
        	all_operations* oper = &neg;

        	comands_order.push_back(oper);
            //negative(im->pixels, im->info_header.width, im->info_header.height); 
        }

        else if(!strcmp(comand, "improve"))
        {
        	static improve_clarity improve;
        	all_operations* oper = &improve;
        	comands_order.push_back(oper);
            //improve_clarity(im->pixels, im->info_header.width, im->info_header.height, im->info_header.bits_per_pixel);
        }

        else if(!strcmp(comand, "grey"))
        {
        	static grey gr;
        	all_operations* oper = &gr;
        	comands_order.push_back(oper);
        	//grey_filter(im);
        }

        else if(!strcmp(comand, "save"))
        {
        	use_filters(im, comands_order);
            make_picture(im, "new_image.bmp");

        }

        else if(!strcmp(comand, "save_as"))
        {
            char* new_file_name = new char[MAX_FILE_NAME];
            if(!new_file_name)
                std::cout << "You have not enough memory\n";

            std::cin >> new_file_name;
            use_filters(im, comands_order);
            make_picture(im, new_file_name);
        }

        else if(!strcmp(comand, "delete_prev"))
        {
        	comands_order.pop_back();
        }

        else
        {
            std::cout << "You have written the wrong command\n";
        }

    }
}



void read_flags(image* im, int argc, char* argv[])
{
	char* comand = new char[MAX_COMAND];

    for(size_t num = 2; num < argc; ++num)
    {
    	comand = argv[num];
		//printf("comand = %s\n", comand);

    	if(!strcmp(comand, "--replace") || !strcmp(comand, "-r"))
    	{
    		++num;

            pixel_image old_color, new_color;

    		if(im->info_header.bits_per_pixel != 32)
    		{
    			if(num + 5 >= argc)
    			{
    				std::cout << "Wrong use of flag replace\n";
		            docs();
		            break;
    			}
                old_color.b = atoi(argv[num++]);
                old_color.g = atoi(argv[num++]);
                old_color.r = atoi(argv[num++]);

                new_color.b = atoi(argv[num++]);
                new_color.g = atoi(argv[num++]);
                new_color.r = atoi(argv[num]);
    		}
    		else
    		{
    			if(num + 7 >= argc)
    			{
    				std::cout << "Wrong use of flag replace\n";
		            docs();
		            break;
    			}

                old_color.b = atoi(argv[num++]);
                old_color.g = atoi(argv[num++]);
                old_color.r = atoi(argv[num++]);
                old_color.a = atoi(argv[num++]);

                new_color.b = atoi(argv[num++]);
                new_color.g = atoi(argv[num++]);
                new_color.r = atoi(argv[num++]);
                new_color.a = atoi(argv[num]);
    		}
    		
    		replace_color rep(old_color, new_color);
        	rep.make(im);
    		//replace_color(im->pixels, old_color, new_color, im->info_header.width, im->info_header.height, im->info_header.bits_per_pixel);
    	}

    	else if(!strcmp(comand, "--sepia") || !strcmp(comand, "-s"))
    	{
    		sepia sep;
    		sep.make(im);
    	}

    	else if(!strcmp(comand, "--cut") || !strcmp(comand, "-c"))
    	{
    		++num;

    		if(num + 3 >= argc)
    		{
    			std::cout << "Wrong use of flag cut\n";
	            docs();
	            break;
    		}
    		size_t x = atoi(argv[num++]);
    		size_t y = atoi(argv[num++]);
    		size_t width = atoi(argv[num++]);
    		size_t height = atoi(argv[num]);

    		//std::cout << x << " " << y << " " << width << " " << height << std::endl;

    		static cut ct(x, y, width, height);
    		ct.make(im);
    	}

    	else if(!strcmp(comand, "less_noise") || !strcmp(comand, "-l"))
    	{
    		less_noise noise;
        	noise.make(im);
    	}

        else if(!strcmp(comand, "--negative") || !strcmp(comand, "-n"))
        {
        	negative neg;
        	neg.make(im);
        }

        else if(!strcmp(comand, "--improve") || !strcmp(comand, "-i"))
        {
        	improve_clarity improve;
        	improve.make(im);
        }

        else if(!strcmp(comand, "--grey") || !strcmp(comand, "-g"))
        {
        	grey gr;
        	gr.make(im);
        }


        else
        {
            std::cout << "Wrong flag : " << argv[num] << "\n";
            docs();
            break;
        }

    }
    
    make_picture(im, "new_image.bmp");

    im = delete_all(im);
}

void use_filters(image* im, std::list <all_operations*> comands_order)
{
	while(comands_order.size())
	{
		comands_order.front()->make(im);
		comands_order.pop_front();
	}
}

void docs()
{
	//--help -h
	std::cout << "--help -h flags to see docs\n";

	std::cout << "You shold write name of the program than file you want to save to than flags(filters yo want to use)";
	std::cout << " or name of the program and than --help(-h)\n";


	std::cout << "Flags(filters) you cann use\n";

	std::cout << "--negative -n flags to make negative photo\n";
	std::cout << "--improve -i flags to improve photo's clarity\n";
	std::cout << "--cut -c args(x y width height) flags with four args to cut rectangle from your photo with (x, y)- coordinats of the left down corner\n";
	std::cout << "--replace -r args(r g b  r g b or r g b a  r g b a) flags to replace first color to second one\n";
	std::cout << "--less_noise -l flags to make less noice in your photo\n";
	std::cout << "--sepia -s flags to make sepia photo\n";
	std::cout << "--grey -g flags to make grey photo\n";
}