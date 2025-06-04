CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude -Isrc -I.
LDFLAGS = -mconsole -Llib
LDLIBS = -lBearLibTerminal

SRC = main.cpp
OUTDIR = output
OUTFILE = $(OUTDIR)/main.exe
DLL = BearLibTerminal.dll

all: $(OUTFILE)

$(OUTFILE): $(SRC) $(DLL)
	mkdir -p $(OUTDIR)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUTFILE) $(LDFLAGS) $(LDLIBS)
	cp $(DLL) $(OUTDIR)/

clean:
	rm -rf $(OUTDIR)
	rm -f *.o
