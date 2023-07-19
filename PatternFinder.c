#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

/* Program is used to search 8bit or 16 bit pattern through a binary file
 * Each matched location offset is printed
 * Written by AJ 
 * date 19-07-2023
 */

void searchInBinary(FILE *f,  uint16_t pt, int nos_byte);
void printElements (unsigned int *offset_start , int max_elements_sz );
int ELEMENT_COUNT = 0;
int MAX_ELEMENTS_SZ = 0 ;
int visible = 0 ;								// visible byte by byte of binary file (flag)

#define OFFSET_ELEMENT_SZ sizeof(unsigned int)			// 'offset location' size required defined

void insertAnElement (unsigned int *offset_start , unsigned int value);

int main(int argc, char *argv[])
{

	/* Help Instructions */
	switch (argc){
		case 1:
			printf("\n\tBinary source file not given\n");
		case 2:
			printf("\n\tHex Pattern not given\n");
			printf("\nEnter Command line arguments (1)source (2)pattern \n");				//   (3)nos_bytes
			printf("\nOptionally you can specify nos bytes in (3) argument \n");				
			printf("\nOptions: 1 -for choosing 1 byte search , 2 -for searching 2byte, v -for visible hex pattern\n");				
			return 10;						// return 10 means some args missing 
	}
	/* Binary File input guard */
	FILE *f = fopen(argv[1],"rb");
	if ( f == NULL ){
		printf("\nCould not Open file \'%s\' \n\n",argv[1]);
		return 20;
	}
	else{
		printf("\n!!..\tFile \'%s\' opened\t..!!\n\n",argv[1]);
	}

	/* Command line arg2 hex value input guard */
	char *ptr;
	uint16_t pt = strtoul(argv[2],&ptr,16) ; 			// Coverting the 'hex string' into unsigned long

	if (pt == 0 && (strlen(ptr) != 0)){
		printf("\n\n!! Ambigous Hex Value Recieved  !!\n\n");
		return 25;						// return 25 means the pattern that provided is not valid
	}

	/* Optional Character use described, finding the number of bytes used */
	uint8_t nos_search_byte ; 
	nos_search_byte = pt > 0xff ? 2 : 1;
	char optional_char;

	if (argc == 4 ){
		optional_char  = *argv[3];
		switch ( optional_char ){
			case 'v':
				visible = 1;
				break;
			case '1':
				nos_search_byte = 1;
				break;
			case '2':
				nos_search_byte = 2;
				break;
		}
	}

	/* Finding the memory size of given binary file */
	fseek(f,0,SEEK_END);
	printf("\n\nSize of Binary File: %ld bytes\n\n",ftell(f) );
	fseek(f,0,SEEK_SET);
	/* ------------- */	

	searchInBinary(f, pt , nos_search_byte );
	return 100;								// return 100 means succesful execution, (100 is just a random number)
}

void searchInBinary(FILE *f,  uint16_t pt , int nos_byte){

	/* Search for pattern in a binary file, pattern can be 8bit or 16bit */
	uint16_t patternHex = pt;
	uint8_t hex1;
	/* dynamic memory allocation */
	unsigned int* offset_start = (unsigned int*) malloc ( OFFSET_ELEMENT_SZ );

	uint8_t byteCount  = 1;
	uint8_t match = 0;
	while( fread(&hex1,1,1,f ) ){					// read from file byte byte, each byte stored in hex1
		if (visible)
			printf("%x ",hex1);
		if ( nos_byte == 2)
			pt >>= 16-8*byteCount;				
		if ( hex1 == (uint8_t)pt ){				// compare lower 8 bits with bits 8 bits in file
			match = 1;
			while (byteCount < nos_byte){
				pt = patternHex;				
				fread(&hex1,1,1,f );
				if (visible)
					printf("%x ",hex1);

				if ( hex1 != (uint8_t)pt ){
					match = 0;
					break; }
				pt >>= 16-8*byteCount;
				pt = patternHex;
				byteCount++;
			}
		}
		if (match){
			if(visible)
				printf("(__match__) ");
			/* offset_start[ match_count++ ] = ftell(f) - nos_byte; */
			insertAnElement( offset_start, ftell(f) - nos_byte)  ;
			match = 0;
		}
		byteCount = 1;
		pt = patternHex;
	}
	printElements(offset_start, MAX_ELEMENTS_SZ);
}


void printElements (unsigned int *offset_start , int max_elements_sz ){
	/*
	 * First Arguments 'Memory start pointer' allocated by malloc
	 * Arg2  is the max number of match found that is written in heap area
	 *
	 */
	printf("\n\nNumber of match found: %d\n\n",MAX_ELEMENTS_SZ );
	printf("\n\n-----------------------Offset Table-----------------------\n\n");
	int element_count = 0;
	while ( element_count < max_elements_sz ){
		int val = offset_start[element_count];
		printf("%#x\n",val);
		element_count++;
	}
}

void insertAnElement (unsigned int *offset_start , unsigned int value){
	/*
	 * Function Arguments 
	 * First Arguments 'Memory start pointer' allocated by malloc
	 * Value to be appended ( value => offset location each match )
	 */
	offset_start[MAX_ELEMENTS_SZ] = value;
	offset_start = realloc(offset_start,OFFSET_ELEMENT_SZ + MAX_ELEMENTS_SZ * OFFSET_ELEMENT_SZ );
	MAX_ELEMENTS_SZ++;
}
