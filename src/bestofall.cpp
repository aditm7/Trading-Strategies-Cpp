#include "bestofall.h"

BestOfAll::BestOfAll(){ // default constructor

}

BestOfAll::~BestOfAll(){ // default destructor

}

BestOfAll::BestOfAll(string _code,string _start_date,string _end_date){
  this->stock_code = _code;
  this->start_date = _start_date;
  this->end_date = _end_date;

  this->basic_strategy = new Basic(_code,7,5,_start_date,_end_date);
  this->dma_strategy = new DMA(_code,50,5,2,_start_date,_end_date);
  this->dmapp_strategy = new DMApp(_code,14,5,5,28,2,0.2,_start_date,_end_date);
  this->macd_strategy = new MACD(_code,5,_start_date,_end_date);
  this->rsi_strategy = new RSI(_code,14,5,30,70,_start_date,_end_date);
  this->adx_strategy = new ADX(_code,14,5,25,_start_date,_end_date);
  this->linear_regression_strategy = new LinearRegression(_code,5,2,_start_date,_end_date);
}

void BestOfAll::run(){
  vector<thread> vt;
  vt.push_back(thread(&Basic::run, this->basic_strategy));
  vt.push_back(thread(&DMA::run, this->dma_strategy));
  vt.push_back(thread(&DMApp::run, this->dmapp_strategy));
  vt.push_back(thread(&MACD::run, this->macd_strategy));
  vt.push_back(thread(&RSI::run, this->rsi_strategy));
  vt.push_back(thread(&ADX::run, this->adx_strategy));
  vt.push_back(thread(&LinearRegression::run, this->linear_regression_strategy));
  
  for (thread& t : vt) t.join();
  // all threads execution completed at this point
  vector<double> ans = {this->basic_strategy->bal,
                        this->dma_strategy->bal,
                        this->dmapp_strategy->bal,
                        this->macd_strategy->bal,
                        this->rsi_strategy->bal,
                        this->adx_strategy->bal,
                        this->linear_regression_strategy->bal };
  
  int idx = max_element(ans.begin(),ans.end()) - ans.begin();
  if(idx==0){
    this->bal = this->basic_strategy->bal;
    this->orders = this->basic_strategy->orders;
    this->cashflow = this->basic_strategy->cashflow;
  }
  else if(idx==1){
    this->bal = this->dma_strategy->bal;
    this->orders = this->dma_strategy->orders;
    this->cashflow = this->dma_strategy->cashflow;
  }
  else if(idx==2){
    this->bal = this->dmapp_strategy->bal;
    this->orders = this->dmapp_strategy->orders;
    this->cashflow = this->dmapp_strategy->cashflow;
  }
  else if(idx==3){
    this->bal = this->macd_strategy->bal;
    this->orders = this->macd_strategy->orders;
    this->cashflow = this->macd_strategy->cashflow;
  }
  else if(idx==4){
    this->bal = this->rsi_strategy->bal;
    this->orders = this->rsi_strategy->orders;
    this->cashflow = this->rsi_strategy->cashflow;
  }
  else if(idx==5){
    this->bal = this->adx_strategy->bal;
    this->orders = this->adx_strategy->orders;
    this->cashflow = this->adx_strategy->cashflow;
  }
  else if(idx==6){
    this->bal = this->linear_regression_strategy->bal;
    this->orders = this->linear_regression_strategy->orders;
    this->cashflow = this->linear_regression_strategy->cashflow;
  }
}

void BestOfAll::run_strategy(){
  run();
  write_daily_cashflow(this->cashflow);
  write_order_statistics(this->orders, "order_statistics.csv");
  write_to_txt(this->bal);
}