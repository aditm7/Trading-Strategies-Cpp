#pragma once
#include "csv.h"
#include "stock.h"
#include "debug.h"
#include "basic.h"
#include "dma.h"
#include "dmapp.h"
#include "macd.h"
#include "rsi.h"
#include "adx.h"
#include "linear_regression.h"
#include <cassert>
#include <thread>
#include <vector>
#include <string>
#include <sstream>
#include <cstdio>
using namespace std;

struct BestOfAll{
  string stock_code;
  string start_date;
  string end_date;
  
  Basic* basic_strategy;
  DMA* dma_strategy;
  DMApp* dmapp_strategy;
  MACD* macd_strategy;
  RSI* rsi_strategy;
  ADX* adx_strategy;
  LinearRegression* linear_regression_strategy; 

  double bal;
  vector<Order*> orders;
  vector<pair<string,double>> cashflow;
  
  BestOfAll();
  ~BestOfAll();

  BestOfAll(string _code,string _start_date,string _end_date);
  void run();
  void run_strategy();
};
