#include "bmp.h"

void grey::make(image* im)
{
	for(int i = 0; i < im->info_header.height; i++)
		for(int j = 0; j < im->info_header.width; j++)
	    {
	    	double res = 0.299 * im->pixels[i][j].r + 0.587 * im->pixels[i][j].g + 0.114 * im->pixels[i][j].b;
	    	im->pixels[i][j].b = static_cast<uint8_t> (res);
		   	im->pixels[i][j].g = static_cast<uint8_t> (res);
	    	im->pixels[i][j].r = static_cast<uint8_t> (res);
	    }

}

void negative::make(image* im)
{
	for(int i = 0; i < im->info_header.height; i++)
		for(int j = 0; j < im->info_header.width; j++)
	    {
	    	im->pixels[i][j].b = ~im->pixels[i][j].b;
			im->pixels[i][j].g = ~im->pixels[i][j].g;
	    	im->pixels[i][j].r = ~im->pixels[i][j].r;
	    }	
}

void sepia::make(image* im)
{
	/*outputRed = (inputRed * .393) + (inputGreen *.769) + (inputBlue * .189)
	outputGreen = (inputRed * .349) + (inputGreen *.686) + (inputBlue * .168)
	outputBlue = (inputRed * .272) + (inputGreen *.534) + (inputBlue * .131)*/


	for(int i = 0; i < im->info_header.height; i++)
		for(int j = 0; j < im->info_header.width; j++)
		{
			double new_blue = im->pixels[i][j].b * 0.393 + im->pixels[i][j].g * 0.769 + im->pixels[i][j].r * 0.189;   //in fact red
			double new_green = im->pixels[i][j].b * 0.349 + im->pixels[i][j].g * 0.686 + im->pixels[i][j].r * 0.168;
			double new_red = im->pixels[i][j].b * 0.272 + im->pixels[i][j].g * 0.534 + im->pixels[i][j].r * 0.131;   //in fact blue
			
			/*double new_red = im->pixels[i][j].r * 0.393 + im->pixels[i][j].g * 0.769 + im->pixels[i][j].b * 0.189;   //in fact red
			double new_green = im->pixels[i][j].r * 0.349 + im->pixels[i][j].g * 0.686 + im->pixels[i][j].b * 0.168;
			double new_blue = im->pixels[i][j].r * 0.272 + im->pixels[i][j].g * 0.534 + im->pixels[i][j].b * 0.131;*/ 

			new_blue = (new_blue > 255) ? 255 : new_blue;
			new_red = (new_red > 255) ? 255 : new_red;
			new_green = (new_green > 255) ? 255 : new_green;


			im->pixels[i][j].b = static_cast<uint8_t> (new_blue);
			im->pixels[i][j].g = static_cast<uint8_t> (new_green);
			im->pixels[i][j].r = static_cast<uint8_t> (new_red);
		}
	
}

uint8_t find_median(uint8_t x, uint8_t y, uint8_t z)
{
	uint8_t arr[3] = {x, y, z};

	std::sort(arr, arr + 3);

	return arr[1];
}

void less_noise::make(image* im)
{
	for(int i = 0; i < im->info_header.height; ++i)
		for(int j = 0; j < im->info_header.width; ++j)
		{
			//reeeeeed (blue)

			uint8_t median[4] = {im->pixels[i][j].r, im->pixels[i][j].r, im->pixels[i][j].r, im->pixels[i][j].r};
			if(i - 1 >= 0 && i + 1 < im->info_header.height)
				median[0] = find_median(im->pixels[i][j].r, im->pixels[i - 1][j].r, im->pixels[i + 1][j].r);
			if(j - 1 >= 0 && j + 1 < im->info_header.width)
				median[1] = find_median(im->pixels[i][j].r, im->pixels[i][j - 1].r, im->pixels[i][j + 1].r);
			if(j - 1 >= 0 && j + 1 < im->info_header.width && i - 1 >= 0 && i + 1 < im->info_header.height)
			{
				median[2] = find_median(im->pixels[i - 1][j + 1].r, im->pixels[i][j].r, im->pixels[i + 1][j - 1].r);
				median[3] = find_median(im->pixels[i - 1][j - 1].r, im->pixels[i][j].r, im->pixels[i + 1][j + 1].r);
			}

			median[0] = find_median(im->pixels[i][j].r, median[0], median[1]);
			median[1] = find_median(im->pixels[i][j].r, median[2], median[3]);

			median[0] = find_median(im->pixels[i][j].r, median[0], median[1]);

			im->pixels[i][j].r = median[0];


			//greeeeeen 

			for(int k = 0; k < 4; ++k)
				median[k] = im->pixels[i][j].g;
			if(i - 1 >= 0 && i + 1 < im->info_header.height)
				median[0] = find_median(im->pixels[i][j].g, im->pixels[i - 1][j].g, im->pixels[i + 1][j].g);
			if(j - 1 >= 0 && j + 1 < im->info_header.width)
				median[1] = find_median(im->pixels[i][j].g, im->pixels[i][j - 1].g, im->pixels[i][j + 1].g);
			if(j - 1 >= 0 && j + 1 < im->info_header.width && i - 1 >= 0 && i + 1 < im->info_header.height)
			{
				median[2] = find_median(im->pixels[i - 1][j + 1].g, im->pixels[i][j].g, im->pixels[i + 1][j - 1].g);
				median[3] = find_median(im->pixels[i - 1][j - 1].g, im->pixels[i][j].g, im->pixels[i + 1][j + 1].g);
			}

			median[0] = find_median(im->pixels[i][j].g, median[0], median[1]);
			median[1] = find_median(im->pixels[i][j].g, median[2], median[3]);

			median[0] = find_median(im->pixels[i][j].g, median[0], median[1]);

			im->pixels[i][j].g = median[0];


			//bluuuue (red)

			for(int k = 0; k < 4; ++k)
				median[k] = im->pixels[i][j].b;

			if(i - 1 >= 0 && i + 1 < im->info_header.height)
				median[0] = find_median(im->pixels[i][j].b, im->pixels[i - 1][j].b, im->pixels[i + 1][j].b);
			if(j - 1 >= 0 && j + 1 < im->info_header.width)
				median[1] = find_median(im->pixels[i][j].b, im->pixels[i][j - 1].b, im->pixels[i][j + 1].b);
			if(j - 1 >= 0 && j + 1 < im->info_header.width && i - 1 >= 0 && i + 1 < im->info_header.height)
			{
				median[2] = find_median(im->pixels[i - 1][j + 1].b, im->pixels[i][j].b, im->pixels[i + 1][j - 1].b);
				median[3] = find_median(im->pixels[i - 1][j - 1].b, im->pixels[i][j].b, im->pixels[i + 1][j + 1].b);
			}

			median[0] = find_median(im->pixels[i][j].b, median[0], median[1]);
			median[1] = find_median(im->pixels[i][j].b, median[2], median[3]);

			median[0] = find_median(im->pixels[i][j].b, median[0], median[1]);

			im->pixels[i][j].b = median[0];
		}
}	


void improve_clarity::make(image* im)
{
	int change_arr[3][3] = {-1, -1, -1, -1, 9, -1, -1, -1, -1};

	

	for(int i = 0; i < im->info_header.height; ++i)
		for(int j = 0; j < im->info_header.width; ++j)
		{
			int res_b = 0, res_g = 0, res_r = 0, coeff = 0;

			for(int neighbour_i = i - 1; neighbour_i < i + 2; ++neighbour_i)
				for(int neighbour_j = j - 1; neighbour_j < j + 2; ++neighbour_j)
				{
					//printf("neighbour_i = %d neighbour_j = %d\n", neighbour_i, neighbour_j);
					if(neighbour_i >= 0 && neighbour_j >= 0 && neighbour_i < im->info_header.height && neighbour_j < im->info_header.width)
					{
						res_b += im->pixels[neighbour_i][neighbour_j].b * change_arr[neighbour_i - i + 1][neighbour_j - j + 1];
						res_g += im->pixels[neighbour_i][neighbour_j].g * change_arr[neighbour_i - i + 1][neighbour_j - j + 1];
						res_r += im->pixels[neighbour_i][neighbour_j].r * change_arr[neighbour_i - i + 1][neighbour_j - j + 1];

						coeff += change_arr[neighbour_i - i + 1][neighbour_j - j + 1];
						//std::cout << coeff << std::endl;
					}
				}

			res_b /= coeff;
			res_g /= coeff;
			res_r /= coeff;

			res_b = (res_b > 0) ? res_b : 0;
			im->pixels[i][j].b = (res_b > 255) ? 255 : res_b;
				
			res_g = (res_g > 0) ? res_g : 0;
			im->pixels[i][j].g = (res_g > 255) ? 255 : res_g;

			res_r = (res_r > 0) ? res_r : 0;
			im->pixels[i][j].r = (res_r > 255) ? 255 : res_r;
			
		} 
}

