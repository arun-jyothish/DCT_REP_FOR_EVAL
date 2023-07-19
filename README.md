///////////////////////////////////////////////////

www.dicortech.com Quality By Design
Digital Core Technologies Pvt. Ltd.
Pattern finder
Digital Core Technologies needs pattern search application to search specific pattern in a
binary file.
User can enter either 8bit or 16-bit pattern, the application needs to accept the pattern and
file as its argument and find all the occurrences of the same pattern and its offset location
and list the same.
The application needs to work like any other standard commands in Linux and it should also
provide a help option.
Sample output Data
Pattern Searched : 0xF1
No of matches found : 3
Offset locations : 12, 34 , 92

///////////////////////////////////////////////////


Mile stones:

	1. User perspective problem view	
		1. User need less interaction 
		2. Minimum interactive way 
			$./PatternFind  binaryfile "pattern"

	2. What is binary File
		1.binary files are stored in binary format
		2.binary files are executable files
		3.Text files are stored in ASCII format
		4.Generally stored in .bin format (but not necessary)
		
	3. How to Open a Binary file			
		CLI:
			xxd -b binary_file			--> dumps data in binary format
			xxd    binary_file			--> dumps data in hex format
			hexdump binary_file			--> dumps data in hex format
		GUI:
			ghex binary_file				--> dumps data in hex
			flhex binary_file				--> dumps data in hex 
		
	3. How to Open a file from C-program
		File f = fopen ("file","r");			modes -> r, r+ ,w ,a ,a+, w+
					
				code: 
	4. Create a pattern searching algorithm	

	5. Verify algorithm in binary file 

	6. Add man page to algorithm
	
-----------------------------------------------------------------------------------------------------------------
		Milestone 3 code: 
							#include <stdio.h>
							#include <stdlib.h>


							void main(int argc, char *argv[])
							{
								FILE *f = fopen("sample.txt","r");
								if ( f == NULL ){
									printf("Error while openig sample.txt\n");
								}
								char c;
								c = getc(f);
								while ( c  != EOF ){
									/* printf("%s",c); */
									putchar(c);
									c = getc(f);

								}

							}
		Inference:
			code search for document named "sample.txt"  and it prints its contents

			---------- sample.txt ----------
				
            hi_hello_			hoi_hello__
            ello
            hell
            how are you,hello

            you
-----------------------------------------------------------------------------------------------------------------

		Milestone 4 Algorithm for pattern matching: 
		
		start: input file
		1: 	read character
		2:	check matching of first character of given pattern
		3: 	if no match read next character from main file
		4: 	if matched loop through other character of given string 
		5:	if any mismatch	reset given strings index, continue main string index goto step 2 
		6:	if completely match increment found_count continue step 2 from current index of main string 

        Code for pattern search:
        -----------------------
        void findPattern( FILE *f , char* c_pat_ptr, int pattern_length ){                              // input args file stream, character pointer of search string , character length

            int pattern_found = 0;
            char c_src = getc(f);
            int i = 0;
            while ( c_src  != EOF ){
                c_src = getc(f);
                char c_pat = c_pat_ptr[i] ;
                if ( c_pat == c_src ){
                    if ( i == pattern_length ){
                        pattern_found++; }
                    i++; }
                else{
                    i = 0; }
            }
            if (pattern_found == 0){
                printf("\n\nNo pattern found .. !");
            }
            else{
                printf("\n\n%d pattern(s) found .. !",pattern_found);
            }
        }
-----------------------------------------------------------------------------------------------------------------
		Milestone 6 read input from command line: 

        1. Using arg variables in main

            int main(int argc, char *argv[])                // argc number of arguments passed in command line
                                                            // char * array points to each string ( provided as arguments )
            argc 0 ==> no args provided
            argc 1 ==> one args provided
            argc 2 ==> two args provided

            argv[0] ==> command name itself                 // char *
            argv[1] ==> first argument 
            argv[2] ==> second argument

        
-----------------------------------------------------------------------------------------------------------------
		Milestone 7 print binary data: 
		
            used fread function
		
		void printBinary(FILE *f){
			char hex;
			/* fread(f,&hex); */
			while( fread(&hex,1,1,f)){
				printf("%x",hex);
			}
		}


-----------------------------------------------------------------------------------------------------------------
		Milestone 8 search in binary data: 

        Code for pattern search:
        -----------------------
		void searchInBinary(FILE *f,  uint8_t pt){
			printf("\n\nSearching pattern: %x \n",pt);
			uint8_t hex;
			int match_count = 0;
			/* fread(f,&hex); */
			while( fread(&hex,1,1,f)){
				printf("%x ",hex);
				if ( hex == pt ){
					match_count++;
				}
			}
			printf("\n\nNumber of match found: %d\n",match_count);
		}

-----------------------------------------------------------------------------------------------------------------
