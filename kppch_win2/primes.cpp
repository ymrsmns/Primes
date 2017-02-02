#include <primesieve.hpp>
#include <iostream>
#include <vector>

int main()
{
    // store the primes below 1000
    std::vector<int> primes;
    primesieve::generate_primes(1000, &primes);

    primesieve::iterator pi;
    uint64_t prime;

    // iterate over the primes below 10^9
    while ((prime = pi.next_prime()) < 1000000000)
        std::cout << prime << std::endl;

    return 0;
}
