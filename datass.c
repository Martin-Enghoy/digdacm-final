#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX 100000

char writeToFile(char text);
char *storingLongStr(char *str, unsigned int maxLength, unsigned int currSize);

int main(){
	char * stream = malloc(MAX);
	char * input = malloc(MAX), * output = malloc(MAX);
	char out[MAX];
	char buffer[33];
	int i,j,k,sum,bin,counter=0,fullLength,maxLength=128,currSize=0;

	input = storingLongStr(input, maxLength, currSize);
	char * block = strtok(input, " ");
    // 01110100 01100101 01110011 01110100
	stream[0] = '\0';
	
	i = 0;
	
	    while( block != NULL ) {
	        strcat(stream, block);
	        block = strtok(NULL, " ");
	        i++;
	    }
	    //stream[i] = '\0';
	    strcpy(input,stream);
	    fullLength = strlen(input);                     //counts string length
	    
		if(strlen(input)%8==0){
		//DIY binary to decimal
		for ( i=0; i<fullLength; i+=8){
			bin = 128;
			for (j = i; j < i+8; j++){
				if(input[j]=='1'){
					sum += bin;
				}
				bin = bin/2;
			}
			
			output[counter] = sum; //puts decimal value into array
			counter++;
			//strcat(output, buffer);
			sum = 0; //resets ascii value
		}
		output[counter] = '\0';
		
		printf("%s", output); //outputs decimal to corresponding ascii
		
	/*
	for(k=0; i<counter; k++){
		printf("%c",output[k]);	
	}
	*/
	} else {
		printf("Input is not divisible by 8.");
	}
	
	return 0;
}

// Dynamic allocation for the inputted data
char *storingLongStr(char *str, unsigned int maxLength, unsigned int currSize) {
    if (str != NULL) {
        int c = EOF;
        unsigned int i = 0;

        // Accept user input until hit enter or end of file
        while ((c = getchar()) != '\n' && c != EOF) {
            str[i++] = (char) c;

            // If i reaches maximum size, realloc activated
            if (i == currSize) {
                currSize = i + maxLength;
                str = realloc(str, currSize);
            }
        }

        str[i] = '\0';
    }
    return str;
}
