#! /bin/bash
make -j > /dev/null

echo -e "\e[34;46mchecking sequential version...\e[m"
echo -e "\e[34;46mrunning unit test...\e[m"
cd unit_test/

./unit_test.sh 

cd ../

echo -e "\e[34;46mrunning function test...\e[m"
cd function_test/

./function_test.sh rainfall_seq 1

cd ../


echo -e "\e[34;46mchecking parallel version...\e[m"

echo -e "\e[34;46mrunning function test...\e[m"
cd function_test/

./function_test.sh rainfall_pt 2

cd ../
