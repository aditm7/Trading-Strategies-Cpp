#pragma once

#include <stock.h>
#include <Eigen/Dense>
#include <string>
#include <iostream>

using namespace std;

tuple<Eigen::MatrixXd, Eigen::VectorXd> build_matrix(vector<Stock*>data);

Eigen::MatrixXd add_one(Eigen::MatrixXd X);

Eigen::MatrixXd linear_regression(Eigen::MatrixXd X, Eigen::VectorXd Y);

vector<double> predict_prices(vector<Stock*> data, Eigen::MatrixXd theta);