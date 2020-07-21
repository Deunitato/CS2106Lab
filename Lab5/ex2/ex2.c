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



void  makeSaveName(char* name, char* charReturn){
	char newname[8];
	char extension[3];
	int nameCount = 0;
	int extensionCount = 0;
	int reachExt = 0;
	//count occupied
	for(int i = 0 ; i < strlen(name) ; i ++){
		if((name[i] == NULL) || name[i] == ' ') continue; //empty

		if(name[i] == '.'){ //reach extension
		reachExt = 1;
		continue;
		}

		if(reachExt == 1){ //extension
			extension[extensionCount] = name[i];
			extensionCount++;
			continue;
		}
		else{
			newname[nameCount] = name[i];
			nameCount++;
		}

	}
	int j =0;
	//make the name here
	char returnName[12];
	for(int i = 0 ; i < 8; i ++){ //write the title here
		if(i < (8 - nameCount)){
         returnName[i] = ' '; 
		 continue;
		}
		
		returnName[i] = newname[j];
		j ++;
	}

	for(int i = 8; i < 11; i ++){//add extension name here
	    if((i-8)<extensionCount){
			returnName[i] = extension[i-8];
			continue;
		}
		else{
			returnName[i] = ' ';
		}
	}
	returnName[11] = '\0';
	//printf("SaveName:%s\n", returnName);
	
	strcpy(charReturn,returnName);

}

/*
These are check functions, returns -1 if it fails the check,
1 if it passes
*/
//checks if the file can be open
int fileOpenable(char* filename){
	FILE *file;
    if ((file = fopen(filename, "r")))
    {
        fclose(file);
        return 1;
    }
    return -1;
}

//checks if the file already exist in the data
int fileExist(FAT_RUNTIME* rt, char* filename){
	FAT_DE *deArr = rt->dir_buffer;
	char tempName[12];
	int sectors = 8;

	makeSaveName(filename,tempName);
	for(int i = 0 ; i < sectors ; i ++){ //loop
	  char* name = (deArr + i) -> name;
	  if(strncmp(name, tempName, strlen(tempName)) == 0){return 1;}
	}

	return -1;
}

int isRootFree(FAT_RUNTIME* rt){
	FAT_DE *deArr = rt->dir_buffer;
	int sectors = 8;
	for(int i = 0 ; i < sectors ; i ++){ //loop
	  if(DE_IS_FREE(deArr[i])){return 1;};
	}

	return -1;
}


uint16_t findFreeSector(uint16_t start, FAT_RUNTIME* rt){

	uint16_t pointerSector = start;
	do{
		if(pointerSector == FE_FREE) {return pointerSector;} //iterate till find
		pointerSector = pointerSector + 1;

		//loop 
		if(pointerSector == 129){
			pointerSector = 0;
		}

	} while(pointerSector!=start);

	return -1;
}

int import_file(FAT_RUNTIME* rt, char* filename, uint16_t try_sector)
//Import a file content into USFAT based filesystem
{

    //TODO: Add your code here
	//init code
	uint16_t pointerSector = try_sector;
	int halfcopy = 0;
	FAT_DE *de;
	de = malloc(sizeof(FAT_DE));

	//do checks
	if(fileOpenable(filename) == -1) return -1;
	//if(fileExist(rt,filename) == -1) return -1; 
	//if(isRootFree(rt) == -1) return -1;

	//open the file furst
	FILE *file;
    file = fopen(filename, "r");
	if((pointerSector = findFreeSector(pointerSector,rt)) == -1) return -1; //get start sec

	struct stat sb;

	if (stat(filename, &sb) == -1) {
		perror("stat");
	}
	else {
		printf("\nFile size:                %lld bytes\n",
		(long long) sb.st_size);
	}
	uint32_t fileSizeTotal = sb.st_size;
	uint32_t tobeCopiedSize = fileSizeTotal;
	construct_DE(de,filename, 1, pointerSector,tobeCopiedSize);

	

	while(1){
		uint8_t buffer[S_SECTOR_SIZE];


		/*Read the file that is going to be imported*/
		printf("1");
		__read_sector(file, 0, buffer); //read the file

		
		/*get the next free sector*/
		uint16_t temp = findFreeSector(pointerSector,rt); 
		if(temp == -1) { //check if there is anymore free
			halfcopy = 1;
			break;
		}

		/*Write to data sector*/
		write_data_sector(rt->media_f, pointerSector, buffer);
		tobeCopiedSize = tobeCopiedSize - S_SECTOR_SIZE;
		(rt->dir_buffer)[pointerSector] = *de;


		/*Write fat entries*/
		(rt->fat)[pointerSector] = temp;
		pointerSector = temp; //move to the next

		/*Clean up if this is last iteration*/
		if(tobeCopiedSize <=0){ //there is no more after this
			(rt->fat)[pointerSector] = FE_END; //set the last sector as end
			break;
		}
	}

	
	
	//flush
	write_back_runtime(rt);

	//not copied finish
	if(halfcopy){
		// return the copied size
		return fileSizeTotal - tobeCopiedSize;
	}
	return 1;
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
