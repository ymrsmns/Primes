#include <cstdlib>
#include <iostream>
#include <vector>
#include <stdint.h>
#include <cmath>
#include "next_prime64.h"
using namespace std;
const int koord1[55] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,191,193,197,199,211,223,227,229,233,239,241,251,257};


std::vector<uint64_t> prime;

int  checkNArr(uint64_t number){
    if (number < 2) return false;
    if (number == 2) return true;
    for (int i = 0; i < 55 ; i++){ if (number % koord1[i] == 0) {return false;} }
    return true;
}

void checkN(uint64_t number){

 int i = 1;
 uint64_t tmp, r;
   if(!checkNArr(number))
        while(i < 1000){
		i++;
                tmp = checkNArr(number+i);
                if ( tmp ) {r = number+i; prime.push_back(r); break;}
        }
}

int prime_calc_my(uint64_t from, uint64_t to){
	checkN((uint64_t)from);
        uint64_t x  = prime.front();// checkN((uint64_t)from);
        int cnt = 0;
	std::cout << "from:  " << from << std::endl;
	 std::cout << "x:  " << x << std::endl;

         while(x && x < to) {
		cnt++;
		checkN(x+1);
		x  = prime.back();
     }
}
