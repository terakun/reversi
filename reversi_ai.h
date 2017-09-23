#ifndef REVERSI_AI_H
#define REVERSI_AI_H
#include <utility>
#include <array>
#include "./reversi.h"

namespace reversi{
  class reversi_ai{

    using move_score = std::pair<bitboard,double>;
    int turn_;
    std::mt19937 mt_;
    const int max_depth_;
    std::array<int,board_w*board_h> evaltable_;

    double eval(const game &);
    public: 
    
    enum option{
      ALPHA_BETA,RANDOM
    }opt_;

    reversi_ai(int turn,option o,const std::string &evalfile,int max_depth);

    void operator()(game &);
    move_score alpha_beta(game &,double alpha,double beta,int depth);
    void random(game &);
  };
};

#endif
