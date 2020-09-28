/*
 * A C program implementation of customized framing for the project in Data and Digital Communications
 * Term 3, A.Y. 2019-2020
 *
 * Input: Bits of information with Two-Dimensional Parity as the Error Detection Code (EDC)
 * Output: Set of frames (Header | Payload | Trailer)
 *
 * Author: Jan Luis Antoc
 * BS CpE, DLSU-Manila
 *
 * Last updated: September 26, 2020
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char *storingLongStr(char *str, unsigned int maxLength, unsigned int currSize);
int numOfDigits(int num);
void textToBinary(const char *text, int m, int n, char storage[m][n]);

int main() {
    // Variables needed for reading a string with an unknown length
    int maxLength = 128;
    int currSize;

    char *inputFromEDC = malloc(maxLength);
    currSize = maxLength;

    // Read the output of the EDC block
    inputFromEDC = storingLongStr(inputFromEDC, maxLength, currSize);

    // Determines the size of the 2D array for the storage of the frames
    // Calculation for the total size of the frame:
    //  Start byte = 8 bits
    //  Payload size (2 bytes MAX) = 16 bits
    //  Payload with EDC (45 bytes + 1 MAX) = 405 bits
    //  Stop byte = 8 bits
    //  Additional three bits on the parities for inputting by byte basis
    int frameTotSize = 440;
    int EDCLen = strlen(inputFromEDC);
    int groups = EDCLen / 405;
    int additionalGroup = EDCLen % 405;

    // Takes care of the instance that a block of bits has less than 44 bytes as payload
    if (additionalGroup != 0) {
        groups++;
    }

    char frames[groups][frameTotSize];
    // Index used in getting the specific bits from the output of EDC
    // Counter for moving  the starting position of reading input after each frame
    int indexInputEDC = 0;
    int counter = 0;

    // Succeeding for loops are for the insertion of bytes in the frame
    for (int k = 0; k < groups; k++) {
        int maxEDC = 405;
        // Used SOH as start byte
        int indexOfFrame = 0;

        // Lessen the maximum bits that would be read when the total payload + EDC is less than 45
        // bytes
        if (additionalGroup != 0) {
            if (k == groups - 1) {
                maxEDC = additionalGroup;
            }
        }
        // Each payload or the parity byte has 9 bits each
        int payloadEDCSize = maxEDC / 9;
        counter += maxEDC;

        char startOF[8] = "00000001";
        // Insertion of the start byte to the frame (SOH)
        for (int a = 0; a < 8; a++) {
            frames[k][indexOfFrame] = startOF[a];
            indexOfFrame++;
        }

        // Determining the binary conversion of the digits of the payload size
        int payloadSizeDigits = numOfDigits(payloadEDCSize);
        char payloadSizeChar[2][8];
        char sizeToStr[3];

        itoa(payloadEDCSize, sizeToStr, 10);

        // If the payload is less than 10, append 0 before the number
        if (payloadSizeDigits == 1) {
            char temp = sizeToStr[0];
            sizeToStr[0] = '0';
            sizeToStr[1] = temp;
        }

        textToBinary(sizeToStr, 2, 8, payloadSizeChar);

        // Creating the storage for the payload size, converted to binary
        int payloadSizeHeader = 16;
        char *payloadSizeHeaderStr;
        payloadSizeHeaderStr = (char *)malloc(payloadSizeHeader * sizeof(char));

        int index = 0;
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 8; j++) {
                *(payloadSizeHeaderStr + index) = payloadSizeChar[i][j];
                index++;
            }
        }

        // Insertion of the payload size as the header of the frame
        for (int b = 0; b < payloadSizeHeader; b++) {
            frames[k][indexOfFrame] = payloadSizeHeaderStr[b];
            indexOfFrame++;
        }

        // Frees memory allocated to the storage of the payload size as the header
        free(payloadSizeHeaderStr);
        payloadSizeHeaderStr = NULL;

        // Takes care of the payload and the parity bits (column and row)
        //payloadEDCSize = maxEDC / 9;
        char parityBits[payloadEDCSize + 8];
        int indexOfParityBits = 0;

        // Separate the column and row parities from the payload
        // Insert directly the payload to the frame
        for (int c = indexInputEDC; c < counter; c++) {
            if (((c + 1) % 9 == 0) || c >= counter - 9) {
                parityBits[indexOfParityBits] = inputFromEDC[c];
                indexOfParityBits++;
            } else {
                frames[k][indexOfFrame] = inputFromEDC[c];
                indexOfFrame++;
            }
            indexInputEDC++;
        }

        // Separation of the parity byte from the column parities
        char parityByte[8];
        int indexOfParityByte = 0;

        for (int d = indexOfParityBits - 9; d < indexOfParityBits - 1; d++) {
            parityByte[indexOfParityByte] = parityBits[d];
            indexOfParityByte++;
        }

        // Insertion of the parity byte to the frame
        for (int e = 0; e < 8; e++) {
            frames[k][indexOfFrame] = parityByte[e];
            indexOfFrame++;
        }

        // Put the last column parity to the index 0 of the parity byte position
        parityBits[indexOfParityBits - 9] = parityBits[indexOfParityBits - 1];

        // Making sure that the column parities has a bit length of divisible by 16
        // Note: The frame is in per byte basis
        int newSizeParityBits = indexOfParityBits - 8;
        int copyNewSize = newSizeParityBits;

        while ((newSizeParityBits % 8) != 0) {
            newSizeParityBits++;
        }

        char newParityBits[newSizeParityBits];
        indexOfParityBits = 0;

        // Copying the column parities and assigning 0 as an extender to make a byte for each
        // bit that will not add up to 8 bits
        for (int f = 0; f < newSizeParityBits; f++) {
            if (indexOfParityBits < copyNewSize) {
                newParityBits[f] = parityBits[indexOfParityBits];
                indexOfParityBits++;
            } else {
                newParityBits[f] = '0';
            }
        }

        // Insertion of the column parities to the frame
        for (int g = 0; g < newSizeParityBits; g++) {
            frames[k][indexOfFrame] = newParityBits[g];
            indexOfFrame++;
        }

        // Used EOT as stop byte
        char endOF[8] = "00000100";
        // Insertion of the end of the frame (EOT)
        for (int h = 0; h < 8; h++) {
            frames[k][indexOfFrame] = endOF[h];
            indexOfFrame++;
        }
    }

    for (int t = 0; t < groups; t++) {
        for (int u = 0; u < 440; u++) {
            char bit = frames[t][u];
            if (bit != '0' && bit != '1') {
                break;
            } else {
                printf("%c", bit);
            }
        }
    }

    free(inputFromEDC);
    inputFromEDC= NULL;
    return 0;
}

// For determining the number of digits in an integer
int numOfDigits(int num) {
    if (num < 10) {
        return 1;
    }
    return 1 + numOfDigits(num / 10);
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

// Binary conversion
void textToBinary(const char *text, int m, int n, char storage[m][n]) {
    for (int i = 0; i < m; i++) {
        unsigned char t = text[i];
        int number = t;
        char *binaryChar;

        binaryChar = (char *)malloc(32 + 1);

        int counter = 0;
        // Divide by 2 while the number(or the char conversion to int) is greater than 0
        while (number > 0) {
            *(binaryChar + counter) = (number % 2) + '0';
            number = number / 2;
            counter++;
        }
        strrev(binaryChar);

        // Making sure each converted character is 8 bits long
        int additionalZero = 8 - counter;
        int counterCopy = counter;
        if (additionalZero != 0) {
            for (int x = 0; x < additionalZero; x++) {
                storage[i][x] = '0';
                counter++;
            }
        }

        int idxOfStorage = additionalZero;
        for (int y = 0; y < counterCopy; y++) {
            storage[i][idxOfStorage] = binaryChar[y];
            idxOfStorage++;
        }
    }
}