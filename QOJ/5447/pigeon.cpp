#include <string>

// you may define some global variables, but it does not work if you try to
// transfer any information from function pigeon_num or function send to
// function receive through these variables. you had better not use the same
// global variables in function send and in function receive.

/*
Taskid:         The index of the subtask. If it is subtask 2, then Taskid=2.
k:              The time threshold. Suppose the i-th pigeon taking off is the
p_i-th one to land. It is guaranteed that abs(i-p_i)<=k. return value:   The
number of pigeons Little E will use.
*/
int pigeon_num(int Taskid, int k) {
  // you may do some initialization for SEND function here

  return 5;  // change this into your code
}

/*
Taskid:         The index of the subtask. If it is subtask 2, then Taskid=2.
n:              The number of pigeons Little E will use. This equals the return
value of function pigeon_num. k:              The time threshold. Suppose the
i-th pigeon taking off is the p_i-th one to land. It is guaranteed that
abs(i-p_i)<=k. msg:            The content of the message. return value:
The order of the pigeons taking off. The length of this string must be n.
return_value[i]='0' means the (i+1)-th pigeon taking off is black.
return_value[i]='1' means the (i+1)-th pigeon taking off is white.
*/
std::string send(int Taskid, int n, int k, __uint128_t msg) {
  return "10101";  // change this into your code
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Init {
 public:
  Init() {
    // you may do some initialization for RECEIVE function here
  }
};

/*
Taskid:         The index of the subtask. If it is subtask 2, then Taskid=2.
k:              The time threshold. Suppose the i-th pigeon taking off is the
p_i-th one to land. It is guaranteed that abs(i-p_i)<=k. msg:
The order of the pigeons landing. Its length is equal to n. msg[i] denotes the
color of the (i+1)-th pigeon watched landing by Little F. '0' means black and
'1' means white. return value:	The content of the message. It should be
correct.
*/
__uint128_t receive(int Taskid, int k, const std::string &msg) {
  const static Init init;
  {  // change this into your code
    __uint128_t hi = 5281683694948011861llu;
    __uint128_t lo = 3195384480471073102llu;
    return hi << 64 |
           lo;  // this value equals 97429867398990605044182047185430790478
  }
}