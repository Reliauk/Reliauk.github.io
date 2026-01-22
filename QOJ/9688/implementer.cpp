#include "message.h"
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
    Mess(int K, int N, int round, int number, const std::vector<int>&received):
        K(K), N(N), round(round), number(number){
            for(int i = 0; i < round; ++ i)
                this->received[i] = received[i];
            re = send(K, N, round, number , received);
        }
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
    int check(void){
        rec.clear();
        for(int i = 0; i < round; ++i)
            rec.push_back(received[i]);
        return re == send(K, N, round, number, rec);
    }
}Ms[maxT * (maxK + 1) * maxN];
int adder[256];
std::vector<int>received_message[256];
int main(){
    type = Rea.ReadU();
    Errorif(type < 0 || type > 2);
    if(type == 0){
        int T = Rea.ReadU(), K = Rea.ReadU();
        int N = init(K), cn = 0;
        Errorif(N < 1 || N > 31);
        unsigned int mask = (1U << N) - 1U;
        for(int id = 1; id <= T; ++id){
            unsigned int ans = Rea.ReadU() & mask;
            for(int i = 0; i < N; ++ i){
                received_message[i].clear();
                if((ans >> i) & 1){
                    ans |= 1llu << i;
                    received_message[i].push_back(1);
                }
                else{
                    received_message[i].push_back(0);
                }
            }
            int ok = true;
            for(int round = 1; round <= K + 1; ++ round){
                for(int number = 0; number < N; ++ number){
                    Ms[cn] = Mess(K, N, round, number, received_message[number]);
                    unsigned int ret = Ms[cn].re;
                    ++ cn;
                    if(round > K)ok &= (ret == ans);
                    else{
                        for(int index = 0; index < N; ++ index){
                            if((ret >> index) & 1){
                                ++ adder[index];
                            }
                        }
                    }
                }
                for(int index = 0; index < N; ++ index){
                    received_message[index].push_back(adder[index]);
                    adder[index] = 0;
                }
            }
            Errorif(!ok, "Wrong answer!");
        }
        Wri.WriteU(1);
        Wri.WriteU(cn);
        for(int i = 0; i < cn; ++ i)
            Ms[i].output();
    }
    else if(type == 1){
        int T = Rea.ReadU();
        for(int id = 0; id < T; ++id)
            Ms[id].input();
        Errorif(Ms[0].N != init(Ms[0].K));
        for(int id = 0; id < T; ++id)
            Errorif(Ms[id].check() == 0);
        printf("%d\n", Ms[0].N);
    }
    else if(type == 2){
        int length = Rea.ReadU();
        for(int i = 0; i < length; ++i)
            putchar(Rea.ReadU());
    }
    return 0;
}