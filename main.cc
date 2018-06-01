/*
 * main.cc
 *
 *  Created on: May 30, 2018
 *      Author: ankkitabose
 */
#include <pthread.h>
#include <iostream>
#include <cstdint>
#include <vector>
#include <fstream>
#include <string>
#include <cerrno>
#include "Chunk.h"
#include "Pipe.h"

const int PIPE_SIZE = 50;
//const int FILEPATH_SIZE = 50;
const int WINDOW_DEFAULT = 32768;
const int THREAD_COUNT = 1;

using namespace std ;
pthread_mutex_t mtx;
pthread_mutex_t mtx1;
pthread_mutex_t mtx2;

int64_t hashInt = 0;

class ThreadData {
public:
	Pipe* input_pipe;
  string filename;
  //char filename[FILEPATH_SIZE];
  int window;
};

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

void* ProducerFunction(void* tdata) {
  ThreadData* thread_data = (ThreadData*) tdata;
  int window = thread_data->window;
  Pipe* input_pipe = thread_data->input_pipe;
	ifstream ifile(thread_data->filename, std::ifstream::binary);

	if (ifile.good()) {


    int64_t index=0;
		//vector<int> test()
		while(1)
    {	vector<char> buffer (window + 1, 0);
			ifile.read(buffer.data(), window);
			streamsize s = ((ifile) ? window : ifile.gcount());
      if(s==0)
        break;
			buffer[s] = 0;

      Chunk chunk(window/8, index, (int)s); // let's say chunk size is 16, thus 2 ints

			for(int i=0;i<window;i++) {
				//cout<<buffer[i];
        if(i==s)
          break;
        else {
					chunk.PushCharacter(buffer[i],i);
          //cout<<buffer[i]<<" "<<i<<",";
        }
			}
			//chunk.ConvertToInt64(array, buffer.size()) ;
      input_pipe->Insert(&chunk);
			if(!ifile)
        break;

      index++;
    }
    ifile.close();
  }

  //cout<<"\nout of readin and inserting in the pipe";
  input_pipe->Shut();
}

void* ConsumerFunction(void* cdata) {

  ThreadData* consumer_data = (ThreadData*) cdata;
  Pipe* input_pipe = consumer_data->input_pipe;
  Chunk data;

  int64_t mask = 15;
  mask = mask<<60;

  pthread_mutex_lock(&mtx1);
  int flag = input_pipe->Remove(&data);
  if(flag);
    //cout<<"\n new chunk removed from pipe with index "<<data.getIndex();
  pthread_mutex_unlock(&mtx1);

  while(flag) {
    data.ConvertToInt64();
    //cout<<"\n INTEGER = "<<data.getIntegerList()[0];
  	int64_t* intList = data.getIntegerList();
    pthread_mutex_lock(&mtx);
    //cout<<"\nhash before : "<<hex<<hashInt;
  	for (int i=0 ; i<data.getSize(); i++) {

      int64_t x = hashInt<<4;
      int64_t z = (hashInt & mask)>>60;
      hashInt = x|z;
      hashInt  = hashInt ^ intList[i];
  	}
    //cout<<"\nhash after : "<<hex<<hashInt;
    pthread_mutex_unlock(&mtx);

    pthread_mutex_lock(&mtx1);
    //data = NULL;
    flag = input_pipe->Remove(&data);
    if(flag); {
      //cout<<"\n new chunk removed from pipe with index inside : "<<data.getIndex();
    }
    pthread_mutex_unlock(&mtx1);

  }

}

int main(int argc, char* argv[]) {
		//char filename[FILEPATH_SIZE];
    string filename;
		int window = WINDOW_DEFAULT;
    int threadCount = THREAD_COUNT;
    /*cout<<"\n"<<argc;
    for(int i=0;i<argc;i++) {
      cout<<"\n"<<argv[i];
    }*/


		cout<<"Enter filename: " ;
		getline(cin,filename) ;
		while(1) {
			cout<<"\nEnter window size (Default - 32768): ";
			cin>>window;
			if (window%8 != 0) {
        cout<<"ERROR! Has to be a multiple of 8. Try again!\n" ;
        continue;
      }
      cout<<"\nEnter no. of threads (Default - 1): ";
      cin>>threadCount;
      break;
		}

    /*Threading the producer*/

    Pipe input_pipe (PIPE_SIZE);
    ThreadData* tdata = new ThreadData;
    tdata->input_pipe = &input_pipe;
    tdata->filename = filename;
    tdata->window = window;
  	pthread_t producer;
  	pthread_create (&producer, NULL, ProducerFunction, (void*)tdata);

    if(pthread_mutex_init(&mtx1, NULL)!=0) {
      cout<<"\n Unable to initialize mutex. Exiting!!";
      return 1;
    }
    if(pthread_mutex_init(&mtx2, NULL)!=0) {
      cout<<"\n Unable to initialize mutex. Exiting!!";
      return 1;
    }
    if(pthread_mutex_init(&mtx, NULL)!=0) {
      cout<<"\n Unable to initialize mutex. Exiting!!";
      return 1;
    }
    pthread_t consumers[threadCount];

    for(int i=0; i<threadCount; i++) {
      pthread_create(&consumers[i], NULL, ConsumerFunction, (void*)tdata);
    }

    pthread_join(producer, NULL);
    for(int i=0; i<threadCount; i++) {
      pthread_join(consumers[i], NULL);
    }

    pthread_mutex_destroy(&mtx);
    pthread_mutex_destroy(&mtx1);
    pthread_mutex_destroy(&mtx2);
    cout<<"\nFinal Hash value is : "<<hex<<hashInt<<endl;

}
