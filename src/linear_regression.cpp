#include <linear_regression.h>
#include <Eigen/Dense>

using namespace std;


// open,high,low,ltp,close,volume,value,no_of_trades,vwap
tuple<Eigen::MatrixXd, Eigen::VectorXd> read_csv(const string& filename){
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        exit(1);
    }
    string line;
    getline(file, line);
    vector<vector<double>>data;
    
    while (getline(file, line)) {
        stringstream ss(line);
        vector<double>xi;
        string substr;

        // i should not be 3,5,6
        int i=0;
        getline(ss, substr, ',');
        while(ss.good()){
            getline(ss, substr, ',');
            if(xi.size()<8 && isdigit(substr[0])){
                if(i!=3 && i!=5 && i!=6){
                    double d = stod(substr);
                    xi.push_back(d);
                }
            }
            i++;
        }
        data.push_back(xi);
    }

    int n=data.size()-1;
    int f=data[0].size()+1;


    // open_t-1,high_t-1,low_t-1,close_t-1,no_of_trades_t-1,vwap_t-1,open_t
    // close_t
    // Matrix X,Y
    Eigen::MatrixXd X(n,f);
    Eigen::VectorXd Y(n);

    for(int i=1;i<data.size();i++){
        Y(i-1)=data[i][3];
        for(int j=0;j<f-1;j++){
            X(i-1,j) = data[i-1][j];
        }
        X(i-1,f-1) = data[i][0];
    }

    cout<<"No.of samples: "<<X.rows()<<endl;
    cout<<"No.of features: "<<X.cols()<<endl;

    tuple<Eigen::MatrixXd, Eigen::VectorXd> out = tuple<Eigen::MatrixXd, Eigen::VectorXd>(X,Y);
    return out;
}