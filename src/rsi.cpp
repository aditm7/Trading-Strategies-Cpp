#include "rsi.h"

RSI::RSI(){ // default constructor

}

RSI::~RSI(){ // default destructor

}

RSI::RSI(string _code,int _n,int _x,double _oversold_threshold,double _overbought_threshold,string _start_date,string _end_date){
  this->stock_code = _code;
  this->n = _n;
  this->x = _x;
  this->oversold_threshold = _oversold_threshold;
  this->overbought_threshold = _overbought_threshold;
  this->start_date = _start_date; 
  this->end_date = _end_date;

  this->data = read_csv("data/"+this->stock_code+".csv");
}

void RSI::run(){ // actual strategy code
  int idx = -1;
  for(int i=0;i<this->data.size();i++){ // first day from which trading should begin
    if(compare_dates(this->data[i]->date,this->start_date)){
      idx = i;
      break;
    }
  }
  if(idx==-1) return; // returning if no trading day found
  assert(idx+1>=this->n);

  double sum_gain = 0.0;
  double sum_loss = 0.0;
  int tn = this->n;
  int k=idx-1;
  while(tn--){
    assert(k>=1);
    sum_gain += max(this->data[k]->close - this->data[k-1]->close,0.0);
    sum_loss += max(this->data[k-1]->close - this->data[k]->close,0.0);
  }
  for(int i=idx;i<this->data.size();i++){ // doing the trading
    sum_gain -= max(this->data[i-n]->close - this->data[i-n-1]->close,0.0);
    sum_gain += max(this->data[i]->close - this->data[i-1]->close,0.0);
    sum_loss -= max(this->data[i-n-1]->close - this->data[i-n]->close,0.0);
    sum_loss += max(this->data[i-1]->close - this->data[i]->close,0.0);
    
    double rs = sum_gain/sum_loss;
    double rsi = 100.0 - (100.0/(1.0+rs));
    if(rsi < this->oversold_threshold && this->curr_x < this->x){ // BUY the stock
      curr_x++;
      this->orders.push_back(new Order(this->data[i]->date,0,1,this->data[i]->close));
      this->bal -= this->data[i]->close;
    }
    else if(rsi > overbought_threshold && this->curr_x > -this->x){ // SELL the stock
      curr_x--;
      this->orders.push_back(new Order(this->data[i]->date,1,1,this->data[i]->close));
      this->bal += this->data[i]->close;
    }
    this->cashflow.push_back({this->data[i]->date,this->bal});
  }

  // squaring off the positions
  this->bal += this->curr_x * this->data.back()->close;
}

void RSI::run_strategy(){ // calls run and just save the data
  run();
  write_daily_cashflow(this->cashflow);
  write_order_statistics(this->orders);
  write_to_txt(this->bal);
}