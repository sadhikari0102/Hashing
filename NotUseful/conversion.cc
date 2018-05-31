/*
 * conversion.cc
 *
 *  Created on: May 29, 2018
 *      Author: ankkitabose
 */

#include<iostream>
#include<cstdint>
#include <cstdio>
#include <cinttypes>
#include <stdint.h>   // for uint32_t
#include <limits.h>   // for CHAR_BIT
#include <assert.h>
#define INT_BITS 64
using namespace std;

 int64_t rotl64 (int64_t n, unsigned int c)
{
	 int64_t mask = 15;
	 mask = mask<<60;
	 //cout<<bitset<64>(mask)<<endl;
	 int64_t x = n<<c;
	 //cout<<bitset<64>(x)<<endl;
	 int64_t z = (n & mask)>>60;
	 //cout<<bitset<64>(z)<<endl;
	 int64_t fin = x|z;
	 //cout<<bitset<64>(fin)<<endl;
	 return fin;
  //int64_t mask = (CHAR_BIT*sizeof(n) - 1);  // assumes width is a power of 2.

  // assert ( (c<=mask) &&"rotate by type width or more");
  //c &= mask;
  //return (n<<c) | (n>> (INT_BITS - c));
}

int main() {
 int64_t value;
 char ch = 'A' ;
 value = ch ;
 cout<<value<<"\n bye \n";
 value = value<<4;
 cout<<bitset<64>(value)<<endl;
 int64_t oldvalue = value;
 int64_t newVal=value;
 for (int i=0;i<8; i++) {
	 int64_t temp = rotl64(newVal, 4);
	 newVal=oldvalue^temp;
	 cout<<bitset<64>(newVal);
	 //value = value ^ oldvalue;
	// oldvalue = value;

 }
 cout<<bitset<64>(newVal)<<endl;

}

