#include <stdint.h>
#include <iostream>
using namespace std; 

int main() {
//unsigned int x = 4294967296;
//cout << __builtin_popcount(x) << endl;
cout << __builtin_ctzll(0) << endl;


uint64_t b =9223372036854775806;

     b = (b & 0x5555555555555555LU) + (b >> 1 & 0x5555555555555555LU);
     b = (b & 0x3333333333333333LU) + (b >> 2 & 0x3333333333333333LU);
     b = b + (b >> 4) & 0x0F0F0F0F0F0F0F0FLU;
     b = b + (b >> 8);
     b = b + (b >> 16);
     b = b + (b >> 32) & 0x0000007F;

cout << b << endl;



}
