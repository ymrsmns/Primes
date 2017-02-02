#include <cstdlib>
#include <iostream>
#include <vector>
#include <stdint.h>
#include <cmath>
#include "next_prime64.h"
using namespace std;
const int koord1[55] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,191,193,197,199,211,223,227,229,233,239,241,251,257};


std::vector<uint64_t> prime;

// задание дл€ обсчета интервала
typedef struct {
	uint64_t from; // начало интервала
	uint64_t to; // конец интервала
	int cnt; // кол-во простых
	int state; // состо€ние
} interval_t;

inline int pow_on_mod(uint64_t base, uint64_t degree, uint64_t mod){
  
 unsigned  result=1;
 std::cout << "for: " << degree << std::endl;
  clock_t s = clock();

  for(uint64_t i=0; i<degree; ++i){
    result = (result * base)%mod;
//    std::cout  << round((i*100)/degree)/10000. << "% "<< result << std::endl;
  }
 std::cout << "time: " << clock()-s << std::endl;
  return result;
}

//inline int pow_on_mod2(uint64_t a, uint64_t b, uint64_t m){
//	uint64_t r = 1;
//	while (b) {
//           if (b & b%2 == 1) {
//            r *= a;
//            r %= m;
//            }
//        a *= a;
//        a %= m;
//        b >>=1;
//    }
//return r;
//}

bool miller_rabin (uint64_t m) {

       if(m==2 || m==3)
             return true;

       if(m % 2 == 0 || m == 1){
             return false;
       }

       uint64_t s = 0;
       uint64_t t = m-1;
       uint64_t x=0;

       uint64_t r1 = 2;
       uint64_t r2 = m-2;
       uint64_t a;

       uint64_t r = (log(m) / log(2));

       while(t!=0 && t % 2 == 0){
 		t /=2;
		s++;
       }

       for(uint64_t  i = 0; i < r; i++) {
             a = r1 + rand()%(r2-r1);

		 std::cout << "r: " << r << std::endl;
                 std::cout << "a: " << a << std::endl;
                 std::cout << "t: " << t << std::endl;
                 std::cout << "m: " << m << std::endl;
             x = pow_on_mod(a,t,m);
             if(x==1 || x==m-1) {
                    continue;
             }
//	     std::cout << "x: " << x << std::endl;
             for(uint64_t j = 0; j < s-1; j++) {
		std::cout << "s-1: " <<  s-1 << std::endl;
                    x = pow_on_mod(x,2,m);

                    if(x == 1)
                           return false;
                    if(x == m - 1)
                                  break;
             }
  //           std::cout << "time: " << clock()-s << std::endl;
             if(x == m - 1)
                    continue;

             return false;
       }

    return true;
}

void eratosfen(unsigned int limit)
{
	unsigned int* bitmap = (unsigned int*) calloc(limit / 64 + ((limit & 63) ? 1 : 0), sizeof(unsigned int));
	//printf("2 3 ");
	prime.push_back(2);
	prime.push_back(3);
	unsigned int max_prime = 3;
	bool need_fill = true;
	while(need_fill) {
		unsigned int step = max_prime << 1;
		for(unsigned int i = max_prime * max_prime; i < limit; i += step) { // Вычеркиваем кратные max_pr
			bitmap[i >> 6] |= (1 << ((i >> 1) & 31));
		}
		if(max_prime * max_prime >= limit) need_fill = false; // дальше заполнять не надо
		// вычитываем следущую порцию
		for(unsigned int i = max_prime + 2; i < limit; i += 2) {
			if((bitmap[i >> 6] & (1 << ((i >> 1) & 31))) == 0) {
				prime.push_back(i);
				//printf("%u ", i);
				if(need_fill) {
					max_prime = i;
					break;
				}
			}
		}
	}
	free(bitmap);
}
// ferma habr - start
/// 
long long mul(long long a, long long b, long long m){
	if(b==1)
		return a;
	if(b%2==0){
		long long t = mul(a, b/2, m);
		return (2 * t) % m;
	}
	return (mul(a, b-1, m) + a) % m;
}

long long pows(long long a, long long b, long long m){
	if(b==0)
		return 1;
	if(b%2==0){
		long long t = pows(a, b/2, m);
		return mul(t , t, m) % m;
	}
	return ( mul(pows(a, b-1, m) , a, m)) % m;
}
long long gcd(long long a, long long b){//Быстрое возведение в степень по модулю
	if(b==0)
		return a;
	return gcd(b, a%b);
}
bool ferma(long long x){
	if(x == 2)
		return true;
	srand(time(NULL));
	for(int i=0;i<100;i++){
		long long a = (rand() % (x - 2)) + 2;
		if (gcd(a, x) != 1)
			return false;			
		if( pows(a, x-1, x) != 1)		
			return false;			
	}
	return true;
}

// ferma habr - end

int  checkNArr(uint64_t number){
    if (number < 2) return false;
    if (number == 2) return true;
    for (int i = 0; i < 55 ; i++){ if (number % koord1[i] == 0) {return false;} }
    if (ferma(number)) return true;
    return false;
//    if (miller_rabin(number))

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
//  std::cout << "r: " << r << std::endl;
// return r;
}


int prime_calc_My(uint64_t from, uint64_t to){
	checkN((uint64_t)from);
        uint64_t x  = prime.front();// checkN((uint64_t)from);
        int cnt = 0;
	std::cout << "from:  " << from << std::endl;
	 std::cout << "x:  " << x << std::endl;

         while(x && x < to) {
		cnt++;
		checkN(x+1);
		x  = prime.back();
  //              std::cout << "wx:  " << x << std::endl;
     }
 std::cout << "end:  " << x << std::endl;
        return cnt;

}

/// n -end
// s start
long long frequencyPrimes (long long s,long long n)                           // функция поиска простых чисел
{ //Вычисляет квадратный корень
  long long freq = n-1;
  for (long long i = s; i <= n; ++i)
      for (long long j = sqrt( (float)i ); j > 1; --j)
          if (i % j == 0)
          {
	   std::cout << i   << std::endl;
            --freq;
            break;
          }
  return freq;
}

int main()
{
  // store the primes below 1000
  std::vector<int> primes;
  uint64_t from = 50000000000000000;
  uint64_t to = from + 10;//10 000 000;
  int cnt;

  clock_t s2 = clock();
  cnt = frequencyPrimes(from,to);
//  cnt = prime_calc_My(from,to);
  //eratosfen(to);
//  miller_rabin(from+9);
  int time2 = (int)((clock() - s2) * 1000 / CLOCKS_PER_SEC);
  printf("Count %d primes. Time: %d msec. Speed: %dK/sec\n", cnt, time2, cnt / (time2|1));


//eratosfen(from);


//  for(int i=0; i<prime.size(); ++i) {
//	std::cout << prime[i] << ' '  << std::endl;
//	if (miller_rabin(prime[i] )) {std::cout << "Prosto:  " << prime[i] << std::endl;}
//  }

//  uint64_t n = prime.front();
//  if (miller_rabin(n)){ //checkN
//	 std::cout << "Prosto:  " << n << std::endl; 
// }

  return 0;
}

