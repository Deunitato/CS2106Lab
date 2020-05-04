/*************************************
* Lab 5 Exercise 2
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

//TODO: Add helper functions if needed

int import_file(FAT_RUNTIME* rt, char* filename, uint16_t try_sector)
//Import a file content into USFAT based filesystem
{

    //TODO: Add your code here

	return -1;
}


int main(int argc, char** argv)
{
	FAT_RUNTIME fat_rt;
	char filename[13];
    int result;

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
	printf("Import File (\"DONE\" to quit) > ");
	while(scanf("%s", filename) != 0){

		if(!strcmp(filename, "DONE")) break;
    
        int start_sector;
		
        printf("Start sector (in Hex) > 0x");
        scanf("%x", &start_sector);

        printf("Import \"%s\" to [0x%04x] Data Sector...", filename, start_sector);

	    if((result = import_file(&fat_rt, filename, start_sector)) == -1){
			printf("FAILED!\n");
		} else {
			printf("Written %d bytes.\n", result);
        } 

		print_directory(fat_rt.dir_buffer);
	    printf("\nImport File (\"DONE\" to quit) > ");
	}
 
	return 0;
}
