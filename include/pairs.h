#include "csv.h"
#include "stock.h"
#include "debug.h"

using namespace std;

struct Pairs{
  string stock_code_1;
  string stock_code_2;
  int n;
  int x;
  int threshold;
  string start_date;
  string end_date;

  int curr_x_1=0;
  int curr_x_2=0;
  double bal=0;
  vector<Stock*> data_1;
  vector<Stock*> data_2;
  vector<Order*> orders;
  vector<pair<string,double>> cashflow;

  Pairs();
  ~Pairs();

  Pairs(string _code_1, string _code_2,int _n,int _x, int _threshold, string _start_date,string _end_date);

  void run();
  void run_strategy();
};