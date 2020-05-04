#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "USFAT.h"

int create_USFAT_IMG(char* filename)
//Constants Used:
//- S_SECTOR_SIZE 
//- S_FORMAT_MAGIC
//- FATFS_FAT_SECTOR_COUNT 
//- FATFS_FAT_ENTRY_SIZE
//- FATFS_FAT_ENTRY_NUMBER 
{
	FILE* img_f;
	uint8_t zero_buffer[ S_SECTOR_SIZE ];
	uint8_t buffer[ S_SECTOR_SIZE ];
	uint16_t fat[FATFS_FAT_ENTRY_NUMBER];
	int i;

	if((img_f = fopen(filename,"wb")) == NULL){
		return -1;
	}

	//Write the file table
	//Take care to put a FE_END for root directory sector
	memset(fat, 0, S_SECTOR_SIZE);  

	for (i = 0; i < FATFS_FAT_ENTRY_NUMBER; i++){
		fat[i]  = FE_FREE;
	}
	fat[FATFS_ROOT_SECTOR]  = FE_END;

	for (i = 0; i < FATFS_FAT_SECTOR_COUNT; i++){
		if(fwrite(fat, 2, FATFS_FAT_ENTRY_NUMBER, img_f)
		   != FATFS_FAT_ENTRY_NUMBER) {

			return -1;
		}
	}

	//Write the data region
	//Take care to zero out the root directory
	memset(zero_buffer, 0, S_SECTOR_SIZE);  
	memset(buffer, S_FORMAT_MAGIC , S_SECTOR_SIZE); 

	for (i = 0; i < FATFS_FAT_ENTRY_NUMBER; i++){
		if ( i == FATFS_ROOT_SECTOR){
			if(fwrite(zero_buffer, 1, S_SECTOR_SIZE, img_f) != S_SECTOR_SIZE){
				return -1;
			}

		} else {
			if(fwrite(buffer, 1, S_SECTOR_SIZE, img_f) != S_SECTOR_SIZE){
				return -1;
			}
		}
	}

	fclose(img_f);

	//Successful!
	return 0;
}

int main(int argc, char** argv)
{
	FAT_DE sample;
	uint8_t sector_buffer[S_SECTOR_SIZE];
	FILE* media_f;
	FAT_RUNTIME fat_rt;

	if (argc < 2 ){
		printf("Usage: %s <image_file_name>\n", argv[0]);
		return -1;
	}

    print_basic_info();

	if(create_USFAT_IMG(argv[1])){
		printf("Image File creation failed\n");
	}

	return 0;
}
