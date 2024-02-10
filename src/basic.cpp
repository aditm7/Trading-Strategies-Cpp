#include "basic.h"

Basic::Basic(){ // default constructor

}

Basic::~Basic(){ // default destructor

}

Basic::Basic(string _code,int _n,int _x,string _start_date,string _end_date){
  this->stock_code = _code;
  this->n = _n;
  this->x = _x;
  this->start_date = _start_date; 
  this->end_date = _end_date;

  this->data = read_csv("data/"+this->stock_code+".csv");
}

void Basic::run(){ // actual strategy code
  int idx = -1;
  for(int i=0;i<this->data.size();i++){ // first day from which trading should begin
    if(compare_dates(this->data[i]->date,this->start_date)){
      idx = i;
      break;
    }
  }
  if(idx==-1) return; // returning if no trading day found
  
  assert(idx+1>=this->n);
  int signal = 0;
  deque<int> dq; // a deque to maintain the trends of last x days: 1 if up, -1 if down, 0 is same
  int tn=this->n;
  int k=idx-1;
  while(tn--){ // initial setup of variables
    assert(k>=1);
    if(this->data[k]->close > this->data[k-1]->close){
      dq.push_front(1);
      signal++;
    }
    else if(this->data[k]->close < this->data[k-1]->close){
      dq.push_front(-1);
      signal--;
    }
    else dq.push_front(0);
    k--;
  }

  for(int i=idx;i<this->data.size();i++){ // doing the trading
    signal-=dq.front();
    dq.pop_front();

    if(this->data[i]->close > this->data[i-1]->close){
      dq.push_back(1);
      signal++;
    }
    else if(this->data[i]->close < this->data[i-1]->close){
      dq.push_back(-1);
      signal--;
    }
    else dq.push_front(0);
    assert(dq.size()==this->n);

    if(signal==this->x && this->curr_x < this->x){ // BUY the stock
      curr_x++;
      this->orders.push_back(new Order(this->data[i]->date,0,1,this->data[i]->close));
      this->bal -= this->data[i]->close;
    }
    else if(signal==-this->x && this->curr_x > -this->x){ // SELL the stock
      curr_x--;
      this->orders.push_back(new Order(this->data[i]->date,1,1,this->data[i]->close));
      this->bal += this->data[i]->close;
    }
    this->cashflow.push_back({this->data[i]->date,this->bal});
  }

  // squaring off the positions
  this->bal += this->curr_x * this->data.back()->close;
}

void Basic::run_strategy(){ // calls run and just save the data
  run();
  write_daily_cashflow(this->cashflow);
  write_order_statistics(this->orders);
  write_to_txt(this->bal);
}