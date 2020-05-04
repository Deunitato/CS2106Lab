#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "USFAT.h"

int __read_sector(FILE* media_f, uint16_t sector_number, uint8_t buffer[])
//Note: sector_number is _absolute_ 
//
//Constants Used:
//- S_SECTOR_SIZE 
{
	int offset = sector_number * S_SECTOR_SIZE;

	if (fseek(media_f, offset, SEEK_SET) != 0){
		return -1;
	}	

	if(fread(buffer, 1, S_SECTOR_SIZE, media_f) != S_SECTOR_SIZE){
		return -1;
	}

	return 0;
}

int read_data_sector(FILE* media_f, uint16_t data_sector_number, uint8_t
buffer[])
{
	return __read_sector(media_f, 
						 data_sector_number + FATFS_FAT_SECTOR_COUNT, 
						 buffer);
}

int __write_sector(FILE* media_f, uint16_t sector_number, uint8_t buffer[])
//Note: sector_number is _absolute_ 
//
//Constants Used:
//- S_SECTOR_SIZE 
{
	int offset = sector_number * S_SECTOR_SIZE;

	if (fseek(media_f, offset, SEEK_SET) != 0){
		return -1;
	}	

	if(fwrite(buffer, 1, S_SECTOR_SIZE, media_f) != S_SECTOR_SIZE){
		return -1;
	}

	return 0;
}

int write_data_sector(FILE* media_f, uint16_t data_sector_number, uint8_t
buffer[])
{
	return __write_sector(media_f, 
						 data_sector_number + FATFS_FAT_SECTOR_COUNT, 
						 buffer);
}

void print_sector_debug(uint8_t *data)
//Constants Used:
//- S_SECTOR_SIZE 
//
{
    int i, j;

	//print title row
    printf("\nOffset: ");
	for (i = 0; i < 16; i++){
		printf("%02d", i);
		if ((i+1)%4 == 0){
			printf(" ");
		}
	}
	printf(" Text (if printable)\n");
	for (i = 0; i < 64; i++){
		printf("-");
	}
	printf("\n");

	//print data in hex and ascii (if possible)
    for (i=0;i<S_SECTOR_SIZE;i++)
    {
        if ( i % 16 == 0){
            printf("  %04d: ", i);
        }

        printf("%02x", data[i]);
        if ((i+1) % 4 == 0) {
            printf(" ");
        }

        if ( (i+1) % 16 == 0) {
            printf("   ");
            for (j=0; j<16; j++) {
                char ch = data[i-15+j];

				//for printable characters
                if (ch > 31 && ch < 127) {
                    printf("%c", ch);
                } else {
                    printf(".");
                }
            }
            printf("\n");
        }
    }
}


int read_fat(FILE* media_f, uint16_t *FAT )
{
	if (fseek(media_f, 0, SEEK_SET) != 0){
		return -1;
	}	
	
	if (fread(FAT, 1, S_SECTOR_SIZE * FATFS_FAT_SECTOR_COUNT, media_f) != 
			S_SECTOR_SIZE * FATFS_FAT_SECTOR_COUNT){
		return -1;
	}
	
	return 0;
}

int write_fat(FILE* media_f, uint16_t *FAT )
{
	if (fseek(media_f, 0, SEEK_SET) != 0){
		return -1;
	}	
	
	if (fwrite(FAT, 1, S_SECTOR_SIZE * FATFS_FAT_SECTOR_COUNT, media_f) != 
			S_SECTOR_SIZE * FATFS_FAT_SECTOR_COUNT){
		return -1;
	}
	
	return 0;
}

void print_fat_debug(uint16_t *FAT)
//Used:
//- FATFS_FAT_ENTRY_NUMBER
{
	int i;

	//print title row
    printf("\nOffset: ");
	for (i = 0; i < 8; i++){
		printf("+%02d  ", i);
		if ((i+1)%4 == 0){
			printf(" ");
		}
	}
    printf("\n");

    //print separator
	for (i = 0; i < 48; i++){
		printf("-");
	}
	printf("\n");

    //print FAT Entries
	for (i = 0; i < FATFS_FAT_ENTRY_NUMBER; i++){
		if ( i % 8 == 0){
			printf("0x%04x: ", i);
		}

		switch( FAT[i] ){
			case FE_FREE: 
				printf("FREE ");
				break;
			case FE_BAD:
				printf("BAD! ");
				break;
			case FE_END:
				printf("END* ");
				break; 
			default:
        		printf("%04x ", FAT[i]);
				break;
		}
		if ((i+1) % 4 == 0) {
			printf(" ");
		}
		if ((i+1) % 8 == 0) {
			printf("\n");
		}
	}
}


//Convert "normal file name" to short file name
//Note that short file name, i.e. sfn is NOT null terminated
void to_sfn( char* filename, char* sfn )
//Assumption:
//- filename is valid ( only one ".", at most 8 + 3, must have extension)
{	
	int dot_pos = strlen(filename) - 1;

	while (filename[dot_pos] != '.'){
		dot_pos--;
	}

	//Clear name
	memset(sfn, ' ', 11); 

	//voodoo: copy name to [0..7], right aligned
	strncpy(&sfn[8-dot_pos], filename, dot_pos); 

	//voodoo: copy extension to [8..10], left aligned
	strncpy(&sfn[8], &filename[dot_pos+1], strlen(filename) - dot_pos - 1);
}

void construct_DE( FAT_DE* new_de, char* name, int is_dir, 
					uint16_t start_sector, uint32_t file_size)
//Assumption:
//- filename is valid ( only one ".", at most 8 + 3, must have extension)
{
	
	to_sfn(name, (char*) new_de->name);

	uint8_t attr_temp = DE_ATTR_READ;

	if (is_dir){
		attr_temp |= DE_ATTR_DIRECTORY;
	}

	new_de->attribute = attr_temp;
	new_de->start_sector = start_sector;
	new_de->file_size = file_size;
}

void print_directory_entry( FAT_DE* de_ptr)
{
	printf("%.8s.%.3s ", de_ptr->name, &de_ptr->name[8]);
    printf("%02x ", de_ptr->attribute);
	if (DE_ATTR_IS(de_ptr->attribute, DE_ATTR_DIRECTORY)){
    	printf("<dir>  ");
	} else {
    	printf("<file> ");
	}
    printf("[0x%04x] ", de_ptr->start_sector);
    printf("%6d\n", de_ptr->file_size);
}

void print_directory( FAT_DE directory[] )
//Used:
//- DE_NUMBER_IN_SECTOR
{
	int i;

    printf("  Filename     Attr     Start     Size\n");
    printf("--------------------------------------\n");
 
	for (i = 0; i < DE_NUMBER_IN_SECTOR; i++){
		if (!DE_IS_FREE(directory[i])) {
			print_directory_entry (&directory[i]);
		}
	}

}

void print_basic_info()
//Print out a few basic information of the USFAT based file system
{
	FAT_DE sample;

	printf("Sector Size = %d bytes\n", S_SECTOR_SIZE); 
	printf("Number of FAT entry = %d\n", FATFS_FAT_ENTRY_NUMBER); 
	printf("Size of FAT DE = %lu bytes\n", sizeof(sample));
	printf("Root directory is at [0x%04x] Data Sector\n", FATFS_ROOT_SECTOR); 
}


int init_runtime_from_media(FAT_RUNTIME* rt, char filename[])
//Initialize the runtime information from an USFAT media
// USFAT media is represented as a "disk image" stored in file
//Return:
//    0 for success
//   -1 for errors
{
	if((rt->media_f = fopen(filename, "rb+")) == NULL){
        return -1;
    }
	return read_fat(rt->media_f, rt->fat);  
}

int write_back_runtime(FAT_RUNTIME* rt)
//Flush the runtime information back to the "disk media"
{
    //Write back FAT
	if (write_fat(rt->media_f, rt->fat) == -1){
        return -1;
    }

    //Write back Directory Entries
	return write_data_sector(rt->media_f, rt->current_dir, 
                    (uint8_t*)rt->dir_buffer);
}

void set_directory_to(FAT_RUNTIME* rt, uint16_t dir_sector)
//Change current directory 
//Assume:
//- The dir_sector is a valid data block with directory entries
//- The run time is initialized properly
{
	rt->current_dir = dir_sector;
	read_data_sector( rt->media_f, rt->current_dir, 
			(uint8_t*)rt->dir_buffer);
}

void shutdown_runtime(FAT_RUNTIME* rt)
{
    write_back_runtime(rt);
    fclose(rt->media_f);
}
