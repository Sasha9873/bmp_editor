#include "bmp.h"


int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        image* im = nullptr;
        read_comands(im);
        //docs();
        return 0;
    }

    char* file_name = new char[MAX_FILE_NAME];
    if(!file_name)
    {
        std::cout << "Not enough memory\n";
        return 1;
    }

    strcpy(file_name, argv[1]);

    if(!strcmp(file_name, "--help") || !strcmp(file_name, "-h"))
    {
        docs();
        delete[] file_name;
        return 0;
    }
      
    image* im =  load_image(file_name);
    if(!im)
    {
        std::cout << "Could not load image\n";
        return 2;
    }

    if(argc == 2)
        read_comands(im);
    else
        read_flags(im, argc, argv);


    delete[] file_name;

	return 0;
}