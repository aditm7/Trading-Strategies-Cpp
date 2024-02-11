#include "csv.h"
#include "stock.h"
#include "debug.h"
#include <cassert>

using namespace std;

struct ADX{
  string stock_code;
  int n;
  int x;
  double adx_threshold;
  string start_date;
  string end_date;

  int curr_x=0;
  double bal=0;
  vector<Stock*> data;
  vector<Order*> orders;
  vector<pair<string,double>> cashflow;

  ADX();
  ~ADX();

  ADX(string _code,int _n,int _x,double _adx_threshold,string _start_date,string _end_date);

  void run();
  void run_strategy();
};