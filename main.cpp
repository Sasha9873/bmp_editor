#include "bmp.h"


int main(int argc, char* argv[])
{
	char* file_name = new char[MAX_FILE_NAME];
    strcpy(file_name, argv[argc - 1]);

    image* im =  load_image(file_name);

    make_picture(im, file_name);

	return 0;
}