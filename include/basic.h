#include "csv.h"
#include "stock.h"
#include "debug.h"

using namespace std;

struct Basic{
  string stock_code;
  int n;
  int x;
  string start_date;
  string end_date;

  int curr_x=0;
  double final_pnl=0;
  vector<Stock*> data;
  vector<Order*> orders;
  vector<pair<string,double>> cashflow;

  Basic();
  ~Basic();

  Basic(string _code,int _n,int _x,string _start_date,string _end_date);

  void run();
  void run_strategy();
};