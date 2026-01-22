#include <vector>
#include <cstdio>
#include <cstring>
#include <algorithm>
void Errorif(bool eq, const char* message = "Wrong answer!");
struct Reader{
    unsigned int ReadU(void){
        unsigned int x;
        scanf("%u",&x);
        return x;
    }
}Rea;
struct Writer{
    void WriteU(unsigned int x){
        printf("%u ",x);
    }
}Wri;
int type;
void Errorif(bool eq, const char* message){
    if(eq){
        Wri.WriteU(2);
        if(message != NULL){
            int length = strlen(message);
            Wri.WriteU(length);
            for(int i = 0; i < length; ++ i)
                Wri.WriteU(message[i]);
        }
        exit(0);
    }
}
const int maxT = 105, maxK = 8, maxN = 32;
std::vector<int> rec;
struct Mess{
    int K, N, round, number;
    int received[10];
    unsigned int re;
    Mess(){}
    void input(void){
        K = Rea.ReadU();
        N = Rea.ReadU();
        round = Rea.ReadU();
        number = Rea.ReadU();
        for(int i = 0; i < round; ++ i)
            received[i] = Rea.ReadU();
        re = Rea.ReadU();
    }
    void output(void){
        Wri.WriteU(K);
        Wri.WriteU(N);
        Wri.WriteU(round);
        Wri.WriteU(number);
        for(int i = 0; i < round; ++ i)
            Wri.WriteU(received[i]);
        Wri.WriteU(re);
    }
}Ms[maxT * (maxK + 1) * maxN];
int Rand(int i){
    return rand() % i;
}
int main(){
    srand(114514);
    type = Rea.ReadU();
    Errorif(type < 1 || type > 2);
    if(type == 1){
        int T = Rea.ReadU();
        for(int id = 0; id < T; ++id)Ms[id].input();
        std::random_shuffle(Ms, Ms + T, Rand);
        Wri.WriteU(1);
        Wri.WriteU(T);
        for(int id = 0; id < T; ++id)
            Ms[id].output();
    }
    else if(type == 2){
        Wri.WriteU(type);
        int length = Rea.ReadU();
        Wri.WriteU(length);
        for(int i = 0; i < length; ++i)
            Wri.WriteU(Rea.ReadU());
    }
    return 0;
}