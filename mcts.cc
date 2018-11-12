#include "./mcts.h"
#include <stack>

using namespace reversi;

void node::expand(){
  if(children_.size() > 0) return;

  if(g_.check_pass()){
    int pass_num = g_.get_pass();
    g_.pass();
    children_.emplace_back(new node(g_));
    g_.undo(0,0,pass_num);
    return ;
  }

  bitboard validmoves = g_.get_valid_moves();
  for(int i=0;i<board_size;++i){
    bitboard m = idx2bitboard(i);
    if(!(m&validmoves)) continue;
    bitboard rev = g_.set(m);
    children_.emplace_back(new node(g_));
    g_.undo(m,rev);
  }
}

node::node_ptr node::select(std::mt19937 &mt){
  double best_score = -1.0;
  node::node_ptr best_node = nullptr;
  std::vector<node_ptr> untried_nodes;
  for(node::node_ptr child : children_){
    double score = child->score(n_);
    if(score == 1.0e10){
      untried_nodes.push_back(child);
    }
    if(best_score < score){
      best_score = score ;
      best_node = child;
    }
  }
  if(!untried_nodes.empty()){
    std::uniform_int_distribution<> dist(0,untried_nodes.size()-1);
    return untried_nodes[dist(mt)];
  }
  return best_node;
}

std::pair<node::node_ptr,double> mcts::compute_best_hand_and_score(){
  node::node_ptr best_hand = nullptr;
  int best_trial = -1;
  for(node::node_ptr child : root_->children_){
    int trial = child->n_;
    if( trial > best_trial ){
      best_trial = trial;
      best_hand = child;
    }
  }
  return std::make_pair(best_hand,best_hand->winning_rate());
}

game mcts::playout(game g){
  while(!g.isfinish()){
    if(g.check_pass()){
      g.pass();
      continue;
    }

    std::vector<bitboard> validmoves;
    g.get_valid_moves(validmoves);
    std::uniform_int_distribution<> dist(0,validmoves.size()-1);
    bitboard m = validmoves[dist(mt_)];
    g.set(m);
  }
  return g;
}

void mcts::operator()(game &g){
  root_ = node::node_ptr(new node(g));
  root_->expand();
  for( int i = 0 ; i < max_playout_ ; ++i ){
    node::node_ptr n = root_;
  
    std::stack<node::node_ptr> s;

    while(true){
      s.push(n);
      if(n->is_leaf()) break;
      n = n->select(mt_);
    }
    
    if( n->n_ > n_th_ ){
      n->expand();
      n = n->select(mt_);
      s.push(n);
    }
    
    game end = playout(n->g_);
    int win = (end.get_my_stone_num() >= end.get_your_stone_num());
    if(win != 0 && win != 1) std::abort();
    if(end.turn() != turn_) win = 1 - win;

    while(!s.empty()){
      node::node_ptr played_n = s.top() ; s.pop(); 
      played_n->n_ += 1;
      played_n->win_ += win;
    }
  }
  
  auto hs = compute_best_hand_and_score();
  g = hs.first->g_;
}
