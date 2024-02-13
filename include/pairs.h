#pragma once
#include "csv.h"
#include "stock.h"
#include <cmath>
#include <vector>
#include "debug.h"

using namespace std;

struct Pairs{
  string stock_code_1;
  string stock_code_2;
  int n;
  int x;
  double threshold;
  string start_date;
  string end_date;

  int curr_x=0;
  double bal=0.0;
  
  vector<Stock*> data_1;
  vector<Stock*> data_2;
  vector<Order*> orders_1;
  vector<Order*> orders_2;
  vector<pair<string,double>> cashflow;

  Pairs();
  ~Pairs();

  Pairs(string _code_1, string _code_2,int _n,int _x,double _threshold, string _start_date,string _end_date);

  void run();
  void run_strategy();
};