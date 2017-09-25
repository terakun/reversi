#include <fstream>
#include "./reversi_ai.h"

namespace reversi{

  reversi_ai::reversi_ai(int t,option o,const std::string &evalfile,int max_depth,int exhaustive_threshold,std::string name):mt_(std::random_device()()),t_(t),opt_(o),max_depth_(max_depth),exhaustive_threshold_(exhaustive_threshold),name_(name){
    std::ifstream ifs(evalfile);
    for(int i=0;i<board_size;++i){
      ifs >> evaltable_[i];
    }
  }

  void reversi_ai::operator()(game &g){
    if(g.check_pass()){
      g.pass();
      return;
    }
    if( board_size - g.get_stone_num() < exhaustive_threshold_ ){
      opt_ = EXHAUSTIVE;
    }

    switch(opt_) {
      case ALPHA_BETA:
        {    
          move_score ms = alpha_beta(g,-100000,100000,max_depth_);
          if(ms.first==0){
            std::cout << "something wrong!" << std::endl;
            std::cout << ms.second << std::endl;
            std::abort();
          }
          g.set(ms.first);
          break; 
        }
      case NEGASCOUT:
        {
          move_score ms = negascout(g,-100000,100000,max_depth_);
          if(ms.first==0){
            std::cout << "something wrong!" << std::endl;
            std::cout << ms.second << std::endl;
            std::abort();
          }
          g.set(ms.first);
          break; 
        }
      case EXHAUSTIVE:
        {
          move_result mr = exhaustive(g);
          if(!mr.second){
            std::cout << "surrender" << std::endl;
            std::abort();
          }

          g.set(mr.first);
          break;
        }
      case RANDOM:
        random(g);
        break;
    }
  }
  
  int reversi_ai::eval(const game &g){
    int score = 0;
    bitboard my_s = g.get_my_stone();
    bitboard your_s = g.get_your_stone();
    for(int i=0;i<board_h;++i){
      for(int j=0;j<board_w;++j){
        score += check_bit(i,j,my_s) * evaltable_[i*board_w+j];
        score -= check_bit(i,j,your_s) * evaltable_[i*board_w+j];
      }
    }
    score += 20*get_num(g.get_valid_moves());
    return score;
  }

  reversi_ai::move_score reversi_ai::alpha_beta(game &g,int alpha,int beta,int depth){
    move_score best_ms(0,-1000000);
    bitboard validmoves = g.get_valid_moves();

    if( depth == 0 ){
      int score = eval(g);
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

    for(int i=0;i<board_size;++i){
      bitboard m = idx2bitboard(i);
      if(!(m&validmoves)) continue;
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

  reversi_ai::move_score reversi_ai::negascout(game &g,int alpha,int beta,int depth){
    if( depth == 0 ){
      int score = eval(g);
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

    bitboard validmoves = g.get_valid_moves();

    move_score best_ms(0,alpha);
    double b = beta;
    bool not_eldest = false;
    for(int i=0;i<board_size;++i){
      bitboard m = idx2bitboard(i);
      if(!(m&validmoves)) continue;

      bitboard rev = g.set(m);
      move_score ms = negascout(g,-b,-best_ms.second,depth-1);
      g.undo(m,rev);

      if(-ms.second>best_ms.second&&-ms.second<beta&&depth>1&&not_eldest){
        g.set(m);
        best_ms = negascout(g,-beta,-ms.second,depth-1);
        g.undo(m,rev);
      }

      if(-ms.second>-best_ms.second){
        best_ms = move_score(m,ms.second);
      }else{
        best_ms = move_score(m,ms.second);
      }
      
      if(-best_ms.second >= beta) return best_ms;
      b = -best_ms.second+1;
      not_eldest = true;
    }
    return best_ms;
  }

  reversi_ai::move_result reversi_ai::exhaustive(game &g){
    if(g.isfinish()){
      return move_result(0,g.win());
    }

    if(g.check_pass()){
      int pass_num = g.get_pass();
      g.pass();
      auto mr = exhaustive(g);
      g.undo(0,0,pass_num);
      return mr;
    }

    bitboard validmoves = g.get_valid_moves();
    move_score mr;
    for(int i=0;i<board_size;++i){
      bitboard m = idx2bitboard(i);
      if(!(m&validmoves)) continue;
      bitboard rev = g.set(m);
      mr = exhaustive(g);
      g.undo(m,rev);
      if(g.turn() == t_){
        if(mr.second) return move_result(m,true);
      }else{
        if(!mr.second) return move_result(m,false);
      }
    }

    return mr;
  }

  void reversi_ai::random(game &g){
    std::vector<bitboard> validmoves;
    g.get_valid_moves(validmoves);

    std::uniform_int_distribution<> dist(0,validmoves.size()-1);
    g.set(validmoves[dist(mt_)]);
  }
};
