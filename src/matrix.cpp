#include <matrix.h>

using namespace std;

// Calculate the transpose of a matrix
vector<vector<double>> transpose(const vector<vector<double>>& matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();

    vector<vector<double>> result(cols, vector<double>(rows));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j][i] = matrix[i][j];
        }
    }
    return result;
}

// Matrix multiplication
vector<vector<double>> multiply(const vector<vector<double>>& matrix1, const vector<vector<double>>& matrix2) {
    int rows1 = matrix1.size();
    int cols1 = matrix1[0].size();
    int rows2 = matrix2.size();
    int cols2 = matrix2[0].size();

    if (cols1 != rows2) {
        throw invalid_argument("Invalid matrix dimensions for multiplication");
    }

    vector<vector<double>> result(rows1, vector<double>(cols2, 0.0));

    for (int i = 0; i < rows1; ++i) {
        for (int j = 0; j < cols2; ++j) {
            for (int k = 0; k < cols1; ++k) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    return result;
}

double determinant(const vector<vector<double>>& matrix) {
    int n = matrix.size();
    if (n == 1) {
        return matrix[0][0];
    }

    double det = 0;
    for (int i = 0; i < n; i++) {
        vector<vector<double>> minor(n - 1, vector<double>(n - 1));
        for (int j = 1; j < n; j++) {
            int row = 0;
            for (int k = 0; k < n; k++) {
                if (k != i) {
                    minor[j - 1][row++] = matrix[j][k];
                }
            }
        }
        det += (i%2==0?1:-1) * matrix[0][i] * determinant(minor);
    }
    return det;
}

vector<vector<double>> inverse(const vector<vector<double>>& matrix) {
    int n = matrix.size();

    // Handle non-invertible matrices
    double det = determinant(matrix);
    if (abs(det) < 1e-10) {
        throw runtime_error("Matrix is not invertible");
    }

    vector<vector<double>> inverse(n, vector<double>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            vector<vector<double>> minor(n - 1, vector<double>(n - 1));
            for (int k = 0; k < n; k++) {
                for (int l = 0; l < n; l++) {
                    if (k != i && l != j) {
                        minor[k - (k > i)][l - (l > j)] = matrix[k][l];
                    }
                }
            }
            inverse[j][i] = ((i+j)%2==0?1:-1) * determinant(minor) / det;
        }
    }
    return inverse;
}