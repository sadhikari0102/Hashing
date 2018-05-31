/*
 * Pipe.h
 *
 *  Created on: May 30, 2018
 *      Author: ankkitabose
 */

#ifndef PIPE_H_
#define PIPE_H_
#include <pthread.h>
#include <iostream>
#include <cstdint>
#include "Chunk.h"
using namespace std;

class Pipe {
private:
	Chunk *chunk;
	int totalSpace;
	int firstSlot;
	int lastSlot;
	int done;

	pthread_mutex_t mutex;
	pthread_cond_t producer;
	pthread_cond_t consumer;

public:
	Pipe(int size);
	~Pipe();

	void Insert(Chunk *data);
	int Remove (Chunk *data);
	void Shut();
};




#endif /* PIPE_H_ */
