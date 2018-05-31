/*
 * main.cc
 *
 *  Created on: May 29, 2018
 *      Author: ankkitabose
 */

#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cmath>

using namespace std;

int main() {

	char filename[50] ;
	ifstream fi;
	cin.getline(filename, 50);
	fi.open(filename);

	if (!fi.is_open()) {
		exit(EXIT_FAILURE) ;
	}

	char word[50] ;
	fi>>word;
	while (fi.good()) {
		cout << word << " " ;
		fi >> word;
	}

	cout<<word;

	return 0;
}




