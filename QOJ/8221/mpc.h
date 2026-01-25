#include<array>
struct player{
    bool last_message;
    std::array<int,4096> memory;
};
int precalc(int n,int m);
bool transmit(player &player, int round,int position);