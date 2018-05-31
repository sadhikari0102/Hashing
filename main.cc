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

//const int ind = 0;
//const int start = 0;

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


    int64_t index=1;
				//vector<int> test()
		while(1)
    {	vector<char> buffer (window + 1, 0);
			ifile.read(buffer.data(), window);
			streamsize s = ((ifile) ? window : ifile.gcount());
      if(s==0)
        break;
			buffer[s] = 0;
			//cout << "\nLast portion of file read successfully. " << s << " character(s) read." << endl;

      //create a chunk
      //cout<<(int)s;
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
	/*read 8 chars at a time from string
	Chunk chunk(size/8, index); // let's say chunk size is 16, thus 2 chunks
	char array[size] ;
	for (int i= start; i<str.size(); i++ ) {
		array[start-i] = str[i] ;
	}
	chunk.ConvertToInt64(string(array)) ;
	*/
}

void* ConsumerFunction(void* cdata) {

  ThreadData* consumer_data = (ThreadData*) cdata;
  Pipe* input_pipe = consumer_data->input_pipe;
  Chunk data;

  int64_t mask = 15;
  mask = mask<<60;

  pthread_mutex_lock(&mtx1);
  int flag = input_pipe->Remove(&data);
  cout<<"\n new chunk removed from pipe with index "<<data.getIndex();
  pthread_mutex_unlock(&mtx1);

  while(flag) {
    cout<<"\n new chunk removed from pipe with index "<<data.getIndex();
    //cout<<"\nchunk index = "<<data.getIndex();
    //cout<<"\nno of ints = "<<data.getSize();
    //cout<<"\n actual size "<<data.getActualSize();
    data.ConvertToInt64();
    //cout<<"\n INTEGER = "<<data.getIntegerList()[0];
  	int64_t* intList = data.getIntegerList();
    pthread_mutex_lock(&mtx);
  	for (int i=0 ; i<data.getSize(); i++) {

      //cout<<"\nhash "<<hex<<hashInt;
      int64_t x = hashInt<<4;
      int64_t z = (hashInt & mask)>>60;
      hashInt = x|z;
      hashInt  = hashInt ^ intList[i];

  	}
    pthread_mutex_unlock(&mtx);

    pthread_mutex_lock(&mtx2);
      flag = input_pipe->Remove(&data);
      cout<<"\n new chunk removed from pipe with index "<<data.getIndex();
    pthread_mutex_unlock(&mtx2);

  }

}

int main() {
		//char filename[FILEPATH_SIZE];
    string filename;
		int window = WINDOW_DEFAULT;
    int threadCount = THREAD_COUNT;
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
		//string str = get_file_contents(filename);
		//reader(filename,window);


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
    cout<<"\n Final Hash value is : "<<hex<<hashInt<<endl;
}
