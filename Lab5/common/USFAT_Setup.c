#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "USFAT.h"

int get_free_DE( FAT_DE directory[] )
{
	int i;
	for (i = 0; i < DE_NUMBER_IN_SECTOR; i++){
		if (DE_IS_FREE(directory[i])){
			return i;
		}
	}
	return -1;
}

int get_free_data_sector( uint16_t fat[], uint16_t start,
					uint16_t*candidate)
{
	uint16_t data_sector = start;

	while (!FE_IS( fat[data_sector], FE_FREE)) {
		data_sector = (data_sector+1) % FATFS_FAT_ENTRY_NUMBER; 
		if (data_sector == start) { //looped back to the start!
			return -1;
		}
	}
#ifdef DEBUG
printf("Found [0x%04x] data sector\n", data_sector);
#endif
	*candidate = data_sector;

	return 0;
}


int import_file(FAT_RUNTIME* rt, char* filename)
{
	FILE* from_f;
	uint32_t size_copied = 0, size_current;

	int new_de_pos;
	uint16_t data_sector, previous_sector;
	uint16_t start_sector, tries;
	uint8_t data_buffer[S_SECTOR_SIZE];

	//Can we create more file to current dir?
	if ((new_de_pos = get_free_DE( rt->dir_buffer) ) == -1){
#ifdef DEBUG
printf("!No DE!\n");
#endif
		return -1;
	}

	//Start "randomly"
	if (get_free_data_sector(rt->fat, 
			rand() % FATFS_FAT_ENTRY_NUMBER, &data_sector) == -1){
#ifdef DEBUG
printf("!No data sector!\n");
#endif
		return -1;
	} 
	start_sector = data_sector;

	//Is destination valid?
	if ((from_f = fopen(filename, "rb")) == NULL){
#ifdef DEBUG
printf("!Cannot open source!\n");
#endif
		return -1;
	}

	//Copy the first sector

	size_current = fread(data_buffer, 1, S_SECTOR_SIZE, from_f);
	write_data_sector(rt->media_f, data_sector, data_buffer);
	size_copied += size_current;	

#ifdef DEBUG
printf("Data Sector [0x%04x] = Start\n", start_sector);
printf("\tWritten %d bytes\n", size_current);
#endif


	//Copy start
	while ((size_current = 
			fread(data_buffer, 1, S_SECTOR_SIZE, from_f)) != 0){
		previous_sector = data_sector;
	
		//Find next free sector
		//30% jump chance
		if (rand() % 100 > 70){
			data_sector = rand() % FATFS_FAT_ENTRY_NUMBER; 
		} else {
			data_sector = (data_sector+1) % FATFS_FAT_ENTRY_NUMBER; 
		}

		if (get_free_data_sector(rt->fat, 
								 data_sector, &data_sector) == -1){
			break;				
		} 

		write_data_sector(rt->media_f, data_sector, data_buffer);
		size_copied += size_current;	
#ifdef DEBUG
printf("Sector [0x%04x]\n", data_sector);
printf("\tWritten %d bytes\n", size_current);
#endif

		//update FAT
		rt->fat[previous_sector] = data_sector;
	}	

	//Closing the FAT Chain
	rt->fat[data_sector] = FE_END;
#ifdef DEBUG
printf("Sector [0x%04x] = END\n\n", data_sector);
printf("\tTotal written %d bytes\n", size_copied);
#endif

	//Construct the directory entry
	construct_DE( &(rt->dir_buffer[new_de_pos]), filename, 0, 
		start_sector, size_copied); 

#ifdef DEBUG
printf("DE = ");
print_directory_entry( &(rt->dir_buffer[new_de_pos]));
#endif

	//Writeback FAT
	//write_fat(rt->media_f, rt->fat);

	//Writeback Directory
	//write_data_sector(rt->media_f, rt->current_dir, (uint8_t*)rt->dir_buffer);

    write_back_runtime(rt);

#ifdef DEBUG
print_fat_debug(rt->fat);
printf("***===***\n");
#endif

	return 0;
}



int main(int argc, char** argv)
{
	uint8_t sector_buffer[S_SECTOR_SIZE];
	FILE* media_f;
	FAT_RUNTIME fat_rt;


	if (argc < 2 ){
		printf("Usage: %s <image_file_name>\n", argv[0]);
		return -1;
	}

	print_basic_info();

    //Set random seed
    srand(2357);

    //Load USFAT-based file system from a "disk media"
    if (init_runtime_from_media(&fat_rt, argv[1]) == -1){
        printf("Unable to initialize USFAT from \"%s\".\n",argv[1]);
        return 1;
    }

	//Set current directory to root
    set_directory_to(&fat_rt, FATFS_ROOT_SECTOR);
	
    //TODO: Read from prompt
	import_file(&fat_rt, "test.a");

	import_file(&fat_rt, "test.b");

	import_file(&fat_rt, "test.c");

	import_file(&fat_rt, "test.d");

    shutdown_runtime(&fat_rt);
	 
	return 0;
}
