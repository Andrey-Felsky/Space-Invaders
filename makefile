CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude -Isrc -I. -j
LDFLAGS = -mconsole -Llib
LDLIBS = -lBearLibTerminal

SRC = main.cpp
OUTDIR = output
OUTFILE = $(OUTDIR)/main.exe

all: $(OUTFILE)

$(OUTFILE): $(SRC)
	mkdir -p $(OUTDIR)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUTFILE) $(LDFLAGS) $(LDLIBS)

clean:
	rm -rf $(OUTDIR)
	rm -f *.o
