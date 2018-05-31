#include "Pipe.h"

Pipe::Pipe(int size) {

  pthread_mutex_init (&mutex, NULL);
	pthread_cond_init (&producer, NULL);
	pthread_cond_init (&consumer, NULL);

  chunk = new Chunk[size];

  if(chunk==NULL) {
    cout<<"\nNot enough memory to process chunks";
    exit(1);
  }

  totalSpace = size;
  firstSlot = lastSlot =0;
  done=0;
}

Pipe::~Pipe() {

	delete [] chunk;

	pthread_mutex_destroy (&mutex);
	pthread_cond_destroy (&producer);
	pthread_cond_destroy (&consumer);

}

void Pipe::Insert(Chunk *data) {

	pthread_mutex_lock (&mutex);

	if (lastSlot - firstSlot < totalSpace)
    chunk [lastSlot % totalSpace].Consume (data);
  else {
    pthread_cond_wait (&producer, &mutex);
    chunk [lastSlot % totalSpace].Consume (data);
	}

	lastSlot++;

	pthread_cond_signal (&consumer);

	pthread_mutex_unlock (&mutex);
}

int Pipe::Remove(Chunk *data) {

  pthread_mutex_lock (&mutex);

  if (lastSlot != firstSlot)
    data->Consume (&chunk[firstSlot % totalSpace]);
  else {
    if (done) {
      pthread_mutex_unlock (&mutex);
      return 0;
    }

    pthread_cond_wait (&consumer, &mutex);

    if (done && lastSlot == firstSlot) {
      pthread_mutex_unlock (&mutex);
      return 0;
    }

    data->Consume (&chunk[firstSlot % totalSpace]);
  }
  firstSlot++;

  pthread_cond_signal (&producer);

  pthread_mutex_unlock (&mutex);
  return 1;
}

void Pipe::Shut() {

  pthread_mutex_lock (&mutex);
  done = 1;
  pthread_cond_signal (&consumer);
  pthread_mutex_unlock (&mutex);

}
