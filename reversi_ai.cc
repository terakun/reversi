#include <fstream>
#include "./reversi_ai.h"

namespace reversi{

  reversi_ai::reversi_ai(int turn,option o,const std::string &evalfile,int max_depth):turn_(turn),mt_(std::random_device()()),opt_(o),max_depth_(max_depth){
    std::ifstream ifs(evalfile);
    for(int i=0;i<board_size;++i){
      ifs >> evaltable_[i];
    }
  }

  void reversi_ai::operator()(game &g){
    switch(opt_) {
      case ALPHA_BETA:
        {    
          if(g.check_pass()){
            g.pass();
            break;
          }

          move_score ms = alpha_beta(g,-100000,100000,max_depth_);
          if(ms.first==0){
            std::cout << "something wrong!" << std::endl;
            std::cout << ms.second << std::endl;
            std::abort();
          }
          g.set(ms.first);
          break; 
        }
      case RANDOM:
        random(g);
        break;
    }
  }
  
  double reversi_ai::eval(const game &g){
    double score = 0;
    bitboard my_s = g.get_my_stone();
    for(int i=0;i<board_h;++i){
      for(int j=0;j<board_w;++j){
        score += check_bit(i,j,my_s) * evaltable_[i*board_w+j];
      }
    }
    return score;
  }

  reversi_ai::move_score reversi_ai::alpha_beta(game &g,double alpha,double beta,int depth){
    move_score best_ms(0,-1000000);
    std::vector<bitboard> v;
    g.get_valid_moves(v);

    if( depth == 0 ){
      double score = eval(g);
      return move_score(0,score);
    }

    if(g.isfinish()){
      if(g.get_my_stone_num() > g.get_your_stone_num()){
        return move_score(0,10000000);
      }else{
        return move_score(0,-10000000);
      }
    }

    if(g.check_pass()){
      int pass_num = g.get_pass();
      g.pass();
      auto ms = alpha_beta(g,-beta,-alpha,depth-1);
      g.undo(0,0,pass_num);
      return ms;
    }

    for(auto &m:v){
      bitboard rev = g.set(m);
      move_score ms = alpha_beta(g,-beta,-alpha,depth-1);
      g.undo(m,rev);
      if(best_ms.first==0||-ms.second>best_ms.second){
        alpha = -ms.second;
        best_ms = move_score(m,alpha);
      }
      if(alpha >= beta) return best_ms;
    }
    return best_ms;
  }

  void reversi_ai::random(game &g){
    std::vector<bitboard> validmoves;
    g.get_valid_moves(validmoves);
    if(g.check_pass()){
      g.pass();
      return;
    }
    std::uniform_int_distribution<> dist(0,validmoves.size()-1);
    g.set(validmoves[dist(mt_)]);
  }
};
