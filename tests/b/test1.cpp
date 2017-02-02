#include <cstdlib>
#include <iostream>
#include <vector>
#include <stdint.h>
#include <cmath>
#include "next_prime64.h"
using namespace std;
const int koord1[4] = {2,3,5,7};
const int koord2[51] = {11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,191,193,197,199,211,223,227,229,233,239,241,251,257};


std::vector<uint64_t> prime;

// задание дл€ обсчета интервала
typedef struct {
	uint64_t from; // начало интервала
	uint64_t to; // конец интервала
	int cnt; // кол-во простых
	int state; // состо€ние
} interval_t;



int checkN(uint64_t number){

    if (number < 2) return false;
//    if (number == 2) return true;
    for (int i = 0; i < 4 ; i++){ if (number % koord1[i] == 0) {return false;} }
    for (int i=0; i<50;i++){ if (number % koord2[i] == 0) {return false;}    }
    return number;
}


int prime_calc_My(uint64_t from, uint64_t to){
        uint64_t x =  checkN(from);
        int cnt = 0;
	std::cout << "from:  " << from << std::endl;
	 std::cout << "x:  " << x << std::endl;
         while(x && x < to) {
		cnt++;
		x = checkN(from+cnt);
		 std::cout << "x:  " << x << std::endl;
                if(x > 0)  std::cout << "x:  " << x << std::endl;

         }
        return cnt;

}


int main()
{
  // store the primes below 1000
  std::vector<int> primes;
  uint64_t from = 50000000000000001 ;
  uint64_t to = from + 100;
  int cnt;
  FILE* f = NULL;
  f = fopen("rez.txt", "wb");
  if(!f) {
	printf("Error create file '%s'\n", "rez.txt");
	return -1;
	} 
  printf("Calc primes on interval %llu..%llu\n", from, to);

  clock_t s = clock();
  int time = (int)((clock() - s) * 1000 / CLOCKS_PER_SEC);
  printf("Count %d primes. Time: %d msec. Speed: %dK/sec\n", cnt, time, cnt / (time|1));

  clock_t s2 = clock();
  cnt = prime_calc_My(from,to);
  int time2 = (int)((clock() - s2) * 1000 / CLOCKS_PER_SEC);
  printf("Count %d primes. Time: %d msec. Speed: %dK/sec\n", cnt, time2, cnt / (time2|1));

//  for(int i=0; i<prime.size(); ++i) std::cout << prime[i] << ' ';
  uint64_t n = from;
  if (checkN(n)){
	 std::cout << "Prosto:  " << n << std::endl; 
  }
  return 0;
}

