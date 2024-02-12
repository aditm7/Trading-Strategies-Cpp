#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cassert>
#include "stock.h"
#include "debug.h"
#include <iomanip>
using namespace std;

vector<Stock*> read_csv(string filename);
void write_order_statistics(const vector<Order*> &data, const string& filename);
void write_daily_cashflow(const vector<pair<string,double>> &data);
void write_to_txt(double num);