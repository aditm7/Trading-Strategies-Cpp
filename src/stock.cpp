#include "stock.h"

//------ Stock Class functions -----

Stock::Stock(){ // Defualt constructor

}

Stock::Stock(string _date,double _high,double _low,double _close,double _open, double _vwap,int _num_trades){
  this->date = _date;
  this->high = _high;
  this->low = _low;
  this->close = _close;
  this->open = _open;
  this->vwap = _vwap;
  this->num_trades = _num_trades;
}

Stock::~Stock(){ // Default destructor

}

//------ Order Class functions -----

Order::Order(){ // Defualt constructor

}

Order::Order(string _date,int _order_type,int _num,double _price){
  this->date = _date;
  this->order_type = _order_type;
  this->num = _num;
  this->price = _price;
}

Order::~Order(){ // Default destructor

}