#include<main.h>

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
    // code for BASIC
    cout<<argc<<endl;
  }
  else if(strategy=="DMA"){

  }

  return 0;
}