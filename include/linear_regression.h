#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <tuple>
#include <Eigen/Dense>

using namespace std;

tuple<Eigen::MatrixXd, Eigen::VectorXd> read_csv(const string& filename);