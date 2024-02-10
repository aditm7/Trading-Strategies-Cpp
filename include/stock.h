#pragma once
#include<string>
#include <iostream>
using namespace std;

struct Stock{
  string date;
  double high;
  double low;
  double close;
  double open;
  double vwap;
  int num_trades;

  Stock();
  Stock(string _date,double _high,double _low,double _close,double _open,double _vwap,int _num_trades);
  ~Stock();
  friend ostream& operator<<(std::ostream& os,Stock* &stock);
};

struct Order{
  string date;
  int order_type; // 0 if BUY, 1 if SELL
  int num;
  double price;

  Order();
  Order(string _date,int _order_type,int _num,double _price);
  ~Order();
  friend ostream& operator<<(ostream& os,Order* &order);
};

