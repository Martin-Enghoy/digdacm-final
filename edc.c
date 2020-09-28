//
//  main.c
//  parityCalculation
//
//  Created by Marxette Diane Cabato on 9/22/20.
//  Copyright © 2020 Marxette Diane Cabato. All rights reserved.
//

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 256000

char *storingLongStr(char *str, unsigned int maxLength, unsigned int currSize);
char * parityCheck(char stream[]);
void concatenate(char p[], char q[]);

int main(void) {
    char * input = malloc(MAX), * output = malloc(MAX), temp[397], partial[MAX];
    int fullLength, i, j = 0, maxLength = 128, currSize = 0;
    
    input = storingLongStr(input, maxLength, currSize);
    
    fullLength = strlen(input);                     //counts string length
    
    temp[0] = '\0';
    output[0] = '\0';
    if (fullLength < 396) {
        strcpy(output, parityCheck(input));
    } else {
        while (fullLength > 0) {
            for(i=0; i < 396; i++) {
                strncat(temp, &input[j], 1);
                j++;
            }
            strcpy(partial, parityCheck(temp));
            strcat(output, temp);
            strcpy(temp, "");
            fullLength = fullLength - 396;
        }
    }
    
    printf("%s",output);
    
    free(input);
    free(output);
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

char * parityCheck(char stream[]) {
    int i, one, z = 0, j = 0;
    char *blocks[strlen(stream)/8];                             // blocks of data
    char straightStream[MAX];                                  // continuous data stream without separator
    char *combination = (char *) malloc(440);                    // stream to be appended with parity bits
    char parityBitsRight[MAX], parityBitsBottom[MAX];
    char *string = NULL, parityBit, newString[MAX];
    char twoD[strlen(straightStream)/8][8];                     // binary data Rows and Columns
    char * block = strtok(stream, " ");
    straightStream[0] = '\0';                                   // removes unwanted characters at the beginning
    
    // loop through the string to extract data blocks
    i = 0;
    while( block != NULL ) {
        blocks[i] = block;
        strcat(straightStream, block);
        block = strtok(NULL, " ");
        i++;
    }
    blocks[i] = NULL;
    
    combination[0]='\0';
    parityBitsRight[0]='\0';
    parityBitsBottom[0]='\0';
        
    // Right Parity Bits [ROWS]
    for (z = 0; z < strlen(straightStream)/8; z++) {
        one = 0;
        string = blocks[z];
        for (i = 0; i < 8; i++) {
            if (string[i] == '1') {
                one++;
            }
            //insert bits into 2D arrays
            twoD[z][i] = string[i];
        }
        
        strcpy(newString,string);
        
        // checks parity whether odd or even
        if (one % 2 == 0) {                                 // even: 0 parity bit
            concatenate(newString, "0");
            strcat(parityBitsRight,"0");
            parityBit = '0';
        } else {                                            // odd: 1 parity bit
            strcat(parityBitsRight,"1");
            concatenate(newString, "1");
            parityBit = '1';
        }
        
        // appends block with parity bit into stream
        strcat(combination,newString);
    }
    one = 0;
    
    // Bottom Parity Bits [COLUMNS]
    for (j = 0; j < 8; j++) {
        for (i = 0; i < strlen(straightStream)/8; i++) {
            if (twoD[i][j] == '1') {
                one++;
            }
        }
        
        if (one % 2 == 0) {
            strcat(parityBitsBottom, "0");
        } else {
            strcat(parityBitsBottom, "1");
        }
        one = 0;
    }
    
    // counts number of 1s in bottom parity bits
    for (i = 0; i < strlen(parityBitsBottom); i++) {
        if (parityBitsBottom[i] == '1') {
            one++;
        }
    }
    
    if (one % 2 == 0) {
        strcat(parityBitsBottom, "0");
    } else {
        strcat(parityBitsBottom, "1");
    }
    
    strcat(combination,parityBitsBottom);
    strcpy(stream, combination);
    
    // frees malloc
    free(combination);
    
    return stream;
}

// DIY concatenate lol
void concatenate(char p[], char q[]) {
   int c = 0, d = 0;
    
   while (p[c] != '\0') {
      c++;
   }
 
   while (q[d] != '\0') {
      p[c] = q[d];
      d++;
      c++;
   }
   p[c] = '\0';
}
