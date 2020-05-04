/*************************************
* Lab 5 Exercise 1
* Name:
* Student No:
* Lab Group:
*************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../common/USFAT.h"

void print_as_text(uint8_t *data, int size)
{
    for (int i=0; i < size; i++)
    {
        printf("%c", data[i]);

		//Use a standard 80 char width
        if ( i+1 % 80 == 0) {
        	printf("\n");
        }
    }
}

//TODO: Add helper functions if needed

int read_file( FAT_RUNTIME* rt, char filename[])
{
    //TODO: Your code here

	return 0;	
}

int main(int argc, char** argv)
{
	FAT_RUNTIME fat_rt;
	char filename[13];

	if (argc < 2 ){
		printf("Usage: %s <image_file_name>\n", argv[0]);
		return -1;
	}

    //Load USFAT-based file system from a "disk media"
    if (init_runtime_from_media(&fat_rt, argv[1]) == -1){
        printf("Unable to initialize USFAT from \"%s\".\n",argv[1]);
        return 1;
    }

	//Set current directory to root
    set_directory_to(&fat_rt, FATFS_ROOT_SECTOR);
	
	print_directory(fat_rt.dir_buffer);
	printf("Read File (\"DONE\" to quit) > ");
	while(scanf("%s", filename) != 0){

		if(!strcmp(filename, "DONE")) break;
		
		if(!read_file( &fat_rt, filename)){
			printf("\"%s\" not found!\n", filename);
		} else {
            printf("\n");   //additional newline for readability
        }

		print_directory(fat_rt.dir_buffer);
		printf("Read File (\"DONE\" to quit) > ");
	}
 
	return 0;
}
