#pragma once
#include "csv.h"
#include "stock.h"
#include "debug.h"
#include <cassert>
#include <cmath>
using namespace std;

struct MACD{
  string stock_code;
  int x;
  string start_date;
  string end_date;

  int curr_x=0;
  double bal=0;
  vector<Stock*> data;
  vector<Order*> orders;
  vector<pair<string,double>> cashflow;

  MACD();
  ~MACD();

  MACD(string _code,int _x,string _start_date,string _end_date);

  void run();
  void run_strategy();
};