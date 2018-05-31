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
		int noOfInts;
		int64_t chunkIndex;
		int actualSize;
		int64_t* integerList;
		char* charList;
public:
		Chunk (int size, int64_t index, int actual) {
			this->noOfInts = size;
			this->chunkIndex = index ;
			this->actualSize = actual;
			integerList = new int64_t[size];
			charList = new char[actual];
		}

		Chunk() {};

		~Chunk () {};
		void ConvertToInt64() ; //updates the integer vector
		int getSize() {	return noOfInts; }
		int getIndex() {	return chunkIndex; }
		int getActualSize() {	return actualSize;	}
		void Consume(Chunk *chunk);
	  int64_t* getIntegerList() {	return integerList; }
		int PushCharacter(char c, int ind);
};

#endif /* CHUNK_H_ */
