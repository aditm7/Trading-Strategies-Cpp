BINDIR = bin
INCDIR = include
SRCDIR = src
TARGET = $(BINDIR)/main
PIP_TARGET = $(BINDIR)/done.txt

CPP = g++
CPPFLAGS = -O3 -I$(INCDIR) -MMD -std=c++17 -pthread -w

OBJS = $(patsubst $(SRCDIR)/%.cpp, $(BINDIR)/%.o, $(wildcard $(SRCDIR)/*.cpp))

all: $(TARGET) $(PIP_TARGET)
	@chmod +x scripts/*.sh
	python3 data/fetch.py $(strategy) $(symbol) $(symbol1) $(symbol2) $(n) $(start_date) $(end_date) $(train_start_date) $(train_end_date)
	./bin/main $(strategy) $(symbol) $(symbol1) $(symbol2) $(n) $(x) $(p) $(max_hold_days) $(c1) $(c2) $(oversold_threshold) $(overbought_threshold) $(adx_threshold) $(threshold) $(stop_loss_threshold) $(start_date) $(end_date) $(train_start_date) $(train_end_date)

$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	@$(CPP) -c $(CPPFLAGS) $< -o $@

$(TARGET): $(OBJS)
	@$(CPP) $(CPPFLAGS) $^ -o $@

$(PIP_TARGET): 
	@pip3 install -r "data/requirements.txt"
	@echo "1" > $@

-include $(BINDIR)/*.d

.PHONY: clean all zip
clean:
	@rm -f $(BINDIR)/*.o $(BINDIR)/*.d $(BINDIR)/main*
	@rm -f *.csv *.txt

zip:
	@make clean
	@zip -r -X 2020EE10491_2020EE10458.zip bin data include src logs scripts makefile README.md -x "bin/done.txt" "data/*.csv"