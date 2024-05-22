# Popular Trading Strategies in C++
# Running the Strategy
First compile the main binary by running:
```
make
```
See the assignment PDF for more details on then running the corresponding stategy.

# Code structure and Modularity
We have followed a very structured and modular approach for the codebase. The whole project can be divided into directories : 
- include : Contains all the .h header files, containing the classes and function declaration.
- src : The .cpp files containing the respective function definitions.
- logs : Rather priniting everything in the console, we debugged systematically by using cerr.
- data: Contains the python files which fetches the stock data and saves in .csv format
- bin : Stores the object files, dependency files etc generated during the Build process

Some other things :
- We have used .csv to store all the stock_data, simply for its easy parsing, lightweightedness and user friendly previews. 
- The Makefile is configured such that all the separate object files are created for each .cpp file, all of which then linked together finally to form the main executable file. This speeds the BUILD process, without needing to build everything from scratch when a change is made in a single file. 

# Some key insights from the strategies
## DMA
- Using the past n days data we expect the stock price to be fluctuating between $mean \pm p*std\_dev $
- So if the current stock price cross the upper band, we expect the stock to further increase (maybe due a big bullish move), thereby indicating a buy signal.
- Similarly for the other side of the band.
- In real markets, investors sometimes are faced with FOMO, during these uptrends, surging the prices higher. 

## DMA++/AMA
- Adaptive moving average builds upon the naive DMA by involving volatility into the equation. 
- The ER parameter takes this volatility into account (which was missing from DMA) in a way that when the market is less fluctuating, trader can make use of DMA++/AMA indicator, and on the other hand it becomes less sensitive when the fluctuations/volatility is very high.

## MACD 
- If we solve the recursive relation for few terms that is calculate $E_{12},E_{11}$ etc, then $E_{12}$ will contain the term of $(1-\alpha)^{12} * P_0$, i.e rather giving linear weights, we expect that the influence of the price 12 days before is negligible on the current price, and these exponential weights capture this diminishing role. 

## RSI
- RSI indicator captures the momentum of the stock, basically how quickly the traders are bidding the price of the stock up or down, and plots it on scale from 0 to 100.
- From the graph on Investopedia we can observe that it tries to follow the stock data very quickly, especially the abrupt shifts in the market and captures them.
- Thus if RSI goes low and below a threshold then it means it has been oversold and is therefore poised for a rebound that mean it will go up now, so we should buy. Similarly for the 

## ADX
- Average directional indicator, again a momentum-based tool that captures the trend of price direction. We implemeneted the strategy as specified but the PnL wasn't impressive, mostly due to low frequency of trading, or non-adaptability of this trading startegy for given market  conditions.

# C++ optimisations and related things
- Used `-O3` flag while building, to sped up the runtime performance.
- Did some optimisations like using `inline functions`,`const`, `pthreads` following lock-free programming, loops interchanging to utilise memory hiearchy for matrix multiplications etc for further improvement.
- Our final performance metrics on the BEST_OF_ALL strategy are :<br>
                (10yr data range) : 0.57 real         2.04 user         0.16 sys <br> 
                (20yr data range) : 0.66 real         1.80 user         0.76 sys <br>
- While encountering the precision errors, we realised one more thing, using format specifier (.2f) in printf rounds off the decimal, where as setprecision() truncates the same.
- Used the precomputation and queue DS etc to further improve the performance of strategies.


## Pairs Strategy with Stop Loss
Same as pairs strategy, but now after the normal trade we check all the positions and then for each position we check if it's abs(zscore) exceeds threshold, if it does then close the position. Finally we check if the total net trade in the day is 0 or not, cause if it is then we don't write into the orderstats file.

# Bonus Part (Tuning LR strategy)
During the execution of best_of_all strategy we observed that the Basic strategy outperformed others by a great margin. But Linear regression was also giving some promising results.

Inspired from the plain Linear-Regression model, we made some tweaks and formed a sliding window training model. From the starting date, we are taking window of 30 days, train the LR model, and then trade on the 31st day. This window with a stride of 1, is giving us the best profit.

A point to be noted here is that with this our number of trades also went up, thereby incresing the transactions cost(we didn't take that into account in this model)

We also noted that 7th weight in the LR model after training (the one corresponding to the Number of Trades) has very low value, so we hardcoded it to be 0 after the training. After all these tunings, we ran the strategy and got the net profit of 1114.4 compared to earlier 101.5.