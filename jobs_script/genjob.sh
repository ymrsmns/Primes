#!/bin/bash

echo '68001210000000000'



shag=10000000000;
x=68001210000000000;
echo $x;
COUNTER=0;
         while [  $COUNTER -lt 100000 ]; do
             let x=x+shag
	     let y=x/shag
	     echo "$x" > ./workunits/$y
             let COUNTER=COUNTER+1
         done

for i in $( ls ./workunits); do
        cp ./workunits/$i `./bin/dir_hier_path $i`
        ./bin/create_work --appname primes --wu_name test.0.0.0.$i --wu_template templates/prime_in_wu --result_template templates/prime_out_wu $i
        mv ./workunits/$i ./workunits_send/$i
	echo "Add $i"
## ./bin/create_work -appname primes -wu_name test.0.0.0.21 -wu_template templates/prime_in_wu -result_template templates/prime_out_wu in.txt
##--wu_template templates/prime_in_wu --result_template templates/prime_out_wu $i
done

