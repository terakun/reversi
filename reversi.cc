#include <iostream>
#include <vector>
#include <stack>
#include "reversi.h"

namespace reversi{
  void showbitboard(const bitboard &b){
    for(int i=0;i<board_h;++i){
      for(int j=0;j<board_w;++j){
        std::cout<<((b&pos2bitboard(i,j))?'1':'0');
      }
      std::cout<<std::endl;
    }
  }

  void game::show()const{
    constexpr char chars[2] = {'*','o'};
    std::cout<<"  ";
    for(int j=0;j<board_w;++j){
      std::cout<<j;
    }
    std::cout<<std::endl;
    for(int i=0;i<board_h;++i){
      std::cout<<i<<":";
      for(int j=0;j<board_w;++j){
        bitboard m = pos2bitboard(i,j);
        if(m&my_s_){
          std::cout<<chars[t_];
        }else if(m&your_s_){
          std::cout<<chars[1-t_];
        }else if(m&validmoves_){
          std::cout<<'.';
        }else{
          std::cout<<' ';
        }
      }
      std::cout<<std::endl;
    }
    std::cout<<"my stone("<<chars[t_]<<"):"<<get_my_stone_num()<<std::endl;
    std::cout<<"your stone("<<chars[1-t_]<<"):"<<get_your_stone_num()<<std::endl;
  }

  void human::operator()(game &g)const{
    if(g.check_pass()){
      g.pass();
      std::cout<<"pass."<<std::endl;
      return ;
    }
    int y,x;
    bitboard validmoves = g.get_valid_moves();
    while(true){
      std::cout<<"Your Turn."<<std::endl;
      std::cout<<"Input (y x):";
      std::cin>>y>>x;
      if(validmoves&pos2bitboard(y,x)) break;
    }
    bitboard rev = g.set(y,x);
  }

};
