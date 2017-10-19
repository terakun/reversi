#include <iostream>
#include "./reversi.h"
#include "./reversi_ai.h"
#include "./mcts.h"

int main(){
  using namespace reversi;
  char c;
  for(;;){
    std::cout<<"select black(b) or white(w):";
    std::cin>>c;
    if(c=='b'||c=='w') break;
  }
  int turn = (c=='w');

  reversi_ai alphabeta(turn,reversi_ai::ALPHA_BETA,"./eval.dat",9,20,"alphabeta 9 exhaustive");
  mcts m(1-turn,100);
  human h;
  // play<reversi_ai,reversi_ai> p(ai1,ai2,turn);
  // play<human,reversi_ai> p(h,alphabeta,turn);
  play<reversi_ai,mcts> p(alphabeta,m,turn);
  
  p.run();

  return 0;
}
