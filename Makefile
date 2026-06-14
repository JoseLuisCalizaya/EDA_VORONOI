# Compilador y flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude -O3

# Directorios
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

# Nombre del ejecutable
TARGET = $(BIN_DIR)/voronoi

# Encuentra todos los archivos .cpp en src/ y define los .o correspondientes
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Reglas principales
.PHONY: all clean run setup dirs

all: dirs $(TARGET)

# Crear directorios si no existen
dirs:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Enlazar el ejecutable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compilar los archivos objeto
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regla para compilar y ejecutar
run: all
	@echo "Ejecutando $(TARGET)..."
	@./$(TARGET)

# Limpiar los binarios generados
clean:
	@echo "Limpiando archivos de compilación..."
	rm -rf $(OBJ_DIR) $(BIN_DIR) *.csv
