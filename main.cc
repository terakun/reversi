#include <iostream>
#include "./reversi.h"
#include "./reversi_ai.h"

int main(){
  using namespace reversi;
  char c;
  for(;;){
    std::cout<<"select black(b) or white(w):";
    std::cin>>c;
    if(c=='b'||c=='w') break;
  }
  int turn = (c=='w');

  // reversi_ai ai1(turn,reversi_ai::ALPHA_BETA,"./eval.dat",7,-1,"alphabeta 4");
  reversi_ai ai2(1-turn,reversi_ai::ALPHA_BETA,"./eval.dat",10,20,"alphabeta 8 exhaustive");
  human h;
  // play<reversi_ai,reversi_ai> p(ai1,ai2,turn);
  play<human,reversi_ai> p(h,ai2,turn);
  
  p.run();

  return 0;
}
