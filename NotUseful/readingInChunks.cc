/*
 * readingInChunks.cc
 *
 *  Created on: May 31, 2018
 *      Author: ankkitabose
 */
#include<iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

void get_file_contents(const char* filename) {

		// check if the file to read from exists and if so read the file in chunks
		ifstream ifile(filename, std::ifstream::binary);

		if (ifile.good()) {
			const int BUFFER_SIZE = 1024;
			vector<char> buffer (BUFFER_SIZE + 1, 0);
			while(1)
			{
						string str;
						ifile.read(buffer.data(), BUFFER_SIZE);
						streamsize s = ((ifile) ? BUFFER_SIZE : ifile.gcount());

						buffer[s] = 0;
						if(!ifile) cout << "Last portion of file read successfully. " << s << " character(s) read." << endl;
						str <<  buffer.data() << endl << endl;
						if(!ifile) break;
						cout<<str;
					}
			ifile.close();
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
		get_file_contents(filename);


}
