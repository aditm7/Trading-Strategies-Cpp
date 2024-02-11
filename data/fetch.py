from jugaad_data import util as ut
from jugaad_data.nse import stock_df, stock_raw

import pandas as pd
from datetime import date,datetime,timedelta
import time
import os
import sys

# Required headers are: Date, Open Price, Close Price, High, Low, Last Trade Price (LTP), Volume, Value and Number of Trades
stock_select_headers = [ "CH_TIMESTAMP", 
                    "CH_TRADE_HIGH_PRICE", "CH_TRADE_LOW_PRICE",
                    "CH_CLOSING_PRICE", "CH_OPENING_PRICE",
                    "VWAP","CH_TOTAL_TRADES"]
stock_final_headers = [ "date",
                    "high", "low",
                    "close", "open", 
                    "vwap","no_of_trades"]
stock_dtypes = [ ut.np_date,
            ut.np_float, ut.np_float,
            ut.np_float, ut.np_float,
            ut.np_float, ut.np_int]

def fetch_raw_data(stock_code,from_date,to_date):
  raw = stock_raw(symbol=stock_code,from_date=from_date,to_date = to_date)
  final_raw = []
  for r in raw:
      new_dict = {}
      for i, h in enumerate(stock_select_headers):
          new_key = stock_final_headers[i]
          new_value = stock_dtypes[i](r[h])
          new_dict[new_key] = new_value
      final_raw.append(new_dict)
  return final_raw

def save_csv(data,filename):
  df = pd.DataFrame(data)
  df['date'] = df['date'].dt.strftime('%d/%m/%Y')
  df=df.iloc[::-1]
  df.to_csv(f'data/{filename}.csv', index=False)

strategy = str(sys.argv[1])
stock_code = str(sys.argv[2])

try:
  if strategy=="BASIC" or strategy=="DMA" or strategy=="DMA++" or strategy=="RSI":
    from_date = datetime.strptime(str(sys.argv[4]), '%d/%m/%Y').date()
    to_date = datetime.strptime(str(sys.argv[5]), '%d/%m/%Y').date()
    
    from_date = from_date - timedelta(days=max(20,min(4*(int)(sys.argv[3]),1000)))
    
    raw = fetch_raw_data(stock_code,from_date,to_date)
    save_csv(raw,stock_code)
  
  elif strategy=="MACD":
    from_date = datetime.strptime(str(sys.argv[3]), '%d/%m/%Y').date()
    to_date = datetime.strptime(str(sys.argv[4]), '%d/%m/%Y').date()
    raw = fetch_raw_data(stock_code,from_date,to_date)
    save_csv(raw,stock_code)

  elif strategy=="ADX":
    from_date = datetime.strptime(str(sys.argv[4]), '%d/%m/%Y').date()
    to_date = datetime.strptime(str(sys.argv[5]), '%d/%m/%Y').date()
    from_date = from_date - timedelta(days=30)
    raw = fetch_raw_data(stock_code,from_date,to_date)
    save_csv(raw,stock_code)
  
  elif strategy=="LINEAR_REGRESSION":
    from_date = datetime.strptime(str(sys.argv[3]), '%d/%m/%Y').date()
    from_date = from_date - timedelta(days=10)
    to_date = datetime.strptime(str(sys.argv[4]), '%d/%m/%Y').date()
    raw = fetch_raw_data(stock_code,from_date,to_date)
    save_csv(raw,stock_code)

    train_from_date = datetime.strptime(str(sys.argv[5]), '%d/%m/%Y').date()
    train_to_date = datetime.strptime(str(sys.argv[6]), '%d/%m/%Y').date()
    raw = fetch_raw_data(stock_code,train_from_date,train_to_date)
    save_csv(raw,'train')
  
  print(f"Success for {stock_code}")

except Exception as e:
  print(f"An error occurred with stock code {stock_code},{str(e)}")
