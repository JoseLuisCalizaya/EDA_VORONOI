# EDA_VORONOI

## Instrucciones de Uso

### 1. Clonar el repositorio

Clona el proyecto en tu máquina local y entra al directorio:

```bash
git clone https://github.com/JoseLuisCalizaya/EDA_VORONOI.git
cd EDA_VORONOI
```

### 2. Compilar el código C++
```bash
make run
```
El programa leerá los archivos de texto, calculará los circuncentros, generará la triangulación y extraerá las aristas finitas de Voronoi. Al finalizar, exportará automáticamente tres archivos .csv en la misma carpeta:

   * puntos.csv: Coordenadas de los puntos originales.
   * triangulos.csv: Segmentos que conforman la triangulación de Delaunay.
   * voronoi_aristas.csv: Segmentos que conforman el diagrama de Voronoi.

### 3. Visualizar los resultados
Una vez que el programa en C++ haya generado los archivos .csv, ejecuta el script 
de Python para renderizar el gráfico final:

```bash
python draw.py
```

Este script leerá los datos exportados y generará automáticamente una imagen en alta resolución llamada voronoi_delaunay.png en el directorio actual, mostrando los triángulos de Delaunay en gris, el diagrama de Voronoi en azul y los puntos originales en negro.
