/*
 * main.cc
 *
 *  Created on: May 30, 2018
 *      Author: ankkitabose
 */
#include <pthread.h>
#include <iostream>
#include<cstdint>
#include <vector>
#include <fstream>
#include <string>
#include <cerrno>
#include "Chunk.h"
#include "Pipe.h"

using namespace std ;

string get_file_contents(const char *filename)
{
  ifstream in(filename, ios::in | ios::binary);
  if (in)
  {
    string contents;
    in.seekg(0, ios::end);
    contents.resize(in.tellg());
    in.seekg(0, ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return(contents);
  }
  throw(errno);
}

const int index = 0;
const int start = 0;

void reader(string str, int size) {
	//read 8 chars at a time from string
	Chunk chunk(size/8, index); // let's say chunk size is 16, thus 2 chunks
	char array[] ;
	for (int i= start; i<str.size(); i++ ) {
		array[start-i] = str[i] ;
	}
	chunk.ConvertToInt64(string(array)) ;
}

int64_t hash = 0;
Pipe pipe ;

void hashing() {
	// thread
	Chunk data;
	pipe.Remove(&data);

	for (int i=0 ; i<data.getSize(); i++) {
		rotate(hash) ;
		hash  = hash ^ data.integerList[i];
	}

}
int main() {
		char filename[50] ;
		int window = 32768;
		cout<<"Enter filename: = " ;
		cin.getline(filename, 50) ;
		while(1) {
			cout<<"\nEnter window size= ";
			cin>>window;
			if (window%8 == 0)
				break;
			cout<<"ERROR! Has to be a multiple of 8. Try again!\n" ;
		}
		string str = get_file_contents(filename);


}

