#pragma once
#include "csv.h"
#include "stock.h"
#include "debug.h"
#include <cassert>
#include <cmath>

using namespace std;

struct DMA{
  string stock_code;
  int n;
  int x;
  int p;
  string start_date;
  string end_date;

  int curr_x=0;
  double bal=0;
  vector<Stock*> data;
  vector<Order*> orders;
  vector<pair<string,double>> cashflow;

  DMA();
  ~DMA();

  DMA(string _code,int _n,int _x,int _p,string _start_date,string _end_date);

  void run();
  void run_strategy();
};