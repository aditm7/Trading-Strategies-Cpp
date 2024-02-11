#include "pairs.h"

Pairs::Pairs(){ // default constructor

}

Pairs::~Pairs(){ // default destructor

}

Pairs::Pairs(string _code_1, string _code_2,int _n,int _x, int _threshold, string _start_date,string _end_date){
  this->stock_code_1 = _code_1;
  this->stock_code_2 = _code_2;
  this->n = _n;
  this->x = _x;
  this->threshold = _threshold;
  this->start_date = _start_date; 
  this->end_date = _end_date;

  this->data_1 = read_csv("data/"+this->stock_code_1+".csv");
  this->data_2 = read_csv("data/"+this->stock_code_2+".csv");
}

void Pairs::run(){ // actual strategy code
  assert(data_1.size()==data_2.size());

  int idx = -1;
  for(int i=0;i<this->data_1.size();i++){ // first day from which trading should begin
    if(compare_dates(this->data_1[i]->date,this->start_date)){
      idx = i;
      break;
    }
  }
  if(idx==-1) return; // returning if no trading day found
  
  assert(idx+1>=this->n);

  // Finding initial sum and sum of squares
  double sum = 0.0;
  for (int i = idx-1; i > idx-this->n; i--) {
    sum += (data_1[i]->close -  data_2[i]->close);
  }
  double sumSquares = 0.0;
  for (int i = idx-1; i > idx-this->n; i--) {
    sumSquares += (data_1[i]->close -  data_2[i]->close) * (data_1[i]->close -  data_2[i]->close);
  }

  // Calculating rolling mean and standard deviation
  for (int i = idx; i < data_1.size(); i++) {
    // Rolling mean
    sum += (data_1[i]->close -  data_2[i]->close);
    double rolling_mean = sum/this->n;
    sum -= (data_1[i-this->n+1]->close -  data_2[i-this->n+1]->close);

    // Rollign std deviation
    sumSquares += (data_1[i]->close -  data_2[i]->close) * (data_1[i]->close -  data_2[i]->close);
    double std_dev = sqrt((sumSquares / this->n) - (rolling_mean * rolling_mean));
    sumSquares -= (data_1[i-this->n+1]->close -  data_2[i-this->n+1]->close) * (data_1[i-this->n+1]->close -  data_2[i-this->n+1]->close);
    
    // Z-score
    double z_score = ((data_1[i]->close -  data_2[i]->close) - rolling_mean) / std_dev;

    // Placing trade
    if(z_score > this->threshold){ // SELL S1, BUY S2
        if(this->curr_x_1 > -this->x && this->curr_x_2 < this->x){
            this->curr_x_1--;
            this->curr_x_2++;

            // Sell S1
            this->orders_1.push_back(new Order(this->data_1[i]->date,1,1,this->data_1[i]->close));
            this->bal += this->data_1[i]->close;

            // Buy S2
            this->orders_2.push_back(new Order(this->data_2[i]->date,0,1,this->data_2[i]->close));
            this->bal -= this->data_2[i]->close;
        }
    }
    else if(z_score < -this->threshold){ // BUY S1, SELL S2
        if(this->curr_x_2 > -this->x && this->curr_x_1 < this->x){
            this->curr_x_2--;
            this->curr_x_1++;

            // Sell S2
            this->orders_2.push_back(new Order(this->data_2[i]->date,1,1,this->data_2[i]->close));
            this->bal += this->data_2[i]->close;

            // Buy S1
            this->orders_1.push_back(new Order(this->data_1[i]->date,0,1,this->data_1[i]->close));
            this->bal -= this->data_1[i]->close;
        }
    }
    this->cashflow.push_back({this->data_1[i]->date,this->bal});
  }

  // squaring off the positions
  this->bal += this->curr_x_1 * this->data_1.back()->close;
  this->bal += this->curr_x_2 * this->data_2.back()->close;
}

void Pairs::run_strategy(){ // calls run and just save the data
  run();
  write_daily_cashflow(this->cashflow);
  write_order_statistics(this->orders_1, "order_statisitcs_1.csv");
  write_order_statistics(this->orders_2, "order_statisitcs_2.csv");
  write_to_txt(this->bal);
}