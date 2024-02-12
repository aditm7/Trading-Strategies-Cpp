#include "main.h"

int main(int argc,char* argv[]){
  #ifdef DEBUG
  freopen("logs/debug.log","w",stderr);
  #endif
  if(argc<=1){
    cout<<"Insufficient arguments, exiting ...."<<endl;
    return 1;
  }

  string strategy = argv[1];
  if(strategy=="BASIC"){
    Basic* basic_strategy = new Basic(argv[2],stoi(argv[3]),stoi(argv[4]),argv[5],argv[6]);
    basic_strategy->run_strategy();
  }
  else if(strategy=="DMA"){
    DMA* dma_strategy = new DMA(argv[2],stoi(argv[3]),stoi(argv[4]),stoi(argv[5]),argv[6],argv[7]);
    dma_strategy->run_strategy();
  }
  else if(strategy=="DMA++"){
    DMApp* dmapp_strategy = new DMApp(argv[2],stoi(argv[3]),stoi(argv[4]),stoi(argv[5]),stoi(argv[6]),stod(argv[7]),stod(argv[8]),argv[9],argv[10]);
    dmapp_strategy->run_strategy();
  }
  else if(strategy=="MACD"){
    MACD* macd_strategy = new MACD(argv[2],stoi(argv[3]),argv[4],argv[5]);
    macd_strategy->run_strategy();
  }
  else if(strategy=="RSI"){
    RSI* rsi_strategy = new RSI(argv[2],stoi(argv[3]),stoi(argv[4]),stod(argv[5]),stod(argv[6]),argv[7],argv[8]);
    rsi_strategy->run_strategy();
  }
  else if(strategy=="ADX"){
    ADX* adx_strategy = new ADX(argv[2],stoi(argv[3]),stoi(argv[4]),stod(argv[5]),argv[6],argv[7]);
    adx_strategy->run_strategy();
  }
  else if(strategy=="LINEAR_REGRESSION"){
    LinearRegression* linreg_strategy = new LinearRegression(argv[2],stoi(argv[3]),stoi(argv[4]),argv[5],argv[6],argv[7],argv[8]);
    linreg_strategy->run_strategy();
  }
  else if(strategy=="BEST_OF_ALL"){
    BestOfAll*  best_strategy = new BestOfAll(argv[2],argv[3],argv[4]);
    best_strategy->run_strategy();
  }
  else if(strategy=="PAIRS"){
    if(argc==9){
      Pairs* pairs_strategy = new Pairs(argv[2],argv[3],stoi(argv[4]),stoi(argv[5]),stod(argv[6]),argv[7],argv[8]);
      pairs_strategy->run_strategy();
    }
    else{
      Pairspp* pairspp_strategy = new Pairspp(argv[2],argv[3],stoi(argv[4]),stoi(argv[5]),stod(argv[6]),stod(argv[7]),argv[8],argv[9]);
      pairspp_strategy->run_strategy();
    }
  }

  return 0;
}