#ifndef MCTS_H
#define MCTS_H
#include <iostream>
#include <vector>
#include <memory>
#include <random>
#include "./reversi.h"

// monte carlo tree search
namespace reversi{

  struct node{
    using node_ptr = std::shared_ptr<node>;

    int win_;
    int n_;
    game g_;
    std::vector<node_ptr> children_;
    const double c = 1.4142;

    node():win_(0),n_(0),g_(){}

    node(const node &n){
      win_ = n.win_;
      n_ = n.n_;
      g_ = n.g_;
      children_ = n.children_;
    }

    node(const game &g){
      win_ = 0;
      n_ = 0;
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
      return children_.empty();
    }

    void expand();
    node_ptr select(std::mt19937 &mt);

    double winning_rate() const{
      std::cout << win_ << " " << n_ << std::endl;
      1.0*win_/n_;
    }

    double score(int total) const {
      if(n_==0) return 1.0e10 ;
      return double(win_)/n_ + c*std::sqrt( std::log((double)total) /n_ );
    }

  };

  class mcts{
    std::mt19937 mt_;
    int n_th_;

    int turn_;
    node::node_ptr root_;
    const int max_playout_;
    std::string name_;
    game playout(game g);
    public:
    std::pair<node::node_ptr,double> compute_best_hand_and_score();
    mcts():max_playout_(0){}
    mcts(int turn,int mp,const std::string &name = "monte carlo tree search" ) : turn_(turn),max_playout_(mp),name_(name) {
      mt_ = std::mt19937(std::random_device()());
      n_th_ = 0;
    }
    void operator()(game &);
    std::string get_name()const { return name_; } 
  };
}

#endif
