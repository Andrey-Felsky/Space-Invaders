# compilador padrão (cada um pode sobrescrever com make CXX=g++)
CXX ?= g++

# flags de compilação
CXXFLAGS = -Wall -Wextra -g3 -Iinclude

# arquivos fonte
SRC = main.cpp
OBJ = $(SRC:.cpp=.o)

# pasta de saída
OUTDIR = output
OUTFILE = $(OUTDIR)/main.exe

# regra padrão
all: $(OUTFILE)

# regra para compilar
$(OUTFILE): $(SRC)
	mkdir -p $(OUTDIR)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUTFILE)

clean:
	rm -rf $(OUTDIR)
	rm -f *.o
