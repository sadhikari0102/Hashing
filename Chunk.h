/*
 * Chunk.h
 *
 *  Created on: May 30, 2018
 *      Author: ankkitabose
 */


//  a chunk is made of several integer

#ifndef CHUNK_H_
#define CHUNK_H_
#include <pthread.h>
#include <iostream>
#include <cstdint>
#include <vector>
using namespace std;

class Chunk {
private:
		int sizeOfOneChunk;
		int chunkIndex;
		int64_t *integerList;
public:
		Chunk (int size, int index) {
			this->sizeOfOneChunk = size;
			this->chunkIndex = index ;
			integerList = new int64_t[size];
		}

		Chunk() {};

		~Chunk () {};
		void ConvertToInt64(string str) ; //updates the integer vector
		int getSize() {	return sizeOfOneChunk; }
		int getIndex() {	return chunkIndex; }
		void Consume(Chunk *chunk);
	  int64_t* getIntegerList() {	return integerList; }
};

#endif /* CHUNK_H_ */
