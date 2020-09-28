/*
 *
 *	Martin Adrian C. Enghoy - 11811463 - DIGDACM
 *	Deframing after Line Coding Decode Output
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX 100000

//2nd DF

char *storingLongStr(char *str, unsigned int maxLength, unsigned int currSize);
int getPayloadSize(char payOne[], char payTwo[], int payloadSize);
//void *getPayloadSize(char payOne[], char payTwo[], int *payloadSize);


int main(){
	
	int maxLength =128;
	int currSize = 0;
	int a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q;
	int payloadSize;
	
	char *inputFromLC = malloc(MAX);
	currSize = maxLength;
	
	//Get input
	inputFromLC = storingLongStr(inputFromLC, maxLength, currSize);
	
	/* 
	 *
	 *	Start byte has 8 bits.
	 *	Payload size value (2 bytes MAX) = 16 bits
	 *	Payload with EDC (45 bytes + 1 MAX) = 405 bits
	 *	Stop byte has 8 bits.
	 *	
	 */
	int frameTotSize = 440; //437 total bits + 3;
	int LClen = strlen(inputFromLC); //1960
	int numOfFrames = LClen / 440; //5.61
	if (LClen%440 != 0){
		numOfFrames++; //6
	}

	char dframes[numOfFrames][frameTotSize];
	int indexCount = 0;
	int counter = 0;
	int frames,endOfFrameLen = LClen;
	
	
	
	for(frames = 0; frames < numOfFrames; frames++){
		
		int startFrame = 0, stopFrame = 0;
		
		
		startFrame = frames * 440;
		//printf("\n~~%d",startFrame);
		if(endOfFrameLen < 440){
			stopFrame = LClen-8;
			//printf("\n~-%d",stopFrame);
		} else {
			stopFrame = (440 * (frames+1)-8);
			endOfFrameLen -= 440;
			//printf("\n~%d",stopFrame);
			//printf("--%d",frames);
			//printf("-%d",numOfFrames);
			//printf("-%d",endOfFrameLen);
		}
		//printf("\n~%d",stopFrame);
		
		/* 
		 *
		 *	Checking for start bit and stop bit ahead.
		 *	
		 */
		char startByte[8], stopByte[8];
		strncpy(startByte,&inputFromLC[startFrame],8);
		startByte[8] = '\0';
		//printf("%s~",startByte);
		strncpy(stopByte,&inputFromLC[stopFrame],8);
		stopByte[8] = '\0';
		//printf("%s",stopByte);
		if (!strcmp(startByte,"00000001")){
			indexCount = 8;
			//startByte == "00000001"
			//printf("\nI found the start byte.");
			if (!strcmp(stopByte,"00000100")){		
				//stopByte == "00000100"
				//printf("\nI found the stop byte.");
				/* 
				 *
				 *	Initialization of acquisition of header value
				 *	for Payload size.
				 *	
				 */
				
				char firstHeader[8]; //first number of payload size
				int firstHeaderIndex = (startFrame+8);
				//printf("\n>%d",firstHeaderIndex);
				for(c=0; c<8; c++){
					
					//strncpy(firstHeader,inputFromLC,8);
					firstHeader[c] = inputFromLC[firstHeaderIndex];
					//printf("%s\n",firstHeader);
					//printf("~");
					//printf("%s\n",inputFromLC);
					//printf("-%d",count);
					firstHeaderIndex++;
					//printf("-%d",count);
				}
				firstHeader[c] = '\0';
				
				//printf("-%d",count);
				//printf("%s\n",firstHeader);
				//printf("-%d\n",indexCount);
				
				char secondHeader[8]; //second number of payload size
				int secondHeaderIndex = firstHeaderIndex; //Last iteration from previous for loop
				//printf("\n>>%d\n",secondHeaderIndex);
				for(d=0;d<8;d++){
					secondHeader[d] = inputFromLC[secondHeaderIndex];
					//printf("%s~",secondHeader);
					//printf("%s\n",inputFromLC);
					//printf("-%d",count);
					secondHeaderIndex++;
					//printf("-%d",count);
				}
				secondHeader[d]='\0';
				//printf("-%d",count);
				
				//printf("%s\n",secondHeader);
				//int payLoad;
				payloadSize = getPayloadSize(firstHeader,secondHeader,payloadSize);
				//printf("\n%d", payloadSize);
				//printf("\n-%d", indexCount);
				/* 
				 *
				 *	Starting from first index of payload.
				 *	First index starts after 2nd value of header.
				 *
				 
				 
				//printf("%\n%d", payLoad*8);
				int maxLC = 405; // Maximum size of entire payload + parity byte + bits.
				int startOfParityBits = indexCount+payloadSize*8;
				int grpCount = 0; //init for number of bytes in output payload
				for(e = indexCount; e < startOfParityBits; e+=8){
					for(f = 0; f < 8; f++){
						dframes[grpCount][f] = inputFromLC[e+f];
					}
					grpCount++;
				}
				indexCount = e+8; //After iteration of last byte (parity byte)
				printf("\n~%d\n",grpCount);
			
				for(k=0; k<grpCount; k++){
					for(l=0; l<8; l++){
					printf("%c", dframes[k][l]);
					}
					printf("\n");
				}
				printf("\n");
				*/
				
				char payloadArr[frames][360];
				int payload = payloadSize*8;
				int payloadIndex = secondHeaderIndex; //Start of payload index
				
				for(m = 0; m < payload; m++){
					payloadArr[frames][m] = inputFromLC[payloadIndex];
					payloadIndex++;
				}
				payloadArr[frames][m] = '\0';
				//printf("\n--%d",payloadIndex);
				printf("\n---pl %s",payloadArr[frames]);
				//printf("\n%s",payloadArr);
				//printf("\n-%d", indexCount);
				
				/* 
				 *
				 *	Getting the parity byte and parity bits
				 *	Parity byte is the last of the payload size value.
				 *	Parity bits start after parity byte index.
				 *	Parity bits will be concatenated into initial payload
				 *
				 *	
				 *	Current tests have problems with concatenation.
				 
				
				int indexOfParityBits = indexCount+payloadSize*8;
				printf("%d\n", indexOfParityBits);
				for (g = 0; g < grpCount; g++){
					
					dframes[g][8] = inputFromLC[indexOfParityBits];
					
					//for(h = indexOfParityBits; h < (LClen - 8); h++){
						//concat(dframes[g],inputFromLC[h]);
						//strcat(dframes[g],inputFromLC[h]);
						//dframes[g][9] = inputFromLC
					//}
					indexOfParityBits++;
					dframes[g][9] = '\0';
				}
				*/
				
				int indexOfParityBits = payloadIndex; //Last iteration of payloadIndex
				//printf("\n%d",indexOfParityBits);
				//printf("\n%d", indexOfParityBits+payloadSize);
				printf("\n--%d\n", indexOfParityBits);
				char parityArr[frames][45];
				int parBitCounter = 0;
				for(n=indexOfParityBits; n<(indexOfParityBits+payloadSize); n++){
					parityArr[frames][parBitCounter] = inputFromLC[n];
					//indexCount++;
					parBitCounter++;
					printf("%d ",n);
				}
				parityArr[frames][parBitCounter] = '\0';
				//printf("\n---%d",parBitCounter);
				printf("\n---%s", parityArr[frames]);
				//printf("\n%s",parityArr);
				
				 			
				/*
				 *
				 *	Combining payloadArr and parityArr into 2d Array
				 *	dframes[frames][iteration]
				 *	
				 */
				 //printf("\n-%d", payload+payloadSize);
				 int p = 0;
				 int payloadCounter = 0;
				 printf("\n~%d",payload+payloadSize+1);
				 printf("\n");
				 for(o = 1; o < payload+payloadSize+1; o++){
				 	 if(o==1){
				 	 	dframes[frames][o-1] = payloadArr[frames][payloadCounter];
				 	 	payloadCounter++;
				 	 	//printf("\n-%s",dframes[frames]);
				 	 	printf("0");
					  } else if(o%9==0){
				 		dframes[frames][o-1] = parityArr[frames][p];
				 		p++;
				 		//printf("%c",parityArr[frames][p]);
				 		printf("2~");
					 } else if (o%9!=0){
						 dframes[frames][o-1] = payloadArr[frames][payloadCounter];
						 payloadCounter++;
						 //printf("\n--%s",dframes[frames]);
						 printf("1");
					 }
				 }
				 dframes[frames][o] = '\0';
				 printf("\n%d",o);
				 printf("\n--df %s", dframes[frames]);
				 
				/* 
				 *
				 *	Outputting of 2D Array storing payload concatenated.
				 *
				 */
				//printf("\n");
				for(i=0; i<payload+payloadSize; i++){
					//printf("%c", dframes[frames][i]);	
				}	
				//printf("~\n");
			}
		}
	}
	

	free(inputFromLC);
	inputFromLC = NULL;
	return 0;
}

/*
void *getPayloadSize(char payOne[], char payTwo[], int *payloadSize){
	int i,j,k,l,bin,sumOne=0,sumTwo=0;
	char buffer[3];
	
	//Tens value of Payload Size (1st number)
	bin = 128;
	for (i=0; i<8; i++){
		printf("%d~",sumOne);
		if(payOne[i]=='1'){
			sumOne += bin;
			printf("%d<",sumOne);
		}
		bin = bin/2;
	}	
	
	buffer[0] = sumOne; //'1' -> int 1;
	
	
	//Ones value of Payload Size (2nd number)
	bin = 128;
	for (k=0; k<8; k++){
		if(payTwo[k]=='1'){
			sumTwo += bin;
		}
		bin = bin/2;
	}
	
	buffer[1] = sumTwo;
	
	printf("%d", buffer[0]);
	printf("\n%d", buffer[1]);
	printf("\n~%s",buffer);
	payloadSize = atoi(buffer);
}
*/


int getPayloadSize(char payOne[], char payTwo[], int payloadSize){
	int i,j,k,l,bin,sumOne=0,sumTwo=0;
	char buffer[3];
	
	//Tens value of Payload Size (1st number)
	bin = 128;
	for (i=0; i<8; i++){
		if(payOne[i]=='1'){
			sumOne += bin;
		}
		bin = bin/2;
	}	
	
	buffer[0] = sumOne; //'1' -> int 1;
	
	
	//Ones value of Payload Size (2nd number)
	bin = 128;
	for (k=0; k<8; k++){
		if(payTwo[k]=='1'){
			sumTwo += bin;
		}
		bin = bin/2;
	}
	
	buffer[1] = sumTwo;
	buffer[2] = '\0';
	
	//printf("%s",buffer);
	
	return payloadSize = atoi(buffer);
}


void concat(char p[], char q[]){
	int c=0, d=0;
	
	while (p[c] != '\0'){
		c++;
	}
	
	while (q[d] != '\0'){
		p[c] = q[d];
		d++;
		c++;
	}
	p[c] = '\0';
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


