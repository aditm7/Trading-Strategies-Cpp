#include "linear_regression.h"

LinearRegression::LinearRegression(){ // default constructor

}

LinearRegression::~LinearRegression(){ // default destructor

}

LinearRegression::LinearRegression(string _code,int _x,int _p,string _start_date,string _end_date,string _train_start_date,string _train_end_date){
  this->stock_code = _code;
  this->x = _x;
  this->p = _p;
  this->start_date = _start_date; 
  this->end_date = _end_date;
  this->train_start_date = _train_start_date;
  this->train_end_date = _train_end_date;
  this->train_data = read_csv("data/train.csv");
  this->trade_data = read_csv("data/"+this->stock_code+".csv");
}

void LinearRegression::train_model(){
  int idx = -1;
  for(int i=0;i<this->train_data.size();i++){
    if(compare_dates(this->train_data[i]->date,this->train_start_date)){
      idx = i;
      break;
    }
  }
  if(idx==-1) return; // returning if no training data day found
  assert(idx-1>=0);

  vector<vector<double>> X(this->train_data.size()-idx);
  vector<vector<double>> Y(this->train_data.size()-idx);
  for(int i=idx;i<this->train_data.size();i++){
    X[i-idx] = {1,this->train_data[i-1]->close,this->train_data[i-1]->open, this->train_data[i-1]->vwap, 
            this->train_data[i-1]->low, this->train_data[i-1]->high, 
            double(this->train_data[i-1]->num_trades), this->train_data[i]->open};
    Y[i-idx] = {this->train_data[i]->close};
  }

  vector<vector<double>> Xt = transpose(X);
  vector<vector<double>> XtX = multiply(Xt,X);
  vector<vector<double>> Inv_XtX = inverse(XtX);
  vector<vector<double>> XtY = multiply(Xt,Y);
  vector<vector<double>> theta = multiply(Inv_XtX,XtY);

  vector<double> wts;
  for(int i=0;i<8;i++) wts.push_back(theta[i][0]);
  this->predicted_wts = wts;
}

double LinearRegression::predict_price(vector<double> &row){
  double ans = 0.0;
  for(int i=0;i<8;i++){
    ans += row[i]*this->predicted_wts[i];
  }
  return ans;
}

void LinearRegression::run(){ // actual strategy code
  train_model();
  // Placing orders
  int idx = -1;
  for(int i=0;i<this->trade_data.size();i++){
    if(compare_dates(this->trade_data[i]->date,this->start_date)){
      idx = i;
      break;
    }
  }
  if(idx==-1) return; // returning if no training data day found
  assert(idx-1>=0);
  for(int i=idx;i<this->trade_data.size();i++){ // doing the trading
    double actual_price = trade_data[i]->close;
    vector<double> input_row = {1,this->trade_data[i-1]->close,this->trade_data[i-1]->open, this->trade_data[i-1]->vwap, 
            this->trade_data[i-1]->low, this->trade_data[i-1]->high, 
            double(this->trade_data[i-1]->num_trades), this->trade_data[i]->open};
    
    double predicted_price = predict_price(input_row);
    double percentageDifference = ((predicted_price - actual_price) / actual_price) * 100;

    // If predicted price is more than actual price by threshold percentage, // BUY the stock
    if (percentageDifference >= this->p && this->curr_x < this->x) {
      this->curr_x++;
      this->orders.push_back(new Order(this->trade_data[i]->date,0,1,this->trade_data[i]->close));
      this->bal -= this->trade_data[i]->close;
    }
    // If predicted price is less than actual price by threshold percentage, // SELL the stock
    else if (percentageDifference <= -(this->p) && this->curr_x > -this->x) {
      this->curr_x--;
      this->orders.push_back(new Order(this->trade_data[i]->date,1,1,this->trade_data[i]->close));
      this->bal += this->trade_data[i]->close;
    }
    this->cashflow.push_back({this->trade_data[i]->date,this->bal});
  }

  // squaring off the positions
  this->bal += this->curr_x * this->trade_data.back()->close;
}

void LinearRegression::run_strategy(){ // calls run and just save the data
  run();
  write_daily_cashflow(this->cashflow);
  write_order_statistics(this->orders, "order_statistics.csv");
  write_to_txt(this->bal);
}