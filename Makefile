# Variáveis de Compilação
CXX      = g++
CXXFLAGS = -Wall -std=c++11 -Iinclude
LDFLAGS  = 

# Pastas
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Arquivos
# Busca todos os arquivos .cpp na pasta src
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
# Cria uma lista de arquivos .o na pasta obj
OBJECTS := $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
# Nome do executável final
TARGET  = $(BIN_DIR)/job_shop_solver.exe

# Regra padrão (é a que roda quando você digita apenas 'make')
all: $(TARGET)

# Regra para criar o executável
$(TARGET): $(OBJECTS)
	@if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo ==========================================
	@echo  Compilacao concluida: $(TARGET)
	@echo ==========================================

# Regra para compilar os arquivos .cpp em .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regra para limpar os arquivos temporários e o executável
clean:
	@if exist $(OBJ_DIR) rd /s /q $(OBJ_DIR)
	@if exist $(BIN_DIR) rd /s /q $(BIN_DIR)
	@echo Limpeza concluida.

# Evita conflitos com arquivos de mesmo nome das regras
.PHONY: all clean