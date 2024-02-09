BINDIR = bin
INCDIR = include
SRCDIR = src
TARGET = $(BINDIR)/main

CPP = g++
CPPFLAGS = -O3 -I$(INCDIR) -MMD -Wall -std=c++17 -DDEBUG

OBJS = $(patsubst $(SRCDIR)/%.cpp, $(BINDIR)/%.o, $(wildcard $(SRCDIR)/*.cpp))

all: $(TARGET)
	@chmod +x scripts/*.sh
	python3 data fetch.py $(strategy) $(symbol) $(train_start_date) $(train_end_date) $(start_date) $(end_date)
	./bin/main $(strategy) $(symbol) $(n) $(x) $(p) $(start_date) $(end_date)

$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	@$(CPP) -c $(CPPFLAGS) $< -o $@

$(TARGET): $(OBJS)
	@$(CPP) $(CPPFLAGS) $^ -o $@

-include $(BINDIR)/*.d

.PHONY: clean all
clean:
	@rm -f $(BINDIR)/*.o $(BINDIR)/*.d $(BINDIR)/main*