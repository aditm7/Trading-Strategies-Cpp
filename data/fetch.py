from jugaad_data import util as ut
from jugaad_data.nse import stock_df, stock_raw

import pandas as pd
from datetime import date,datetime,timedelta
import time
import os
import sys

# Required headers are: Date, Open Price, Close Price, High, Low, Last Trade Price (LTP), Volume, Value and Number of Trades
stock_select_headers = [ "CH_TIMESTAMP", 
                    "CH_OPENING_PRICE", "CH_TRADE_HIGH_PRICE",
                    "CH_TRADE_LOW_PRICE",
                    "CH_LAST_TRADED_PRICE", "CH_CLOSING_PRICE",
                    "CH_TOT_TRADED_QTY", "CH_TOT_TRADED_VAL", "CH_TOTAL_TRADES","CH_SYMBOL"]
stock_final_headers = [ "date",
                    "open", "high",
                    "low",
                    "ltp", "close",
                    "volume", "value", "no_of_trades","symbol"]
stock_dtypes = [ ut.np_date,
            ut.np_float, ut.np_float,
            ut.np_float,
            ut.np_float, ut.np_float,
            ut.np_int, ut.np_float, ut.np_int,str]

stock_code = str(sys.argv[2])
from_date = datetime.strptime(str(sys.argv[3]), '%d/%m/%Y').date()
to_date = datetime.strptime(str(sys.argv[4]), '%d/%m/%Y').date()

if(len(sys.argv)==5): # subtract the number of days accordingly n passed
  from_date = from_date - timedelta(days=max(10,int(sys.argv[4])*2))

def fetch_raw_data():
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

def save_csv(data):
  df = pd.DataFrame(data)
  df['date'] = df['date'].dt.strftime('%d/%m/%Y')
  df=df.iloc[::-1]
  df.to_csv(f'data/{stock_code}.csv', index=False)

try:
  file_path = f'data/{stock_code}.csv'
  raw = fetch_raw_data()
  save_csv(raw)
  print(f"Success for {stock_code}")

except Exception as e:
  print(0)
  print(f"An error occurred with stock code {stock_code},{str(e)}")
