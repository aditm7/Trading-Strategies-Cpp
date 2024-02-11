#include "linear_regression.h"

// TODO:
// MSE loss
// calculate R-squared (R2) score
// MAE loss
// Train test split?
// Normalising the feature matrix

LinearRegression::LinearRegression(){ // default constructor

}

LinearRegression::~LinearRegression(){ // default destructor

}

tuple<vector<vector<double>>, vector<double>> build_matrix(vector<Stock*>data, int idx){

    // open_t-1,high_t-1,low_t-1,close_t-1,no_of_trades_t-1,vwap_t-1,open_t
    // close_t
    // Matrix X,Y
    int n=data.size()-idx;
    int f=7;
    vector<vector<double>>X(n, vector<double>(f,0));
    vector<double>Y(n,0);
    for(int i=idx;i<data.size();i++){
        Y[i-idx]=data[i]->close;

        vector<double>row = {data[i-1]->high, data[i-1]->low, data[i-1]->close, data[i-1]->open, data[i-1]->vwap, double(data[i-1]->num_trades), data[i]->open};
        X[i-idx] = row;
    }

    tuple<vector<vector<double>>, vector<double>> out = tuple<vector<vector<double>>, vector<double>>(X,Y);
    return out;
}

vector<vector<double>> add_one(vector<vector<double>> X){
    vector<vector<double>> X_new(X.size(), vector<double>(X[0].size() + 1));
    for(int i=0;i<X.size();i++){
      vector<double>row = X[i];
      row.push_back(1.0);
      X_new[i] = row;
    }
    return X_new;
}

vector<vector<double>> linear_regression(vector<vector<double>> X, vector<double>Y){
    // X: Feature matrix (n x m)
    // Y: Target vector (n x 1)
    // n x m+1
    vector<vector<double>> X_with_intercept = add_one(X);

    // X_T(m+1 x n)
    vector<vector<double>> X_transpose = transpose(X_with_intercept);

    // Square X (m+1 x m+1)
    vector<vector<double>> X2 = multiply(X_transpose, X_with_intercept);

    // X_T * Y (m+1 x 1)
    // Convert Y into nx1
    vector<vector<double>>Y_new(Y.size(),vector<double>(1));
    for(int i=0;i<Y.size();i++){
      Y_new[i]={Y[i]};
    }
    vector<vector<double>> XtY = multiply(X_transpose, Y_new);

    // Inverse of X2
    vector<vector<double>> X2_inverse = inverse(X2);

    // Coefficients (m x 1)
    vector<vector<double>> theta =multiply(X2_inverse, XtY);

    return theta;
}

vector<double> predict_prices(vector<Stock*> data, vector<vector<double>> theta, int idx){
    vector<double>predicted_prices;

    // date,open,high,low,close,no_of_trades,vwap
    tuple<vector<vector<double>>, vector<double>> tuple = build_matrix(data, idx);
    vector<vector<double>> X = get<0>(tuple);
    vector<double> Y = get<1>(tuple);

    // Add intercept to X
    vector<vector<double>> X_with_intercept = add_one(X);

    for(int i=0;i<X_with_intercept.size();i++){
        vector<vector<double>> value = multiply({X_with_intercept[i]}, theta);
        predicted_prices.push_back(value[0][0]);
    }
    return predicted_prices;
}

LinearRegression::LinearRegression(string _code,int _x,int _p,string _start_date,string _end_date){
  this->stock_code = _code;
  this->x = _x;
  this->p = _p;
  this->start_date = _start_date; 
  this->end_date = _end_date;

  this->train_data = read_csv("data/train.csv");
  this->trade_data = read_csv("data/"+this->stock_code+".csv");
}

void LinearRegression::run(){ // actual strategy code
  // Make X,Y matrix from the csv train data
  tuple<vector<vector<double>>, vector<double>> tuple = build_matrix(this->train_data, 1);
  vector<vector<double>> X = get<0>(tuple);
  vector<double> Y = get<1>(tuple);
  // Obtain parameters for linear regression
  vector<vector<double>> theta = linear_regression(X, Y);

  // first day from which trading should begin
  int idx = -1;
  for(int i=0;i<this->train_data.size();i++){
    if(compare_dates(this->trade_data[i]->date,this->start_date)){
      idx = i;
      break;
    }
  }
  if(idx==-1) return; // returning if no trading day found
  
  // Predict price for each day
  assert(idx-1>=0);
  vector<double>predicted_prices = predict_prices(this->trade_data, theta, idx);

  // Placing orders
  for(int i=idx;i<this->trade_data.size();i++){ // doing the trading
    double actual_price = trade_data[i]->close;
    double predicted_price = predicted_prices[i-idx];

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
  write_order_statistics(this->orders, "order_statisitcs.csv");
  write_to_txt(this->bal);
}