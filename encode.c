/*
Biphase-S decoding for DIGDACM Project Term 3 2020
Made by Hans Ongsitco
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(){
	char input;
	int last=-1;//previous value of Encoded Biphase-S, initialized as -1
	
	//Bi-phase-S encoding algorithm
	do{
		input=fgetc(stdin);//gets a character from the input stream
		if(input=='0'){
			if(last==1){
				printf("-1+1");
				last=1;
			}
			else if(last==-1){
				printf("+1-1");
				last=-1;
				
			}
		}
		else if(input=='1'){
			if(last==-1){
				printf("+1+1");
				last=1;
			}
			else if(last==1){
				printf("-1-1");
				last=-1;
			}
		}
	}while(input!=EOF);//loops until end of file

	return 0;
}
