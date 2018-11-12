#include <iostream>
#include "./reversi.h"
#include "./reversi_ai.h"

int main(int argc,char **argv){
  using namespace reversi;
  if( argc < 2 ){
    std::cout<<argv[0] << " " << "[w or b]" << std::endl;
    return -1;
  }
  int turn = (argv[1][0]=='w');

  std::string evalfile = "./eval.dat";
  reversi_ai alphabeta(turn,reversi_ai::ALPHA_BETA,evalfile,9,22,"alphabeta 9 exhaustive 22");
  reversi_ai alphabeta2(1-turn,reversi_ai::ALPHA_BETA,evalfile,6,20,"alphabeta 6 exhaustive 20");
  play<reversi_ai,reversi_ai> p(alphabeta,alphabeta2,turn);
  p.run();

  return 0;
}
