#include "main.h"

int main(int argc,char* argv[]){
  #ifdef DEBUG
  freopen("logs/debug.log","w",stderr);
  #endif
  if(argc<=1){
    cout<<"Insufficient arguments, exiting ...."<<endl;
    return 1;
  }

  string strategy = argv[1];
  if(strategy=="BASIC"){
    Basic* basic_strategy = new Basic(argv[2],stoi(argv[3]),stoi(argv[4]),argv[5],argv[6]);
    basic_strategy->run_strategy();
  }
  else if(strategy=="DMA"){

  }
  else if(strategy=="LINEAR_REGRESSION"){
    string symbol = argv[2];
    // Read train csv file
    vector<Stock*> data = read_csv("./data/train.csv");
    // Make X,Y matrix from the csv data
    tuple<Eigen::MatrixXd, Eigen::VectorXd> tuple = build_matrix(data);
    Eigen::MatrixXd X = get<0>(tuple);
    Eigen::VectorXd Y = get<1>(tuple);
    // Obtain parameters for linear regression
    Eigen::MatrixXd theta = linear_regression(X, Y);

    // Predict price for each day
    data = read_csv("./data/" + symbol + ".csv");
    vector<double>predicted_prices = predict_prices(data, theta);
  }

  return 0;
}