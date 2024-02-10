#include "dma.h"

DMA::DMA(){

}

DMA::~DMA(){

}

DMA::DMA(string _code,int _n,int _x,int _p,string _start_date,string _end_date){
  this->stock_code = _code;
  this->n = _n;
  this->x = _x;
  this->p = _p;
  this->start_date = _start_date; 
  this->end_date = _end_date;

  this->data = read_csv("data/"+this->stock_code+".csv");
}

void DMA::run(){ // actual strategy code
  int idx = -1;
  for(int i=0;i<this->data.size();i++){ // first day from which trading should begin
    if(compare_dates(this->data[i]->date,this->start_date)){
      idx = i;
      break;
    }
  }
  if(idx==-1) return; // returning if no trading day found

  assert(idx+1>=this->n);
  double sq_sum = 0.0;
  double sum = 0.0;
  int tn=this->n; int k=idx-1;
  while(tn--){ // initial setup of variables
    assert(k>=0);
    sq_sum += this->data[k]->close*this->data[k]->close;
    sum += this->data[k]->close;
    k--;
  }

  for(int i=idx;i<this->data.size();i++){ // doing the trading
    sq_sum -= this->data[i-n]->close*this->data[i-n]->close;
    sum -= this->data[i-n]->close;

    sq_sum += this->data[i]->close*this->data[i]->close;
    sum += this->data[i]->close;

    double std_dev = sqrt((n*sq_sum)-(sum*sum))/(1.0*this->n);
    double dma_mean = sum/(1.0*this->n);

    if(this->data[i]->close >= dma_mean+(this->p*std_dev) && this->curr_x < this->x){
      curr_x++;
      this->orders.push_back(new Order(this->data[i]->date,0,1,this->data[i]->close));
      this->bal -= this->data[i]->close;
    }
    else if(dma_mean >= this->data[i]->close+(this->p*std_dev) && this->curr_x > -this->x ){
      curr_x--;
      this->orders.push_back(new Order(this->data[i]->date,1,1,this->data[i]->close));
      this->bal += this->data[i]->close;
    }
    this->cashflow.push_back({this->data[i]->date,this->bal});
  }
  
  // squaring off the positions
  this->bal += this->curr_x * this->data.back()->close;
}

void DMA::run_strategy(){ // calls run and just save the data
  run();
  write_daily_cashflow(this->cashflow);
  write_order_statistics(this->orders);
  write_to_txt(this->bal);
}