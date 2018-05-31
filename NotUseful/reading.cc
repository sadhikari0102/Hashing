/*
 * reading.cc
 *
 *  Created on: May 29, 2018
 *      Author: ankkitabose
 */
#include<iostream>
#include <fstream>
#include <string>
#include <cerrno>
using namespace std;

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

void print_charBychar(string str) {
	for (int i=0; i<str.size(); i++) {
		cout<<str[i]<<" "<<i<<"\n" ;
	}
}

int main() {
	char filename[50] ;
	cin.getline(filename, 50) ;
	string str = get_file_contents(filename);
	cout<<"here" ;
	print_charBychar(str);
	//cout<<str ;
	return 0;
}
