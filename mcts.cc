#include "./mcts.h"
#include <stack>

using namespace reversi;

void node::expand(){
  if(children_.size() > 0) return;

  if(g_.check_pass()){
    int pass_num = g_.get_pass();
    g_.pass();
    node *child = new node(g_);
    children_.push_back(child);
    g_.undo(0,0,pass_num);
    return ;
  }

  bitboard validmoves = g_.get_valid_moves();
  for(int i=0;i<board_size;++i){
    bitboard m = idx2bitboard(i);
    if(!(m&validmoves)) continue;
    bitboard rev = g_.set(m);
    node *child = new node(g_);
    g_.undo(m,rev);
  
    children_.push_back(child);
  }
}

node* node::select(){
  double best_score = -1.0;
  node* best_node = nullptr;
  for(node *child : children_){
    double score = child->score(n_);
    if(best_score < score){
      best_score = score ;
      best_node = child;
    }
  }
  return best_node;
}

std::pair<node*,double> mcts::compute_best_hand_and_score(){
  node* best_hand = nullptr;
  double best_rate = -1;
  for(node *child : root_.children_){
    double rate = child->winning_rate();
    if( rate > best_rate ){
      best_rate = rate;
      best_hand = child;
    }
  }
  return std::make_pair(best_hand,best_rate);
}

void mcts::operator()(game &g){
  root_ = node(g);
  for( int i = 0 ; i < max_playout_ ; ++i ){
    node *n = &root_;
  
    std::stack<node*> s;
    // play out
    while(true){
      s.push(n);
      if(n->is_leaf()) break;
      n->expand();
      n = n->select();
    }
  
    int win = (n->g_.get_my_stone_num() >= n->g_.get_your_stone_num());
    if(n->g_.turn() == turn_) win = 1 - win;

    while(!s.empty()){
      node *played_n = s.top() ; s.pop();
      played_n->n_ += 1;
      played_n->win_ += win;
      win = 1 - win ;
    }
  }
  
  auto hs = compute_best_hand_and_score();
  g = hs.first->g_;
}
