#pragma once
#include<iostream>
#include<string>

using namespace std;
#define endl "\n"

#ifdef DEBUG
#include "debug.hpp"
#else
#define debug(...) 42
#define error(...) 42
#define console(...) 42
#endif