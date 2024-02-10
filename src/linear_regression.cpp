#include <linear_regression.h>

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

tuple<Eigen::MatrixXd, Eigen::VectorXd> build_matrix(vector<Stock*>data, int idx){

    // open_t-1,high_t-1,low_t-1,close_t-1,no_of_trades_t-1,vwap_t-1,open_t
    // close_t
    // Matrix X,Y
    int n=data.size()-1;
    int f=7;
    Eigen::MatrixXd X(n,f);
    Eigen::VectorXd Y(n);
    for(int i=idx;i<data.size();i++){
        Y(i-idx)=data[i]->close;

        Eigen::RowVectorXd row(f);
        row << data[i-1]->high, data[i-1]->low, data[i-1]->close, data[i-1]->open, data[i-1]->vwap, data[i-1]->num_trades, data[i]->open;
        X.row(i-idx) = row;
    }

    tuple<Eigen::MatrixXd, Eigen::VectorXd> out = tuple<Eigen::MatrixXd, Eigen::VectorXd>(X,Y);
    return out;
}

Eigen::MatrixXd add_one(Eigen::MatrixXd X){
    Eigen::MatrixXd X_new(X.rows(), X.cols() + 1);
    X_new << Eigen::MatrixXd::Ones(X.rows(), 1), X;
    return X_new;
}

Eigen::MatrixXd linear_regression(Eigen::MatrixXd X, Eigen::VectorXd Y){
    // X: Feature matrix (n x m)
    // Y: Target vector (n x 1)
    // n x m+1
    Eigen::MatrixXd X_with_intercept = add_one(X);

    // X_T(m+1 x n)
    Eigen::MatrixXd X_transpose = X_with_intercept.transpose();

    // Square X (m+1 x m+1)
    Eigen::MatrixXd X2 = X_transpose * X_with_intercept;

    // X_T * Y (m+1 x 1)
    Eigen::MatrixXd XtY = X_transpose * Y;

    // Inverse of X2
    Eigen::MatrixXd X2_inverse = X2.inverse();

    // Coefficients (m x 1)
    Eigen::MatrixXd theta = X2_inverse * XtY;

    return theta;
}

vector<double> predict_prices(vector<Stock*> data, Eigen::MatrixXd theta, int idx){
    vector<double>predicted_prices;

    // date,open,high,low,close,no_of_trades,vwap
    tuple<Eigen::MatrixXd, Eigen::VectorXd> tuple = build_matrix(data, idx);
    Eigen::MatrixXd X = get<0>(tuple);
    Eigen::VectorXd Y = get<1>(tuple);

    cout<<X.row(0)<<endl;
    cout<<Y.row(0)<<endl;

    // Add intercept to X
    Eigen::MatrixXd X_with_intercept = add_one(X);

    for(int i=0;i<X_with_intercept.rows();i++){
        Eigen::MatrixXd value = X_with_intercept.row(i) * theta;
        predicted_prices.push_back(value(0,0));
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
  tuple<Eigen::MatrixXd, Eigen::VectorXd> tuple = build_matrix(this->train_data, 1);
  Eigen::MatrixXd X = get<0>(tuple);
  Eigen::VectorXd Y = get<1>(tuple);
  // Obtain parameters for linear regression
  Eigen::MatrixXd theta = linear_regression(X, Y);

  // first day from which trading should begin
  int idx = 0;
  for(int i=0;i<this->train_data.size();i++){
    if(compare_dates(this->trade_data[i]->date,this->start_date)){
      idx = i;
      break;
    }
  }
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
  write_order_statistics(this->orders);
  write_to_txt(this->bal);
}