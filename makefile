# Compilador e flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -static-libgcc -Iinclude -Isrc -I.
LDFLAGS = -mconsole

# Fontes existentes no projeto
SRC = \
    main.cpp \
    src/utils/cleanScreen/cleanScreen.cpp \
    src/ranking/score.cpp \
    src/menu/menu.cpp \
    src/mapa/mapa.cpp \
    src/logic/logic.cpp \
    src/enemy/enemy.cpp 

# Gera nomes dos arquivos .o
OBJ = $(SRC:.cpp=.o)

# Nome do executável final
TARGET = main.exe

# Regra principal
all: $(TARGET)

# Linkagem
$(TARGET): $(OBJ)
	$(CXX) $(LDFLAGS) $(OBJ) -o $(TARGET)

# Regra para compilar .cpp → .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpeza (Windows)
clean:
	rm -f $(OBJ) $(TARGET)
