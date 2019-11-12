#! /bin/bash
echo "building tests..."
make -j > /dev/null

echo -e "\e[44mrunning test for Point...\e[m"
./PointTest
if [ "$?" -ne 0 ]; then
    echo "PointTest failed"
    echo "aborting..."
    make clean > /dev/null
    exit 1
fi

echo -e "\e[44mrunning test for RainfallSimulator...\e[m"
./RainfallSimulatorTest
if [ "$?" -ne 0 ]; then
    echo "RainfallSimulatorTest failed"
    echo "aborting..."
    make clean > /dev/null
    exit 1
fi
echo "All Clear!"
echo "cleaning up..."
make clean  > /dev/null



