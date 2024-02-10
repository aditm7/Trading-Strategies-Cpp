#pragma once

#include <stock.h>
#include <Eigen/Dense>
#include <string>
#include <iostream>

using namespace std;

#include "csv.h"
#include "stock.h"
#include "debug.h"

struct LinearRegression{
  string stock_code;
  int x;
  int p;
  string start_date;
  string end_date;

  int curr_x=0;
  double bal=0;
  vector<Stock*> train_data;
  vector<Stock*> trade_data;
  vector<Order*> orders;
  vector<pair<string,double>> cashflow;

  LinearRegression();
  ~LinearRegression();

  LinearRegression(string _code,int _x,int _p,string _start_date,string _end_date);

  void run();
  void run_strategy();
};

tuple<Eigen::MatrixXd, Eigen::VectorXd> build_matrix(vector<Stock*>data, int idx);
Eigen::MatrixXd add_one(Eigen::MatrixXd X);
Eigen::MatrixXd linear_regression(Eigen::MatrixXd X, Eigen::VectorXd Y);
vector<double> predict_prices(vector<Stock*> data, Eigen::MatrixXd theta, int idx);