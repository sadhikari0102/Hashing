/*
 * Chunk.cc
 *
 *  Created on: May 30, 2018
 *      Author: ankkitabose
 */

#include <vector>
#include "Chunk.h"

using namespace std ;

//Question???????????????
void Chunk::ConvertToInt64() {
	int pos = 0;
	for (int i=0 ; i<noOfInts ; i++) {
		int64_t value=0 ;
		int shift = 56 ;
		int j;
		//cout<<"\n";
		for(j=0; j<8; j++) {
			//char ch;
			//cout<<j+pos<<" , ";
			if (j+pos >= actualSize || charList[j+pos]=='\0')
				value<<1;
			else {
				value|=charList[j+pos] ;
				//cout<<"\n"<<charList[j+pos];
			}
			//value |= ch ; //check this
			value << shift;
			shift -= 8;
		}
		//cout<<"\n"<<hex<<value;
		pos = j+pos;
		integerList[i] = value | chunkIndex;
		//cout<<"\nvalue - "<<integerList.size();
	}
}

int Chunk::PushCharacter(char c, int ind) {
	if(ind>=actualSize) {
		cout<<"\nChunk full";
		exit(0);
	}
	else
		charList[ind]=c;
	return 1;
}

void Chunk::Consume(Chunk *chunk) {
	delete [] charList;
	charList =  chunk->charList;
	chunk->charList = NULL;
	integerList =  chunk->integerList;
	chunk->integerList = NULL;
	noOfInts = chunk->noOfInts;
	chunkIndex = chunk->chunkIndex ;
	actualSize = chunk->actualSize;
}
