#include "macd.h"

MACD::MACD(){ // default constructor

}

MACD::~MACD(){ // default destructor

}

MACD::MACD(string _code,int _x,string _start_date,string _end_date){
  this->stock_code = _code;
  this->x = _x;
  this->start_date = _start_date; 
  this->end_date = _end_date;

  this->data = read_csv("data/"+this->stock_code+".csv");
}

void MACD::run(){ // actual strategy code
  int idx = -1;
  for(int i=0;i<this->data.size();i++){ // first day from which trading should begin
    if(compare_dates(this->data[i]->date,this->start_date)){
      idx = i;
      break;
    }
  }
  if(idx==-1) return; // returning if no trading day found
  
  double last_short_ewm = this->data[idx]->close;
  double last_long_ewm = this->data[idx]->close;
  double last_macd = last_short_ewm - last_long_ewm;
  double last_signal = last_macd;

  for(int i=idx;i<this->data.size();i++){ // doing the trading
    if(i!=idx){
      double new_short_ewm = last_short_ewm + ((this->data[i]->close - last_short_ewm)*(2.0/13.0));
      double new_long_ewm = last_long_ewm + ((this->data[i]->close - last_long_ewm)*(2.0/27.0));
      double new_macd = new_short_ewm - new_long_ewm;
      double new_signal = last_signal + ((new_macd - last_signal)*(2.0/10.0));
      last_short_ewm = new_short_ewm;
      last_long_ewm = new_long_ewm;
      last_signal = new_signal;
      last_macd = new_macd;
    }

    if(last_macd > last_signal && this->curr_x < this->x){ // BUY the stock
      curr_x++;
      this->orders.push_back(new Order(this->data[i]->date,0,1,this->data[i]->close));
      this->bal -= this->data[i]->close;
    }
    else if(last_macd < last_signal && this->curr_x > -this->x){ // SELL the stock
      curr_x--;
      this->orders.push_back(new Order(this->data[i]->date,1,1,this->data[i]->close));
      this->bal += this->data[i]->close;
    }
    this->cashflow.push_back({this->data[i]->date,this->bal});
  }

  // squaring off the positions
  this->bal += this->curr_x * this->data.back()->close;
}

void MACD::run_strategy(){ // calls run and just save the data
  run();
  write_daily_cashflow(this->cashflow);
  write_order_statistics(this->orders);
  write_to_txt(this->bal);
}