#ifndef MCTS
#define MCTS
#include <iostream>
#include <vector>
#include "./reversi.h"

// monte carlo tree search
namespace reversi{
  struct node{
    int win_;
    int n_;
    game g_;
    std::vector<node*> children_;
    const double c = 1.4;

    node():win_(0),n_(0),g_(){}

    node(const node &n){
      win_ = n.win_;
      n_ = n.n_;
      g_ = n.g_;
      children_ = n.children_;
    }

    node(const game &g){
      win_ = 0;
      n_ = 1;
      g_ = g;
    }

    node& operator=(const node &n){
      win_ = n.win_;
      n_ = n.n_;
      g_ = n.g_;
      children_ = n.children_;
      return (*this);
    }

    bool is_leaf()const{
      return g_.isfinish();
    }

    void expand();
    node* select();

    double winning_rate() const{
      std::cout << win_ << " " << n_ << std::endl;
      1.0*win_/n_;
    }

    double score(int total) const {
      return 1.0*win_/n_ + c*std::sqrt( std::log(total) /n_ );
    }

  };

  class mcts{
    int turn_;
    node root_;
    const int max_playout_;
    std::string name_;
    public:
    std::pair<node*,double> compute_best_hand_and_score();
    mcts(int turn,int mp,const std::string &name = "monte carlo tree search" ) : turn_(turn),max_playout_(mp),name_(name) {}
    void operator()(game &);
    std::string get_name()const { return name_; } 
  };
}

#endif
