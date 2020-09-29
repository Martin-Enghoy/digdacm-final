#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX 100000

char *readFile(char *fileName);
char *storingLongStr(char *str, unsigned int maxLength, unsigned int currSize);

// input : \Users\Marxette\Desktop\input.txt

int main() {
    char buffer[33];
    int a,i,j;

    int maxLength = 128;
    int currSize;

    char *fullText = malloc(maxLength);
    currSize = maxLength;

    // Dynamic allocation of memory for the text in the input file
    fullText = storingLongStr(fullText, maxLength, currSize);
    int textSize = strlen(fullText);
    char out[(textSize * 9)];

    for( i=0;i<strlen(fullText);i++) //loop based on number of characters in the file
    {
        a=fullText[i];//gets asccii value of the character
        itoa(a,buffer,2); //converts the asccii value to binary

        for( j=strlen(buffer);j<8;j++)strcat(out,"0");//adds zeros infront of the converted binary to have 8bits per character

        strcat(out,buffer); //concats to the final output
        strcat(out," ");
    }
    for (int k = 0; k < textSize * 9; k++) {
        char bit = out[k];
        if (bit == '0' || bit == '1' || bit == ' ') {
            printf("%c", bit);
        }
    }
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