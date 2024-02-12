#include "adx.h"

ADX::ADX(){ // default constructor

}

ADX::~ADX(){ // default destructor

}

ADX::ADX(string _code,int _n,int _x,double _adx_threshold,string _start_date,string _end_date){
  this->stock_code = _code;
  this->n = _n;
  this->x = _x;
  this->adx_threshold = _adx_threshold;
  this->start_date = _start_date; 
  this->end_date = _end_date;

  this->data = read_csv("data/"+this->stock_code+".csv");
}

void ADX::run(){ // actual strategy code
  int idx = -1;
  for(int i=0;i<this->data.size();i++){ // first day from which trading should begin
    if(compare_dates(this->data[i]->date,this->start_date)){
      idx = i;
      break;
    }
  }
  if(idx==-1) return; // returning if no trading day found
  assert(idx>=2);

  double last_tr = max(this->data[idx]->high - this->data[idx]->low,
                      max(this->data[idx]->high-this->data[idx-1]->close,
                      this->data[idx]->low-this->data[idx-1]->close));
  double last_dm1 = max(this->data[idx]->high-this->data[idx-1]->high,0.0); // dm+
  double last_dm0 = max(this->data[idx]->low-this->data[idx-1]->low,0.0); // dm-
  double last_atr = last_tr;
  double last_di1 = (last_dm1)/(last_atr); // di+
  double last_di0 = (last_dm0)/(last_atr); // di-
  double last_dx = (abs(last_di1+last_di0-0.0)!=0.0 ? ((last_di1-last_di0)/(last_di1+last_di0))*100.0 : 0.0);
  double last_adx = last_dx;
  
  for(int i=idx;i<this->data.size();i++){ // doing the trading
    if(i!=idx){
      double new_tr = max(this->data[i]->high - this->data[i]->low,
                      max(this->data[i]->high-this->data[i-1]->close,
                      this->data[i]->low-this->data[i-1]->close));
      double new_dm1 = max(this->data[i]->high-this->data[i-1]->high,0.0);
      double new_dm0 = max(this->data[i]->low-this->data[i-1]->low,0.0);
      double new_atr = last_atr + (2.0/(1.0*this->n+1.0))*(new_tr - last_atr);
      double new_di1 = last_di1 + (2.0/(1.0*this->n+1.0))*((new_dm1/new_atr) - last_di1);
      double new_di0 = last_di0 + (2.0/(1.0*this->n+1.0))*((new_dm0/new_atr) - last_di0);
      double new_dx = (abs(new_di1+new_di0-0.0)!=0.0 ? ((new_di1-new_di0)/(new_di1+new_di0))*100.0 : 0.0);
      double new_adx = last_adx + (2.0/(1.0*this->n+1.0))*(new_dx - last_adx);

      // if(abs(new_di1+new_di0)!=0.0) debug(new_di1+new_di0);

      last_tr = new_tr; last_dm1 = new_dm1; last_dm0 = new_dm0;
      last_atr = new_atr; last_di1 = new_di1; last_di0 = new_di0;
      last_dx = new_dx; last_adx = new_adx;
    }

    if(last_adx > this->adx_threshold && (last_di1+last_di0-0.0)!=0.0 && this->curr_x < this->x){ // BUY the stock
      curr_x++;
      this->orders.push_back(new Order(this->data[i]->date,0,1,this->data[i]->close));
      this->bal -= this->data[i]->close;
    }
    else if(last_adx < this->adx_threshold && (last_di1+last_di0-0.0)!=0.0 && this->curr_x > -this->x){ // SELL the stock
      curr_x--;
      this->orders.push_back(new Order(this->data[i]->date,1,1,this->data[i]->close));
      this->bal += this->data[i]->close;
    }
    this->cashflow.push_back({this->data[i]->date,this->bal});
  }

  // squaring off the positions
  this->bal += this->curr_x * this->data.back()->close;
}

void ADX::run_strategy(){ // calls run and just save the data
  run();
  write_daily_cashflow(this->cashflow);
  write_order_statistics(this->orders, "order_statistics.csv");
  write_to_txt(this->bal);
}