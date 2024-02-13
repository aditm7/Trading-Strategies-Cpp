# COP290 Assignment 1 Task 3
## Team members
- Etiksha Jain (2020EE10491)
- Adit Malhotra (2020EE10458)

## Tuning LR strategy
During the execution of best_of_all strategy we observed that the Basic strategy outperformed others by a great margin. But Linear regression was also giving some promising results.

Inspired from the plain Linear-Regression model, we made some tweaks and formed a sliding window training model. From the starting date, we are taking window of 30 days, train the LR model, and then trade on the 31st day. This window with a stride of 1, is giving us the best profit.

A point to be noted here is that with this our number of trades also went up, thereby incresing the transactions cost(we didn't take that into account in this model)

We also noted that 7th weight in the LR model after training (the one corresponding to the Number of Trades) has very low value, so we hardcoded it to be 0 after the training. After all these tunings, we ran the strategy and got the net profit of 1114.4 compared to earlier 101.5.