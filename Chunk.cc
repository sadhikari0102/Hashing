/*
 * Chunk.cc
 *
 *  Created on: May 30, 2018
 *      Author: ankkitabose
 */

#include <vector>
#include "Chunk.h"

using namespace std ;

void Chunk::ConvertToInt64(string str) {
	int pos = 0;
	for (int i=0 ; i <sizeOfOneChunk ; i++) {
		int64_t value=0 ;
		int shift = 56 ;
		int j;
		for(j=0; j<8; j++) {
			char ch;
			if (j+pos >= str.size())
				ch = 0;
			else
				ch= str[j+pos] ;
			value |= ch ; //check this
			value << shift;
			shift -= 8;
		}
		pos = j+pos;
		integerList[i] = value | chunkIndex;
	}
}

void Chunk::Consume(Chunk *chunk) {
	delete [] integerList;
	integerList =  chunk->integerList;
	chunk->integerList = NULL;
	sizeOfOneChunk = chunk->sizeOfOneChunk;
	chunkIndex = chunk->chunkIndex ;
}
