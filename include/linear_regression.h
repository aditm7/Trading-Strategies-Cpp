#pragma once
#include "csv.h"
#include "stock.h"
#include "debug.h"
#include "matrix.h"
#include <string>
#include <cmath>

using namespace std;

struct LinearRegression{
  string stock_code;
  int x;
  int p;
  string start_date;
  string end_date;
  string train_start_date;
  string train_end_date;

  int curr_x=0;
  double bal=0;
  vector<double> predicted_wts = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
  vector<Stock*> train_data;
  vector<Stock*> trade_data;
  vector<Order*> orders;
  vector<pair<string,double>> cashflow;

  LinearRegression();
  ~LinearRegression();

  LinearRegression(string _code,int _x,int _p,string _start_date,string _end_date,string _train_start_date,string _train_end_date);
  
  void train_model();
  double predict_price(vector<double> &row);
  void run();
  void run_strategy();
};

tuple<vector<vector<double>>, vector<double>> build_matrix(vector<Stock*> &data, int idx);
vector<vector<double>> add_one(vector<vector<double>> &X);
vector<vector<double>> linear_regression(vector<vector<double>> &X, vector<double> &Y);
vector<double> predict_prices(vector<Stock*> &data, vector<vector<double>> &theta, int idx);