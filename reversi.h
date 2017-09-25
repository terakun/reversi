#ifndef REVERSI_H
#define REVERSI_H
#include <random>
#include <iostream>

namespace reversi {
  constexpr int board_w = 8 , board_h = 8;
  constexpr int board_size = board_w*board_h;

  using bitboard = uint64_t;

  constexpr bitboard transfermask[8] = {
    0xfefefefefefefefe,
    0xfefefefefefefefe,
    0xffffffffffffffff,
    0x7f7f7f7f7f7f7f7f,
    0x7f7f7f7f7f7f7f7f,
    0x7f7f7f7f7f7f7f7f,
    0xffffffffffffffff,
    0xfefefefefefefefe
  };

  constexpr int transferdir[8] = {
    1,-(board_w-1),-board_w,-(board_w+1),-1,board_w-1,board_w,board_w+1
  };

  inline bitboard pos2bitboard(int i,int j){ return bitboard(1)<<(i*board_w+j); }
  inline bitboard idx2bitboard(int idx){ return bitboard(1)<<idx; }
  inline bool check_bit(int i,int j,bitboard b){ return bitboard(1)<<(i*board_w+j) & b; }

  void showbitboard(const bitboard &b);

  inline bitboard shift(bitboard m,int cnt){
    if(cnt<0)
      return m>>-cnt;
    else
      return m<<cnt;
  }

  inline bitboard transfer(bitboard m,int i){
    return shift(m,transferdir[i])&transfermask[i];
  }

  inline int get_num(const bitboard &b){
    return __builtin_popcountl(b);
  }

  inline bitboard set_stone(const bitboard &my_stone,const bitboard &your_stone,const bitboard &m){
    bitboard rev = 0;
    if(((my_stone|your_stone)&m)!=0) return rev;

    for(int i=0;i<8;++i){
      bitboard sub_rev = 0;
      bitboard mask = transfer(m,i);
      while(mask!=0&&(mask&your_stone)!=0){
        sub_rev |= mask;
        mask = transfer(mask,i);
      }
      if((mask&my_stone) != 0)
        rev |= sub_rev ;
    }
    return rev;

  }

  // game state class
  class game{
    bitboard my_s_,your_s_;
    bitboard validmoves_;
    int t_;
    int pass_;
    public:
    game():t_(0),pass_(0){
      my_s_   = 0x0000000810000000;
      your_s_ = 0x0000001008000000;
      calc_valid_moves();
    }

    game(const game &g){
      t_ = g.t_;
      my_s_ = g.my_s_;
      your_s_ = g.your_s_;
      validmoves_ = g.validmoves_;
      pass_ = g.pass_;
    }

    game(const bitboard &ms,const bitboard &ys){
      my_s_ = ms ;
      your_s_ = ys;
      t_=0;pass_=0;
    }

    game& operator=(const game &g){
      my_s_ = g.my_s_;
      your_s_ = g.your_s_;
      validmoves_ = g.validmoves_;
      t_ = g.t_;
      pass_ = g.pass_;
      return (*this);
    }

    bool operator==(const game &g)const{
      return my_s_==g.my_s_&&your_s_==g.your_s_&&t_==g.t_&&pass_==g.pass_;
    }

    void show()const;

    bitboard get_my_stone()const{ return my_s_; }
    bitboard get_your_stone()const{ return your_s_; }

    int get_pass()const{ return pass_; }
    int get_my_stone_num()const{ return get_num(my_s_); }
    int get_your_stone_num()const{ return get_num(your_s_); }
    int get_stone_num()const{ return get_num(my_s_|your_s_); }
    bool win()const{ return get_my_stone_num() > get_your_stone_num() ; }

    bool isfinish()const{
      return (pass_==2)||(!((my_s_|your_s_)+1));
    }

    int turn()const{ return t_; }

    bool check_pass()const{
      return !validmoves_;
    }

    void pass(){
      t_ = (t_+1)%2;
      std::swap(my_s_,your_s_);
      calc_valid_moves();
      pass_++;
    }

    bitboard set(const bitboard &mov){
      bitboard rev = set_stone(my_s_,your_s_,mov);

      my_s_ ^= mov|rev;
      your_s_ ^= rev;
      t_ = (t_+1)%2;

      std::swap(my_s_,your_s_);
      pass_ = 0;
      calc_valid_moves();
      return rev;
    }

    bitboard set(int y,int x){
      return set(pos2bitboard(y,x));
    } 

    bool undo(const bitboard &mov,const bitboard &rev,int pass_num=0){
      std::swap(my_s_,your_s_);
      t_ = (t_+1)%2;
      your_s_ ^= rev;
      my_s_ ^= mov|rev;
      calc_valid_moves();
      if(mov==0) pass_ = pass_num;
      return true;
    }

    void calc_valid_moves(){
      validmoves_ = 0;
      for(int i=0;i<board_size;++i){
        bitboard m = idx2bitboard(i);
        if(set_stone(my_s_,your_s_,m))
          validmoves_ |= m;
      }
    }

    bitboard get_valid_moves()const{ return validmoves_; }
    int get_valid_moves(std::vector<bitboard> &v)const{
      v.resize(get_num(validmoves_));
      int idx=0;
      for(int i=0;i<board_h;++i){
        for(int j=0;j<board_w;++j){
          bitboard m = pos2bitboard(i,j);
          if(m&validmoves_)
            v[idx++]=m;
        }
      }
      return v.size();
    }

  };

  class human{
    std::string name_;
    public:
    human(const human &h){}
    human(std::string name="human"):name_(name){}
    void operator()(game &g)const;
    std::string get_name()const{ return name_; } 
  };

  template<typename P1,typename P2> 
  class play{
    std::pair<P1,P2> player;
    game g;
    int t;
    public:
    play(const P1 &p1,const P2 &p2,int t):g(),player(p1,p2),t(t){}

    void run(){
      while(!g.isfinish()){
        g.show();
        if(g.turn()==t){
          player.first(g);
        }else{
          player.second(g);
        }
      }
      g.show();

      if(g.get_my_stone_num()>g.get_your_stone_num()){
        if(g.turn()==t){
          std::cout << player.first.get_name() << " win" << std::endl;
        }else{
          std::cout << player.second.get_name() << " win" << std::endl;
        }
      }else if(g.get_my_stone_num()<g.get_your_stone_num()){
        if(g.turn()==t){
          std::cout << player.second.get_name() << " win" << std::endl;
        }else{
          std::cout << player.first.get_name() << " win" << std::endl;
        }
      }else{
        std::cout << "draw" << std::endl;
      }
    }
  };

} 

#endif
