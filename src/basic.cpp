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
  debug(this->data[0]);
}

void Basic::run_strategy(){ // calls run and just save the data
  run();
  write_daily_cashflow(this->cashflow);
  write_order_statistics(this->orders);
  write_to_txt(this->final_pnl);
}