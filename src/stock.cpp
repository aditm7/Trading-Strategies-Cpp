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

ostream& operator<<(std::ostream& os,Stock*& stock) {
  os << "{ Date: " << stock->date << ", ";
  os << "High: " << stock->high << ", ";
  os << "Low: " << stock->low << ", ";
  os << "Close: " << stock->close << ", ";
  os << "Open: " << stock->open << ", ";
  os << "VWAP: " << stock->vwap << ", ";
  os << "Num_Trades: " << stock->num_trades << " }";
  return os;
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

ostream& operator<<(ostream& os,Order*& order){
  os << "{ Date: " << order->date << ", ";
  os << "Type: " << order->order_type << ", ";
  os << "Qty: " << order->num << ", ";
  os << "Price: " << order->price << " }";
  return os;
}

//-------Helper functions ------

bool compare_dates(string &s1,string &s2){
  int day1, month1, year1, day2, month2, year2;  
  sscanf(s1.c_str(), "%d/%d/%d", &day1, &month1, &year1);
  sscanf(s2.c_str(), "%d/%d/%d", &day2, &month2, &year2);
  
  if(year1<year2) return false;
  else if(year1>year2) return true;
  else{
    if(month1<month2) return false;
    else if(month1>month2) return true;
    else{
      if(day1<day2) return false;
      else return true;
    }
  }
}