#ifndef REVERSI_AI_H
#define REVERSI_AI_H
#include <utility>
#include <array>
#include <map>
#include "./reversi.h"
#include "./mcts.h"

namespace reversi{
  class reversi_ai{
    using move_score = std::pair<bitboard,int>;
    using move_result = std::pair<bitboard,bool>;
    std::mt19937 mt_;
    const int max_depth_;
    int exhaustive_threshold_;
    int t_;
    std::array<int,board_w*board_h> evaltable_;
    std::map<std::pair<bitboard,bitboard>,int> transposition_table_;

    bool exist_solution;

    int eval(const game &);
    std::string name_;

    mcts m_;
    public: 

    enum option{
      ALPHA_BETA,NEGASCOUT,EXHAUSTIVE,MCTS,RANDOM
    }opt_,pre_opt_;

    reversi_ai(int t,option o,const std::string &evalfile,int max_depth,int exhaustive_threshold,std::string name="AI");
    reversi_ai(int t,option o,int max_trial,int exhaustive_threshold,std::string name="AI");

    void operator()(game &);
    move_score alpha_beta(game &,int alpha,int beta,int depth);
    move_score negascout(game &,int alpha,int beta,int depth);
    
    move_result exhaustive(game &);

    void random(game &);
    std::string get_name()const{ return name_; } 
  };
};

#endif
