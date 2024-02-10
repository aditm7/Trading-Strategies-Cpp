#include "dmapp.h"

DMApp::DMApp(){

}

DMApp::~DMApp(){

}

DMApp::DMApp(string _code,int _n,int _x,int _p,int _max_hold_days,double _c1,double _c2,string _start_date,string _end_date){
  this->stock_code = _code;
  this->n = _n;
  this->x = _x;
  this->p = _p;
  this->max_hold_days = _max_hold_days;
  this->c1 = _c1;
  this->c2 = _c2;
  this->start_date = _start_date; 
  this->end_date = _end_date;

  this->data = read_csv("data/"+this->stock_code+".csv");
}

void DMApp::run(){ // actual strategy code
  int idx = -1;
  for(int i=0;i<this->data.size();i++){ // first day from which trading should begin
    if(compare_dates(this->data[i]->date,this->start_date)){
      idx = i;
      break;
    }
  }
  if(idx==-1) return; // returning if no trading day found

  assert(idx+1>=this->n);
  double sum_abs = 0.0;
  double last_sf = 0.5;
  double last_ama = this->data[idx]->close;
  deque<int> dq;
  int tn=n; int k=idx-1;
  while(tn--){ // initial setup of variables
    assert(k>=0);
    sum_abs += abs(this->data[k]->close-this->data[k-1]->close);
    k--;
  }

  for(int i=idx;i<this->data.size();i++){ // doing the trading
    sum_abs -= abs(this->data[i-n]->close-this->data[i-n-1]->close);
    sum_abs += abs(this->data[i]->close-this->data[i-1]->close);
    double delta_price = abs(this->data[i]->close-this->data[i-n]->close);

    int normal_trade = 0;
    if(sum_abs!=0){ // checking if the trade can be made normally
      double er = delta_price/sum_abs;
      if(i!=idx){
        double temp = (2.0*er - (1+this->c2))/(2.0*er + (1+this->c2));
        double new_sf = last_sf + ((this->c1)*(temp-last_sf));
        double new_ama = last_ama + (new_sf*(this->data[i]->close-last_ama));
        last_ama = new_ama;
        last_sf = new_sf;
      }
      if(this->data[i]->close >= last_ama + (last_ama*this->p/100.0) && this->curr_x < this->x) normal_trade++;
      else if(last_ama >= this->data[i]->close + (last_ama*this->p/100.0) && this->curr_x > -this->x) normal_trade--;
    }

    int max_hold_trade = 0;
    if(dq.size()>=1 && i-dq.front()+1>=this->max_hold_days && this->curr_x>0) max_hold_trade = -1; // clear the position by SELL
    if(dq.size()>=1 && i-dq.front()+1>=this->max_hold_days && this->curr_x<0) max_hold_trade = 1; // clear the position by BUY
    
    if((normal_trade==0 && max_hold_trade==-1) || (normal_trade==-1 && max_hold_trade==0) || (normal_trade==-1 && max_hold_trade==-1)){ // SELL the stock
      curr_x--;
      this->orders.push_back(new Order(this->data[i]->date,1,1,this->data[i]->close));
      this->bal += this->data[i]->close;
      if(curr_x>=0) dq.pop_front();
      else if(curr_x<0) dq.push_back(i);
    }
    else if((normal_trade==0 && max_hold_trade==1) || (normal_trade==1 && max_hold_trade==0) || (normal_trade==1 && max_hold_trade==1)){ // BUY the stock
      curr_x++;
      this->orders.push_back(new Order(this->data[i]->date,0,1,this->data[i]->close));
      this->bal -= this->data[i]->close;
      if(curr_x<=0) dq.pop_front();
      else if(curr_x>0) dq.push_back(i);
    }
    else if((normal_trade==1 && max_hold_trade==-1) || (normal_trade==-1 && max_hold_trade==1)){ // special case of opposite actions
      dq.pop_front();
      dq.push_back(i);
    }
    this->cashflow.push_back({this->data[i]->date,this->bal});
  }
  // squaring off the positions
  this->bal += this->curr_x * this->data.back()->close;
}

void DMApp::run_strategy(){ // calls run and just save the data
  run();
  write_daily_cashflow(this->cashflow);
  write_order_statistics(this->orders);
  write_to_txt(this->bal);
}