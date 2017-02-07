#!/bin/bash
echo $1;
mkdir -p ./apps/primes/$1/x86_64-pc-linux-gnu
cp primes_x32 ./apps/primes/$1/x86_64-pc-linux-gnu/primes_$1_x86_64-pc-linux-gnu
./update_versions
