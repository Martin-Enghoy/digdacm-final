#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX 100000

char writeToFile(char text);
char *storingLongStr(char *str, unsigned int maxLength, unsigned int currSize);

int main(){
	char * input = malloc(MAX), * output = malloc(MAX);
	char out[MAX];
	char tempArr[245];
	char buffer[33];
	int i,j,k,sum,bin,counter=0,fullLength,maxLength=128,currSize=0;
	
	input = storingLongStr(input, maxLength, currSize);
    
    fullLength = strlen(input);                     //counts string length
    printf("\n~%d",fullLength);
	if(fullLength%8==0){
	
	//DIY binary to decimal
	for ( i=0; i<fullLength; i+=8){
		bin = 128;
		for (j = i; j < i+8; j++){
			if(input[j]=='1'){
				sum += bin;
			}
			bin = bin/2;
		}
		
		//out[counter] = sum; //puts decimal value into array
		tempArr[counter] = sum;
		printf("\n~~%d",counter);
		//strcat(output, buffer);
		printf("\n~%d",sum);
		//printf("~%s",output);
		//printf("\n---%c",out[counter]);
		printf("\n---%c",tempArr[counter]);
		printf("\n----%s",tempArr);
		counter++;
		sum = 0; //resets ascii value
	}
	output[counter] = '\0';
	
	
	//printf("\n%s", output); //outputs decimal to corresponding ascii
	
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

