g++ implementer.cpp main.cpp -o message -O2 -std=c++17 -static
g++ communicator.cpp -o communicator -O2 -std=c++17 -static
./message > message.tmp1
./communicator < message.tmp1 > message.tmp2
./message < message.tmp2
rm message.tmp1 message.tmp2
