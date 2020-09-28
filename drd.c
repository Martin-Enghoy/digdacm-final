#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX 100000

char *readFile(char *fileName);

// input : \Users\Marxette\Desktop\input.txt

int main() {
	FILE *fp1;
	char inFile[MAX];
	char out[MAX];
	char buffer[33];
	char * fullText = (char *) malloc(440);
	int a,i,j;
	
   //get the input file name from the user
    scanf("%[^\n]", inFile);

    // open the source file in read mode
    fp1 = fopen(inFile, "r");

    // error handling
    if (!fp1) {
        //printf("Unable to open the input file!!\n");
        strcpy(fullText,inFile);
    } else {
    	// reads and gets data from input file
    	fullText = readFile(inFile);
	}
    
   for( i=0;i<strlen(fullText);i++) //loop based on number of characters in the file
   {   		
   		a=fullText[i];//gets asccii value of the character
   		itoa(a,buffer,2); //converts the asccii value to binary
   		
   		for( j=strlen(buffer);j<8;j++)strcat(out,"0");//adds zeros infront of the converted binary to have 8bits per character
   		
   		strcat(out,buffer); //concats to the final output
   		strcat(out," ");
   }
   	printf("%s",out);
   return 0;
}

char *readFile(char *fileName) {
    FILE *file = fopen(fileName, "r");
    char *code;
    size_t n = 0;
    int c;

    if (file == NULL)
        return NULL;            // could not open file

    code = malloc(1000);

    while ((c = fgetc(file)) != EOF)
    {
        code[n++] = (char) c;
    }

    // null terminator
    code[n] = '\0';

    return code;
}
