#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "USFAT.h"


int main(int argc, char** argv)
{
	uint8_t sector_buffer[S_SECTOR_SIZE];
	FILE* media_f;
	FAT_RUNTIME fat_rt;

	if (argc < 2 ){
		printf("Usage: %s <image_file_name>\n", argv[0]);
		return -1;
	}

	fat_rt.media_f = fopen(argv[1], "rb+");
	read_fat(fat_rt.media_f, fat_rt.fat);  

	//Set current directory to root
	fat_rt.current_dir = FATFS_ROOT_SECTOR;
	read_data_sector( fat_rt.media_f, fat_rt.current_dir, 
			(uint8_t*)fat_rt.dir_buffer);

	
	int op, snum;
	uint8_t buffer[S_SECTOR_SIZE];
	uint16_t fat[FATFS_FAT_ENTRY_NUMBER];
	printf("View (1:FAT 2:Data Sector -1:End) > ");
	while(scanf("%d", &op) != 0){

		if (op == -1) break;

		if (op == 1 ){
			read_fat(fat_rt.media_f, fat);
			print_fat_debug(fat);
		} else {
            printf("Data Sector No. (in hex)> 0x");
			scanf("%x", &snum);
			read_data_sector(fat_rt.media_f, snum, buffer);
			print_sector_debug(buffer);
		}
		printf("View (1:FAT 2:Data Sector -1:End) > ");
	}

	fclose(fat_rt.media_f);
	 
	return 0;
}
