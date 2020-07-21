
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include "../common/USFAT.h"

//TODO: Add helper functions if needed

int get_dot_index(char filename[]) {
    int i;
    for (i = 0; i < 9; ++i) {
        if (filename[i] == '.') return i;
    }
    return -1;
}

int get_ext(char filename[], int dot_index) {
    if (dot_index == -1) return 0;

    int x = dot_index + 1;
    int i = 0;
    while (filename[x] != '\0') {
        i++;
        x++;
    }

    if (i > 3) return 0;
    return i;
}

int get_name(int dot_index) {
    if (dot_index == -1) return 0;
    return dot_index;
}

char *get_filename(char filename[]) {
    int dot_index = get_dot_index(filename);
    int ext_size = get_ext(filename, dot_index);
    int name_size = get_name(dot_index);
    if (ext_size == 0 || name_size == 0) return NULL;

    char *file = malloc(sizeof(char) * 11);
    int i;
    for (i = 0; i < 11; ++i) file[i] = ' ';

    for (i = name_size - 1; i >= 0; --i) {
        file[i + (7 - (name_size - 1))] = filename[i];
    }

    for (i = name_size + 1; i < name_size + 1 + ext_size; ++i) {
        file[i - 1 + (8 - name_size)] = filename[i];
    }

    return file;
}

bool file_found(char file[], uint8_t name[]) {
    int i;
    for (i = 0; i < 11; ++i) {
        if (file[i] != name[i]) {
            return false;
        }
    }
    return true;
}

bool check_duplicate_file(char file[], FAT_DE *DIRECTORY) {
    int i;
    for (i = 0; i < DE_NUMBER_IN_SECTOR; ++i) {
        if (file_found(file, DIRECTORY[i].name)) {
            return true;
        }
    }
    return false;
}

int get_empty_directory_entry(FAT_DE *DIRECTORY) {
    int i;
    for (i = 0; i < DE_NUMBER_IN_SECTOR; ++i) {
        if (DE_IS_FREE(DIRECTORY[i])) { // there is a free entry
            return i;
        }
    }
    return -1;
}

int get_free_sector(uint16_t *FAT, uint16_t start) {
    uint16_t i = start;
    while (FAT[i] != FE_FREE) {
		//printf("curr=%d\n", i);
        i = (i + 1) % FATFS_FAT_ENTRY_NUMBER;
        if (i == start) {
			//printf("return=%d\n", i);
            return -1;
        }
    }
    //printf("found=%d\n", i);
    return i;
}

int get_file_size_bytes(char *filename) {
    struct stat struc;
    stat(filename, &struc);
    return struc.st_size;
}

int read_file(FILE* file, uint16_t sector_number, uint16_t size, uint8_t buffer[])
{
	int offset = sector_number * S_SECTOR_SIZE;

	if (fseek(file, offset, SEEK_SET) != 0){
		return -1;
	}	

	if(fread(buffer, 1, size, file) != size){
		return -1;
	}

	return 0;
}

int import_file(FAT_RUNTIME* rt, char* filename, uint16_t try_sector)
    //Import a file content into USFAT based filesystem
{
    //TODO: Add your code here

    // initial checks
    FILE *try_file;
    if ((try_file = fopen(filename, "rb+")) == NULL) return -1;

    FAT_DE *DIRECTORY = rt->dir_buffer;
    uint16_t     *FAT = rt->fat;
    char *entry_filename;
    int free_directory;
    int start_sector;
    int file_size;

    entry_filename = get_filename(filename);
    if (check_duplicate_file(entry_filename, DIRECTORY)) return -1;

    free_directory = get_empty_directory_entry(DIRECTORY);
    if (free_directory == -1) return -1;

    start_sector = get_free_sector(FAT, try_sector);
    if (start_sector == -1) return -1;

    file_size = get_file_size_bytes(filename);

    // start sector found
    int remaining_size = file_size;
    int sector_number = 0;
    int current_sector = start_sector;
    while (true) {
        //printf("sector=%d, remainingsize=%d", current_sector, remaining_size);
        uint8_t *buffer = malloc(sizeof(uint8_t) * S_SECTOR_SIZE);
		int read_size = remaining_size > S_SECTOR_SIZE ? S_SECTOR_SIZE : remaining_size;
		
        read_file(try_file, sector_number, read_size, buffer);
        write_data_sector(rt->media_f, current_sector, buffer);
		
        remaining_size -= read_size;
		free(buffer);
        if (remaining_size <= 0) {
            FAT[current_sector] = FE_END;
            break;
        }
		int next_sector = get_free_sector(FAT, current_sector + 1);
		if (next_sector == -1) {
			FAT[current_sector] = FE_END;
			break;
		}
		FAT[current_sector] = next_sector;
		current_sector = next_sector;
		sector_number += 1;
    }

	construct_DE(&DIRECTORY[free_directory], filename, 0, start_sector, file_size - remaining_size);
    write_back_runtime(rt);
    return file_size - remaining_size;
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
