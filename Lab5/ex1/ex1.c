/*************************************
* Lab 5 Exercise 1
* Name: Charlotte Lim	
* Student No:
* Lab Group: 11
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
// removes all the . 
/*char* makeFileName(char* name){
	//check first 8 char
	char namde[10];
	//count occupied
	for(int i = 0 ; i < 8 ; i ++){
		if(name[i] != NULL){

		}

	}

}*/


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

int read_file( FAT_RUNTIME* rt, char filename[])
{
    //TODO: Your code here
	//trim the file name:

	int sectors = 8;
	FAT_DE *deArr = rt->dir_buffer;
	char tempName[12];

	makeSaveName(filename,tempName);

	//printf("The file:%s, with length %ld", tempName , strlen(tempName));
	for(int i = 0 ; i < sectors ; i ++){ //loop
	  char* name = (deArr + i) -> name;
	  //printf("\nThename: ");
	  //printf("%s, with length %ld",name , strlen(name));
	  if(strncmp(name, tempName, strlen(tempName)) == 0){
		  //256 bytes for each sector
		  //uint32_t sector_size = S_SECTOR_SIZE/4;
		  uint32_t fileSize = (deArr+i)->file_size;
		  uint16_t pointerSector = (deArr+i)->start_sector;
		  

		  do{
			  uint8_t buffer[S_SECTOR_SIZE];
			  read_data_sector(rt->media_f,pointerSector,buffer);
			  if(fileSize >= S_SECTOR_SIZE){
				  
				  print_as_text(buffer,S_SECTOR_SIZE);
			  }
			  else{
				  print_as_text(buffer,fileSize);
			  }
			  if(fileSize > 0){ 
				  pointerSector = (rt->fat)[pointerSector];
				  if(pointerSector == FE_END) break;
				  }
			  fileSize = fileSize - S_SECTOR_SIZE/4; // decrement sector size
		  }while(fileSize > 0);

		  
		  //print_as_text((deArr+i)->start_sector,(deArr+i)->file_size);
		  return 1;
	  }

	}

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
