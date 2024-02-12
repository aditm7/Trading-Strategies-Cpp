#include "csv.h"

vector<Stock*> read_csv(string filename){
  vector<Stock*> data;
  ifstream file(filename);
  string line;
  bool skip=true;

  auto split = [](const string &s,char delim){ // tokenize the input string separated by a delimiter
    vector<string> tokens;
    string token;
    stringstream line(s);
    while (getline(line, token, delim)) {
      tokens.push_back(token);
    }
    return tokens;
  };

  while (getline(file, line)) {
    if(skip) { skip=false; continue; }
    vector<string> tokens = split(line, ','); 
    assert(tokens.size()==7);
    data.push_back(new Stock(
                    tokens[0],stod(tokens[1]),stod(tokens[2]),stod(tokens[3]),
                    stod(tokens[4]),stod(tokens[5]),stoi(tokens[6])
                  ));
  }

  file.close();
  return data;
}

void write_order_statistics(const vector<Order*> &data, const string& filename){
  ofstream file(filename,ios::out);
  file<<fixed<<setprecision(2);
  file<<"Date"<<','<<"Order_dir"<<','<<"Quantity"<<','<<"Price"<<"\n";
  for(auto &order:data){
    file<<order->date<<','
        <<(order->order_type?"SELL":"BUY")<<','
        <<order->num<<','
        <<order->price<<"\n";
  }
  file.close();
}

void write_daily_cashflow(const vector<pair<string,double>> &data){
  ofstream file("daily_cashflow.csv",ios::out);
  file<<fixed<<setprecision(2);
  file<<"Date"<<','<<"Cashflow"<<"\n";
  for(auto &flow:data){
    file<<flow.first<<','<<flow.second<<"\n";
  }
  file.close();
}

void write_to_txt(double num){
  ofstream file("final_pnl.txt",ios::out);
  file<<fixed<<setprecision(2);
  file<<num<<"\n";
  file.close();
}