#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <unordered_set>
#include <queue>
#include <stack>
#include <vector>
using namespace std;
struct Dictionary{
	char** dic;
   int*   code;
	int    size;
	int    max_size;
};

void insert_dic(Dictionary *dict, char* dic) {
	int i = dict->size;
	dict->dic[i] = (char *)malloc(sizeof(char) * strlen(dic) );
	dict->code[i] = i;
	dict->size++;
	strcpy(dict->dic[i], dic);
}

void init_dictionary(Dictionary* dict, int max_size) {
	dict->max_size = max_size;
	dict->size = 0;
	dict->dic = (char **) malloc(sizeof(char*) * max_size);
	dict->code = (int *) malloc(sizeof(int) * max_size);
	insert_dic(dict,"#");
	char dic[2] = "a";
	for(int i = 0; i<26; i++){
		insert_dic(dict,dic);
		dic[0]++;
	}
}

int check_dic(Dictionary* dict, char* dic) {
	//int i = dict->size;
	//dict->dic[i] = (char *)malloc(sizeof(char) * strlen(dic) );
	for(int i=0;i<dict->size;i++){
		if (strcmp(dict->dic[i],dic)==0){
			return dict->code[i];
		}
	}
	return -1;
}



void print_dict(Dictionary* dict) {
	cout<<"=================\n";
	cout<<" Code   Seq\n";
	cout<<"=================\n";
	for(int i = 0; i<dict->size; i++){
		cout<<"  "<<dict->code[i]<<"     ";
		cout<<dict->dic[i]<<endl;
	}
	cout<<"=================\n";
}

void encode(Dictionary* dict, char* text){
	char w[1000];
	char next;
	//char temp;
	for(int i=0;i<strlen(text);i++){
		sprintf(w,"%c",text[i]);
		//cout<<w<<endl;
		if((check_dic(dict,w)!=-1)){
			//insert_dic(dict,w);
			next=text[i+1];
			sprintf(w,"%s%c",w,next);
		}
		if((check_dic(dict,w)==-1))
		insert_dic(dict,w);
		w[strlen(w)-1]='/0';
		
	}
}


int main(){
	Dictionary dict;
	init_dictionary(&dict,1000);
	
	print_dict(&dict);
	//int test = check_dic(&dict, "ws");
	//cout<< test <<endl;
   //cout<<check_dic(&dict,"w");
	
	encode(&dict,"abcbcbcbc");
	print_dict(&dict);
	return 0;
}
	
	