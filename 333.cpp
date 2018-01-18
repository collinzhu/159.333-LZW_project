#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

struct Node{
	unsigned int prefix;
	unsigned int suffix;
	int temp;
};

class LZW {
	private:
		FILE *einput;
		FILE *eoutput;
		FILE *deinput;
		FILE *deoutput;
		static const int dataSize = 4096;
		static const int bitLimit = 12;
		static const int empty = -1;
		unsigned char tempString[500];
		char fileName[100];
		Node dictionary[dataSize];
	public:
		void openFile(int fileNumber, char* in);
		void writeFile(char* out);
		void initialDictionary();
		void writeOut(unsigned int letter);
		int getIndex(unsigned int prefix, unsigned int suffix);
		void encode();
		void openEncodeFile();
		unsigned int readIn(FILE *ok);
		void decode();
		unsigned char* findIndex(unsigned char *string,unsigned int temp);
		void closeFile();
		~LZW();
};



void LZW :: openFile(int fileNumber, char* in){
	if(fileNumber < 2 ){
		cout << "wrong format" << endl;
	}
	if((einput = fopen(in,"rb")) == NULL){
		cout << "can not open file! " << endl;
		exit(0);
	}
}

void LZW :: writeFile(char* out){
	int j = 0, i = 0;
	while(out[i] != '.'){
		fileName[j] = out[i];
		i++;j++;
	}
	fileName[j] = '\0';
	strcat(fileName, ".hz");
	eoutput = fopen(fileName,"wb");
	if(eoutput == NULL){
		printf("can not open this file!");
		exit(10);
	}
}

void LZW :: initialDictionary(){
	int i;
	for(i = 0; i < dataSize; i++){
		dictionary[i].temp = empty;
	}
}

void LZW :: writeOut(unsigned int letter){
	static unsigned long eoutputBuffer = 0L;
	static int count = 0;
	eoutputBuffer |= (unsigned long)letter << (32 - bitLimit - count);
	count += bitLimit;
	while (count >= 8){
		fputc(eoutputBuffer >> 24, eoutput);
		eoutputBuffer <<= 8;
		count -= 8;
	}
}

int LZW :: getIndex(unsigned int prefix, unsigned int suffix){
	int i;
	i = prefix % dataSize;
	while(true){
		if(dictionary[i].temp == empty){
			return i;
		}
		if((dictionary[i].prefix == prefix)&&(dictionary[i].suffix == suffix)){
			return i;
		}
		i++;
		if(i >= dataSize){
			i -= dataSize;
		}
	}
	return i;
}

unsigned char* LZW :: findIndex(unsigned char *string, unsigned int temp){
	while (temp > 255){
		*string++ = dictionary[temp].suffix;
		temp=dictionary[temp].prefix;
	}
	*string = temp;
	return string;
}

void LZW :: encode(){
	unsigned int i;
	unsigned int prefix;
	unsigned int suffix;
	unsigned int tempi = 256;
	initialDictionary();
	prefix =  fgetc(einput);
	while((suffix = fgetc(einput)) != (unsigned)EOF){
		i = getIndex(prefix, suffix);
		if(dictionary[i].temp == empty){
			if(tempi <= (dataSize - 2)){
				dictionary[i].prefix = prefix;
				dictionary[i].suffix = suffix;
				dictionary[i].temp = tempi;
				tempi++;
			}else{
				initialDictionary();
			}
			writeOut(prefix);
			prefix = suffix;
		}else{
			prefix = dictionary[i].temp;
		}
	}
	writeOut(prefix);
	writeOut((dataSize-1));
	writeOut(0);
}

void LZW :: openEncodeFile(){
	if((deinput = fopen(fileName, "rb")) == NULL){
		cout << "can not open file" << endl;
		exit(0);
	}
	if((deoutput = fopen("hz.txt","wb")) == NULL){
		cout << "can not open file" << endl;
		exit(0);
	}
}

unsigned int LZW :: readIn(FILE *ok){
	static int count = 0;
	static unsigned long buffer=0L;
	unsigned int result;
	while (count <= 24){
		buffer |= (unsigned long) getc(ok) << ( 24 - count);
		count += 8;
	}
	result = buffer >> (32 - bitLimit);
	buffer <<= bitLimit;
	count -= bitLimit;
	return result ;
}

void LZW :: decode(){
	unsigned int suffix;
	unsigned int prefix;
	unsigned int tempi;
	unsigned int letter;
	unsigned char *array;
   tempi=256;           
   prefix=readIn(deinput); 
	putc(prefix,deoutput);  
   letter = prefix;          
        
	while ((suffix = readIn(deinput)) != 4095){
		if (suffix < tempi){
			array = findIndex(tempString,suffix);
		}else{
			*tempString = letter;
			array = findIndex(tempString+1,prefix);
		}
		letter = *array;
		while (array >= tempString){
			putc(*array--,deoutput);
		}
		if (tempi <= 4094){
			dictionary[tempi].prefix = prefix;
			dictionary[tempi].suffix = letter;
			tempi++;
		}else{
			initialDictionary();
		}
		prefix=suffix;
	}
}
void LZW :: closeFile(){
	fclose(einput);
	fclose(eoutput);
}
LZW::~LZW(){
	fclose(deinput);
	fclose(deoutput);
}
int main (int argc, char* argv[]) {
	LZW obj;
	obj.openFile(argc,argv[1]);
	obj.writeFile(argv[1]);
	obj.encode();
	obj.closeFile();
	obj.openEncodeFile();
	obj.decode();

}

