#include <linear_regression.h>

// TODO:
// MSE loss
// calculate R-squared (R2) score
// MAE loss
// Train test split?
// Normalising the feature matrix

tuple<Eigen::MatrixXd, Eigen::VectorXd> build_matrix(vector<Stock*>data){
    int n=data.size()-1;
    int f=7;

    // open_t-1,high_t-1,low_t-1,close_t-1,no_of_trades_t-1,vwap_t-1,open_t
    // close_t
    // Matrix X,Y
    Eigen::MatrixXd X(n,f);
    Eigen::VectorXd Y(n);
    for(int i=1;i<data.size();i++){
        Y(i-1)=data[i]->close;

        Eigen::RowVectorXd row(f);
        row << data[i-1]->open, data[i-1]->high, data[i-1]->low, data[i-1]->close, data[i-1]->vwap, data[i-1]->num_trades, data[i]->open;
        X.row(i-1) = row;
    }

    tuple<Eigen::MatrixXd, Eigen::VectorXd> out = tuple<Eigen::MatrixXd, Eigen::VectorXd>(X,Y);
    return out;
}

Eigen::MatrixXd add_one(Eigen::MatrixXd X){
    Eigen::MatrixXd X_new(X.rows(), X.cols() + 1);
    X_new << Eigen::MatrixXd::Ones(X.rows(), 1), X;
    return X_new;
}

Eigen::MatrixXd linear_regression(Eigen::MatrixXd X, Eigen::VectorXd Y){
    // X: Feature matrix (n x m)
    // Y: Target vector (n x 1)
    // n x m+1
    Eigen::MatrixXd X_with_intercept = add_one(X);

    // X_T(m+1 x n)
    Eigen::MatrixXd X_transpose = X_with_intercept.transpose();

    // Square X (m+1 x m+1)
    Eigen::MatrixXd X2 = X_transpose * X_with_intercept;

    // X_T * Y (m+1 x 1)
    Eigen::MatrixXd XtY = X_transpose * Y;

    // Inverse of X2
    Eigen::MatrixXd X2_inverse = X2.inverse();

    // Coefficients (m x 1)
    Eigen::MatrixXd theta = X2_inverse * XtY;

    return theta;
}

vector<double> predict_prices(vector<Stock*> data, Eigen::MatrixXd theta){
    vector<double>predicted_prices;

    // date,open,high,low,close,no_of_trades,vwap
    tuple<Eigen::MatrixXd, Eigen::VectorXd> tuple = build_matrix(data);
    Eigen::MatrixXd X = get<0>(tuple);
    Eigen::VectorXd Y = get<1>(tuple);

    cout<<X<<endl;
    cout<<Y<<endl;

    // Add intercept to X
    Eigen::MatrixXd X_with_intercept = add_one(X);

    for(int i=0;i<X_with_intercept.rows();i++){
        Eigen::MatrixXd value = X_with_intercept.row(i) * theta;
        predicted_prices.push_back(value(0,0));
    }
    return predicted_prices;
}