#ifndef __USFAT_H__
#define __USFAT_H__

#include <stdint.h>		//For various unitX_t definitions

/********************
Storage Related
- All Constants start with S_ prefix
********************/

#define S_SECTOR_SIZE 256	//256 bytes per sector
#define S_FORMAT_MAGIC 0x21	//Value stored in unused data sectors, i.e.
							// when the storage is formatted
							//Traditionally should be 0xF6, USFAT use
							// 0x21 ('!') so that it is visible when
							// printed as char.

//Read/Write raw sector
int __read_sector(FILE*, uint16_t, uint8_t[]);
int __write_sector(FILE*, uint16_t, uint8_t []);


//Wrapper for the above. Sector number is offset w.r.t. to
// the start of data region
int read_data_sector(FILE*, uint16_t, uint8_t[]);
int write_data_sector(FILE*, uint16_t, uint8_t[]);
void print_sector_debug(uint8_t *);

/********************
FAT File System Related
- All Constants start with FATFS_ prefix
********************/

#define FATFS_FAT_SECTOR_COUNT 1	//How many sector(s) used by FAT
#define FATFS_FAT_ENTRY_SIZE 16		//16-bit per FAT Entry, i.e. FAT16

#define FATFS_ROOT_SECTOR 5			//location of the root sector in the data region

//Number of entry in the FAT 
#define FATFS_FAT_ENTRY_NUMBER (FATFS_FAT_SECTOR_COUNT * S_SECTOR_SIZE / (FATFS_FAT_ENTRY_SIZE / 8)) 


//Read / Write FAT Table 
int read_fat(FILE*, uint16_t*);
int write_fat(FILE*, uint16_t*);
void print_fat_debug(uint16_t *);

/********************
FAT Table Entry Related
- All Constants start with FE_ prefix
********************/

#define FE_FREE 0xFFFA
#define FE_BAD  0xFFF7
#define FE_END  0xFFFF

//Macro to check the flag stored in a fat entry 
#define FE_IS(FAT_ENTRY,FE_FLAG) ((FAT_ENTRY & 0xFFFF) == FE_FLAG)   

/********************
Directory Entry Related
- All Constants start with DE_ prefix
********************/

//Number of directory entry in a sector
#define DE_NUMBER_IN_SECTOR (S_SECTOR_SIZE / 32)

//File/Dir Attributes, only bit 0, 1, 2, 4 are used 
#define DE_ATTR_READ 0x01
#define DE_ATTR_HIDDEN 0x02
#define DE_ATTR_SYSTEM 0x04
#define DE_ATTR_DIRECTORY 0x10

//Macro to check file/dir attribute
#define DE_ATTR_IS(DE_ATTR,DE_ATTR_MASK) ((DE_ATTR & DE_ATTR_MASK) != 0)   

//Special value for first byte in filename
#define DE_FN_END 0x00   //available and all subsequent DEs free
#define DE_FN_DELETED 0xE5   //DE deleted

//Macro to check the availability of a DE
#define DE_IS_FREE( DE ) (DE.name[0] == DE_FN_END || DE.name[0] == DE_FN_DELETED)

//Each Directory Entry is 32-byte long
typedef struct {
    uint8_t name[11];	//8+3 format "ABCDEFGH.XYZ"
    uint8_t attribute;
    uint8_t dummy[14];	//these 14 bytes not in use for USFAT 
    uint16_t start_sector;	
    uint32_t file_size;
} FAT_DE;


//Convert "normal file name" to short file name
void to_sfn(char*, char*);
void construct_DE(FAT_DE*, char*, int, uint16_t, uint32_t);
void print_directory_entry(FAT_DE*);
void print_directory(FAT_DE []);

/*******************
FAT Runtime Information
- Infomration kept during runtime to facilitate
  accessing a FAT FS based storage
*******************/

typedef struct {
	FILE* media_f;	//The disk media
	uint16_t fat[FATFS_FAT_ENTRY_NUMBER];   //FAT loaded from disk media
	uint16_t current_dir;   //The current directory
	FAT_DE dir_buffer[DE_NUMBER_IN_SECTOR];   //Cache for current dir's entries
} FAT_RUNTIME;

void print_basic_info();

int init_runtime_from_media(FAT_RUNTIME*, char []);
void shutdown_runtime(FAT_RUNTIME*);

int write_back_runtime(FAT_RUNTIME* );
void set_directory_to(FAT_RUNTIME*, uint16_t);

#endif
