#include <linear_regression.h>

// TODO:
// MSE loss
// calculate R-squared (R2) score
// MAE loss
// Train test split?
// Normalising the feature matrix

tuple<Eigen::MatrixXd, Eigen::VectorXd> build_matrix(vector<Stock*>data, string start_date){
    // TODO: iterate the dates and reach the working date just before start date
    int itr=1;

    // open_t-1,high_t-1,low_t-1,close_t-1,no_of_trades_t-1,vwap_t-1,open_t
    // close_t
    // Matrix X,Y
    int n=data.size()-1;
    int f=7;
    Eigen::MatrixXd X(n,f);
    Eigen::VectorXd Y(n);
    for(int i=itr;i<data.size();i++){
        Y(i-itr)=data[i]->close;

        Eigen::RowVectorXd row(f);
        row << data[i-1]->open, data[i-1]->high, data[i-1]->low, data[i-1]->close, data[i-1]->vwap, data[i-1]->num_trades, data[i]->open;
        X.row(i-itr) = row;
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

vector<double> predict_prices(vector<Stock*> data, Eigen::MatrixXd theta, string start_date){
    vector<double>predicted_prices;

    // date,open,high,low,close,no_of_trades,vwap
    tuple<Eigen::MatrixXd, Eigen::VectorXd> tuple = build_matrix(data, start_date);
    Eigen::MatrixXd X = get<0>(tuple);
    Eigen::VectorXd Y = get<1>(tuple);

    cout<<X<<endl;
    cout<<Y<<endl;

    // Add intercept to X
    Eigen::MatrixXd X_with_intercept = add_one(X);

    for(int i=0;i<X_with_intercept.rows();i++){
        Eigen::MatrixXd value = X_with_intercept.row(i) * theta;
        predicted_prices.push_back(value(0,0));
    }
    return predicted_prices;
}

LinearRegression::LinearRegression(){ // default constructor

}

LinearRegression::~LinearRegression(){ // default destructor

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
  // Make X,Y matrix from the csv data
  tuple<Eigen::MatrixXd, Eigen::VectorXd> tuple = build_matrix(this->train_data, this->start_date);
  Eigen::MatrixXd X = get<0>(tuple);
  Eigen::VectorXd Y = get<1>(tuple);
  // Obtain parameters for linear regression
  Eigen::MatrixXd theta = linear_regression(X, Y);

  // Predict price for each day
  // starting from data[1] --> end
  vector<double>predicted_prices = predict_prices(trade_data, theta, this->start_date);
}

void LinearRegression::run_strategy(){ // calls run and just save the data
  run();
  write_daily_cashflow(this->cashflow);
  write_order_statistics(this->orders);
  write_to_txt(this->final_pnl);
}