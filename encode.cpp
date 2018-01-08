
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;

FILE * input;
FILE * output;


struct Node {
	unsigned int prefix;
	unsigned int suffix;
	int temp;
};
struct Node dictionary[4096];

int bit = 9;
int maxBits(int size) {
	return ((1 << (size)) - 1);
}

void openFile(int fileNumber,char* in) {
	if (fileNumber < 2) {
		cout << "wrong format!" << endl;
	}
	if ((input = fopen(in, "rb")) == NULL) {
		cout << "Can not open file" << endl;
		exit(1);
	
	}
}
void writeFile(char* out){
	char fileName[100];
	int j = 0, i = 0;
	while(out[i] != '.'){
		fileName[j] = out[i];
		i++;j++;
	}
	fileName[j] = '\0';
	strcat(fileName, ".hz");
	output = fopen(fileName,"wb");
	if(output == NULL){
		printf("can not open this file!");
		exit(10);
	}
}
void bufferOutput(unsigned int character)
{
	static unsigned long OutputBuffer = 0L;
	static int count = 0;
	OutputBuffer |= (unsigned long)character << (32 - bit - count);
	count += bit;
	while (count >= 8)  
	{
		fputc(OutputBuffer >> 24, output);
		OutputBuffer <<= 8;
		count -= 8;
	}
}

void setDictionary() {
	for (int i = 0; i < 4096; i++) {
		dictionary[i].temp = -1;
	}
}
int getIndex(unsigned int prefix, unsigned int suffix) {
	int i;
	i = prefix % 4096;//hash function
	return i;
}
void encode() {

	unsigned int maxSize;
	unsigned int i;
	unsigned int prefix;
	unsigned int suffix;
	unsigned int tempi = 258;
	maxSize = maxBits(9);
	setDictionary();
	prefix = fgetc(input);
	while ((suffix = fgetc(input)) != (unsigned)EOF)
	{
		i = getIndex(prefix, suffix);
		if (dictionary[i].temp != -1) {
			prefix = dictionary[i].temp;
		}
		else {
			if (tempi <= maxSize) {
				bufferOutput(prefix);
				dictionary[i].temp = tempi++;
				dictionary[i].prefix = prefix;
				dictionary[i].suffix = suffix;
				prefix = suffix;
			}else{
				if(bit < 12){
					bit++;
					maxSize = maxBits(bit);
				}else{
					bit = 9;
					maxSize = maxBits(bit);
					tempi = 258;
					bufferOutput(256);
					setDictionary();
					
				}
			}
		}
	}
	bufferOutput(prefix);
	if(tempi == maxSize){
		bit++;
	}
	bufferOutput(257);
	bufferOutput(0);
	bufferOutput(0);
	bufferOutput(0);
}



int main(int argc, char* argv[]) {
	openFile(argc,argv[1]);
	writeFile(argv[1]);
	encode();
	fclose(input);
	fclose(output);
}