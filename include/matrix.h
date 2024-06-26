#pragma once

#include <vector>
#include <stdexcept>
#include <cmath>
using namespace std;

// Calculate the transpose of a matrix
vector<vector<double>> transpose(const vector<vector<double>>& matrix);

// Matrix multiplication
vector<vector<double>> multiply(const vector<vector<double>>& matrix1, const vector<vector<double>>& matrix2);

double determinant(const vector<vector<double>>& matrix);

vector<vector<double>> inverse(const vector<vector<double>>& matrix);
