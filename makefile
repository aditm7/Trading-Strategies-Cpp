BINDIR = bin
INCDIR = include
SRCDIR = src
TARGET = $(BINDIR)/main

CPP = g++
CPPFLAGS = -O3 -I$(INCDIR) -MMD -Wall -std=c++17 -pthread -DDEBUG

OBJS = $(patsubst $(SRCDIR)/%.cpp, $(BINDIR)/%.o, $(wildcard $(SRCDIR)/*.cpp))

all: $(TARGET)
# @chmod +x scripts/*.sh
# python3 data/fetch.py $(strategy) $(symbol) $(n) $(start_date) $(end_date) $(train_start_date) $(train_end_date)
# ./bin/main $(strategy) $(symbol) $(n) $(x) $(p) $(max_hold_days) $(c1) $(c2) $(oversold_threshold) $(overbought_threshold) $(adx_threshold) $(threshold) $(stop_loss_threshold) $(start_date) $(end_date) $(train_start_date) $(train_end_date)

$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	@$(CPP) -c $(CPPFLAGS) $< -o $@

$(TARGET): $(OBJS)
	@$(CPP) $(CPPFLAGS) $^ -o $@

-include $(BINDIR)/*.d

.PHONY: clean all
clean:
	@rm -f $(BINDIR)/*.o $(BINDIR)/*.d $(BINDIR)/main*