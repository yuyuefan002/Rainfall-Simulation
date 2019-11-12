#! /bin/bash
make -j > /dev/null

echo -e "\e[34;46mrunning unit test...\e[m"
cd unit_test/

./unit_test.sh

cd ../

echo -e "\e[34;46mrunning function test...\e[m"
cd function_test/

./function_test.sh

cd ../

make clean > /dev/null
