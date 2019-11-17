#! /bin/bash

make -j > /dev/null

echo -e "\e[34;46mgenerating parallel version running result...\e[m"
echo -e "\e[34;46mrunning unit test...\e[m"
cd unit_test/

./unit_test.sh 

cd ../

cd function_test/

for num_thread in {1,2,4,8};
do
    echo -e "\e[31;47mrunning rainfall_seq with #thread = $num_thread...\e[m"
    ./function_test.sh rainfall_seq $num_thread full
done

cd ../

