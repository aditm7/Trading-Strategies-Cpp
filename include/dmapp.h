#include "csv.h"
#include "stock.h"
#include "debug.h"
#include <cassert>
#include <deque>
using namespace std;

struct DMApp{
  string stock_code;
  int n,x,p,max_hold_days;
  double c1,c2;
  string start_date;
  string end_date;

  int curr_x=0;
  double bal=0;
  vector<Stock*> data;
  vector<Order*> orders;
  vector<pair<string,double>> cashflow;

  DMApp();
  ~DMApp();

  DMApp(string _code,int _n,int _x,int _p,int _max_hold_days,double _c1,double _c2,string _start_date,string _end_date);

  void run();
  void run_strategy();
};