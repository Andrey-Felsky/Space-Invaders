# Compilador
CXX = g++
CXXFLAGS = -Wall -Wextra -g3

# Diretórios
SRC_DIR = .
OUT_DIR = output

# Arquivo final
TARGET = $(OUT_DIR)/main.exe

# Lista de arquivos fonte
SRCS = \
    $(SRC_DIR)/main.cpp \
    $(SRC_DIR)/enemy/enemy.cpp \
    $(SRC_DIR)/logic/logic.cpp \
    $(SRC_DIR)/mapa/mapa.cpp \
    $(SRC_DIR)/menu/menu.cpp \
    $(SRC_DIR)/ranking/score.cpp \
    $(SRC_DIR)/utils/cleanScreen/cleanScreen.cpp \
    $(SRC_DIR)/utils/gameElements.cpp

# Gera lista de objetos .o para cada fonte .cpp
OBJS = $(SRCS:.cpp=.o)

# Regra padrão
all: $(TARGET)

# Como gerar o executável
$(TARGET): $(SRCS)
	@mkdir -p $(OUT_DIR)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

# Limpeza dos arquivos compilados
clean:
	del /Q /F $(OUT_DIR)\*.exe 2> NUL || true

.PHONY: all clean
