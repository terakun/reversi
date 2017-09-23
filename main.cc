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
  int turn = (c!='w');

  reversi_ai ai_random(turn,reversi_ai::RANDOM,"./eval.dat",9);
  reversi_ai ai(1-turn,reversi_ai::ALPHA_BETA,"./eval.dat",9);
  human h;
  play<reversi_ai,reversi_ai> p(ai,ai_random,turn);
  // play<reversi_ai,human> p(ai,h,turn);
  
  p.run();

  return 0;
}
