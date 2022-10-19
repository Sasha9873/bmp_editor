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

	pixel_image** new_pixels = create_pixels(im->info_header.width, im->info_header.height);
	if(!new_pixels)
		return;


	for(int i = 0; i < im->info_header.height; i++)
		for(int j = 0; j < im->info_header.width; j++)
		{
			double new_red = im->pixels[i][j].r * 0.393 + im->pixels[i][j].g * 0.769 + im->pixels[i][j].b * 0.189;   
			double new_green = im->pixels[i][j].r * 0.349 + im->pixels[i][j].g * 0.686 + im->pixels[i][j].b * 0.168;
			double new_blue = im->pixels[i][j].r * 0.272 + im->pixels[i][j].g * 0.534 + im->pixels[i][j].b * 0.131;   
		

			new_blue = (new_blue > 255) ? 255 : new_blue;
			new_red = (new_red > 255) ? 255 : new_red;
			new_green = (new_green > 255) ? 255 : new_green;


			new_pixels[i][j].b = static_cast<uint8_t> (new_blue);
			new_pixels[i][j].g = static_cast<uint8_t> (new_green);
			new_pixels[i][j].r = static_cast<uint8_t> (new_red);
			new_pixels[i][j].a = im->pixels[i][j].a;
		}
	
	im->pixels = delete_pixels(im->pixels, im->info_header.height);
	im->pixels = new_pixels;
}

uint8_t find_median(uint8_t x, uint8_t y, uint8_t z)
{
	uint8_t arr[3] = {x, y, z};

	std::sort(arr, arr + 3);

	return arr[1];
}

void less_noise::make(image* im)
{
	pixel_image** new_pixels = create_pixels(im->info_header.width, im->info_header.height);
	if(!new_pixels)
		return;

	for(int i = 0; i < im->info_header.height; ++i)
		for(int j = 0; j < im->info_header.width; ++j)
		{
			/*for(size_t k = 0; k < 3; ++k)
			{
				uint8_t median[4];
				for(size_t num; num < 4; ++num)
					median[num] = *((uint8_t*)(&im->pixels[i][j] + k));

				if(i - 1 >= 0 && i + 1 < im->info_header.height)
					median[0] = find_median(*((uint8_t*)(&im->pixels[i][j] + k)), 
											*((uint8_t*)(&im->pixels[i - 1][j] + k)), 
											*((uint8_t*)(&im->pixels[i + 1][j] + k)));

				if(j - 1 >= 0 && j + 1 < im->info_header.width)
					median[1] = find_median(*((uint8_t*)(&im->pixels[i][j] + k)), 
											*((uint8_t*)(&im->pixels[i][j - 1] + k)), 
											*((uint8_t*)(&im->pixels[i][j + 1] + k)));

				if(j - 1 >= 0 && j + 1 < im->info_header.width && i - 1 >= 0 && i + 1 < im->info_header.height)
				{
					median[2] = find_median(*((uint8_t*)(&im->pixels[i][j] + k)),
											*((uint8_t*)(&im->pixels[i - 1][j + 1] + k)), 
											*((uint8_t*)(&im->pixels[i + 1][j - 1] + k)));

					median[3] = find_median(*((uint8_t*)(&im->pixels[i][j] + k)), 
											*((uint8_t*)(&im->pixels[i - 1][j - 1] + k)), 
											*((uint8_t*)(&im->pixels[i + 1][j + 1] + k)));
				}
				median[0] = find_median(*((uint8_t*)(&im->pixels[i][j] + k)), median[0], median[1]);
				median[1] = find_median(*((uint8_t*)(&im->pixels[i][j] + k)), median[2], median[3]);

				median[0] = find_median(*((uint8_t*)(&im->pixels[i][j] + k)), median[0], median[1]);

				*((uint8_t*)(&new_pixels[i][j] + k)) = median[0];
			}*/

			//reeeeeed

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

			new_pixels[i][j].r = median[0];


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

			new_pixels[i][j].g = median[0];


			//bluuuue

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

			new_pixels[i][j].b = median[0];

			new_pixels[i][j].a = im->pixels[i][j].a;
		}

	im->pixels = delete_pixels(im->pixels, im->info_header.height);
	im->pixels = new_pixels;
}

int need_index(int i, int max_index) 
{
	if(i < 0) 
		i = 0;

	if(i >= max_index)
		i = max_index - 1;

	return i;
}

pixel_image** apply_convolution(std::vector<std::vector<int>> change_arr, int n, image* im, int coeff)
{
	pixel_image** new_pixels = create_pixels(im->info_header.width, im->info_header.height);
	if(!new_pixels)
		return nullptr;

	int k = n / 2;

	for(int i = 0; i < im->info_header.height; ++i)
		for(int j = 0; j < im->info_header.width; ++j)
		{
			int res_b = 0, res_g = 0, res_r = 0;

			for(int neighbour_i = i - k; neighbour_i <= i + k; ++neighbour_i)
				for(int neighbour_j = j - k; neighbour_j <= j + k; ++neighbour_j)
				{
					int new_i = need_index(neighbour_i, im->info_header.height);
					int new_j = need_index(neighbour_j, im->info_header.width);

					res_b += im->pixels[new_i][new_j].b * change_arr[neighbour_i - i + k][neighbour_j - j + k];
					res_g += im->pixels[new_i][new_j].g * change_arr[neighbour_i - i + k][neighbour_j - j + k];
					res_r += im->pixels[new_i][new_j].r * change_arr[neighbour_i - i + k][neighbour_j - j + k];
				}


			res_b /= coeff;
			res_g /= coeff;
			res_r /= coeff;

			res_b = (res_b > 0) ? res_b : 0;
			new_pixels[i][j].b = (res_b > 255) ? 255 : res_b;
				
			res_g = (res_g > 0) ? res_g : 0;
			new_pixels[i][j].g = (res_g > 255) ? 255 : res_g;

			res_r = (res_r > 0) ? res_r : 0;
			new_pixels[i][j].r = (res_r > 255) ? 255 : res_r;

			new_pixels[i][j].a = im->pixels[i][j].a;
			
		} 

	return new_pixels;
}


pixel_image** apply_convolution_gaus(std::vector<std::vector<int>> change_arr, int n, image* im)
{
	pixel_image** new_pixels = create_pixels(im->info_header.width, im->info_header.height);
	if(!new_pixels)
		return nullptr;

	int k = n / 2;
	printf("k = %d    %d\n", k, change_arr[0][0]);

	for(int i = 0; i < im->info_header.height; ++i)
		for(int j = 0; j < im->info_header.width; ++j)
		{
			int res_b = 0, res_g = 0, res_r = 0, coeff = 0;

			for(int neighbour_i = i - k; neighbour_i <= i + k; ++neighbour_i)
				for(int neighbour_j = j - k; neighbour_j <= j + k; ++neighbour_j)
				{
					if(neighbour_i >= 0 && neighbour_j >= 0 && neighbour_i < im->info_header.height && neighbour_j < im->info_header.width)
					{
						//printf("neighbour_i = %d neighbour_j = %d\n", neighbour_i, neighbour_j);
						res_b += im->pixels[neighbour_i][neighbour_j].b * change_arr[neighbour_i - i + k][neighbour_j - j + k];
						res_g += im->pixels[neighbour_i][neighbour_j].g * change_arr[neighbour_i - i + k][neighbour_j - j + k];
						res_r += im->pixels[neighbour_i][neighbour_j].r * change_arr[neighbour_i - i + k][neighbour_j - j + k];

						coeff += change_arr[neighbour_i - i + k][neighbour_j - j + k];
					}
				}

			res_b /= coeff;
			res_g /= coeff;
			res_r /= coeff;

			res_b = (res_b > 0) ? res_b : 0;
			new_pixels[i][j].b = (res_b > 255) ? 255 : res_b;
				
			res_g = (res_g > 0) ? res_g : 0;
			new_pixels[i][j].g = (res_g > 255) ? 255 : res_g;

			res_r = (res_r > 0) ? res_r : 0;
			new_pixels[i][j].r = (res_r > 255) ? 255 : res_r;

			new_pixels[i][j].a = im->pixels[i][j].a;
			
		} 

	return new_pixels;
}

void gaus::make(image* im)
{
	std::vector<std::vector<int>> change_arr = {{1, 4, 6, 4, 1},
									   			{4, 16, 24, 16, 4},
									   			{6, 24, 36, 24, 6},
											    {4, 16, 24, 16, 4},
											    {1, 4, 6, 4, 1}};
	
	pixel_image** new_pixels = apply_convolution_gaus(change_arr, 5, im);
	if(!new_pixels)
		return;


	im->pixels = delete_pixels(im->pixels, im->info_header.height);
	im->pixels = new_pixels;
}


void improve_clarity::make(image* im)
{
	std::vector<std::vector<int>> change_arr = {{-1, -1, -1}, 
												{-1, 9, -1}, 
												{-1, -1, -1}};
	
	pixel_image** new_pixels = apply_convolution(change_arr, 3, im, 1);
	if(!new_pixels)
		return;

	im->pixels = delete_pixels(im->pixels, im->info_header.height);
	im->pixels = new_pixels;
}


void select_edge::make(image* im)
{
	std::vector<std::vector<int>> change_arr = {{-1, -1, -1}, 
												{-1, 8, -1}, 
												{-1, -1, -1}};
	
	pixel_image** new_pixels = apply_convolution(change_arr, 3, im, 1);
	if(!new_pixels)
		return;

	im->pixels = delete_pixels(im->pixels, im->info_header.height);
	im->pixels = new_pixels;
}