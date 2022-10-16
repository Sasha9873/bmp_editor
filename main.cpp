#include "bmp.h"


int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        docs();
        return 0;
    }

    char* file_name = new char[MAX_FILE_NAME];
    strcpy(file_name, argv[1]);

    if(!strcmp(file_name, "--help") || !strcmp(file_name, "-h"))
    {
        docs();
        delete[] file_name;
        return 0;
    }
      
    image* im =  load_image(file_name);

    if(argc == 2)
        read_comands(im);
    else
        read_flags(im, argc, argv);


    delete[] file_name;

	return 0;
}