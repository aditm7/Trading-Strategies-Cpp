#pragma once
#include "csv.h"
#include "stock.h"
#include "debug.h"
#include <cassert>
#include <cmath>
using namespace std;

struct RSI{
  string stock_code;
  int n;
  int x;
  double oversold_threshold;
  double overbought_threshold;
  string start_date;
  string end_date;

  int curr_x=0;
  double bal=0;
  vector<Stock*> data;
  vector<Order*> orders;
  vector<pair<string,double>> cashflow;

  RSI();
  ~RSI();

  RSI(string _code,int _n,int _x,double _oversold_threshold,double _overbought_threshold,string _start_date,string _end_date);

  void run();
  void run_strategy();
};