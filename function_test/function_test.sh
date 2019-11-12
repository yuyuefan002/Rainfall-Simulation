#! /bin/bash

arg_num=$#

num_thread=1

length_rain=10
absorb_rate=0.25
size=4
echo -e "\e[44mrunning ./simulator [P] $length_rain $absorb_rate $size sample_${size}x$size\e[m"
./simulator $num_thread $length_rain $absorb_rate $size sample_$size\x$size.in > my_$size\x$size.out;
head -n 2  my_$size\x$size.out
./check.py $size sample_$size\x$size.out my_$size\x$size.out

length_rain=20
absorb_rate=0.5
size=16
echo -e "\e[44mrunning ./simulator [P] $length_rain $absorb_rate $size sample_${size}x$size\e[m"
./simulator $num_thread $length_rain $absorb_rate $size sample_$size\x$size.in > my_$size\x$size.out;
head -n 2  my_$size\x$size.out
./check.py $size sample_$size\x$size.out my_$size\x$size.out

length_rain=20
absorb_rate=0.5
size=32
echo -e "\e[44mrunning ./simulator [P] $length_rain $absorb_rate $size sample_${size}x$size\e[m"
./simulator $num_thread $length_rain $absorb_rate $size sample_$size\x$size.in > my_$size\x$size.out;
head -n 2  my_$size\x$size.out
./check.py $size sample_$size\x$size.out my_$size\x$size.out

length_rain=30
absorb_rate=0.25
size=128
echo -e "\e[44mrunning ./simulator [P] $length_rain $absorb_rate $size sample_${size}x$size\e[m"
./simulator $num_thread $length_rain $absorb_rate $size sample_$size\x$size.in > my_$size\x$size.out;
head -n 2  my_$size\x$size.out
./check.py $size sample_$size\x$size.out my_$size\x$size.out

length_rain=30
absorb_rate=0.75
size=512
echo -e "\e[44mrunning ./simulator [P] $length_rain $absorb_rate $size sample_${size}x$size\e[m"
./simulator $num_thread $length_rain $absorb_rate $size sample_$size\x$size.in > my_$size\x$size.out;
head -n 2  my_$size\x$size.out
./check.py $size sample_$size\x$size.out my_$size\x$size.out

if [ "$arg_num" -ne 2 ];then
    exit 0
fi

length_rain=35
absorb_rate=0.5
size=2048
echo -e "\e[44mrunning ./simulator [P] $length_rain $absorb_rate $size sample_${size}x$size\e[m"
./simulator $num_thread $length_rain $absorb_rate $size sample_$size\x$size.in > my_$size\x$size.out;
head -n 2  my_$size\x$size.out
./check.py $size sample_$size\x$size.out my_$size\x$size.out 

length_rain=50
absorb_rate=0.5
size=4096
echo -e "\e[44mrunning ./simulator [P] $length_rain $absorb_rate $size sample_${size}x$size\e[m"
./simulator $num_thread $length_rain $absorb_rate $size sample_$size\x$size.in > my_$size\x$size.out;
head -n 2  my_$size\x$size.out
./check.py $size sample_$size\x$size.out my_$size\x$size.out 
