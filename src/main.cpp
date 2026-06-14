#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>

#include "../include/arista.h"
#include "../include/punto.h"
#include "../include/triangulo.h"

using namespace std;

const double EPS = 1e-9;

// ACTIVIDAD 1
Punto circuncentro(const Punto &A, const Punto &B, const Punto &C) {
  double D = 2.0 * (A.x * (B.y - C.y) + B.x * (C.y - A.y) + C.x * (A.y - B.y));
  if (fabs(D) < EPS)
    return Punto(0, 0);
  double Ux = ((A.x * A.x + A.y * A.y) * (B.y - C.y) +
               (B.x * B.x + B.y * B.y) * (C.y - A.y) +
               (C.x * C.x + C.y * C.y) * (A.y - B.y)) /
              D;
  double Uy = ((A.x * A.x + A.y * A.y) * (C.x - B.x) +
               (B.x * B.x + B.y * B.y) * (A.x - C.x) +
               (C.x * C.x + C.y * C.y) * (B.x - A.x)) /
              D;
  return Punto(Ux, Uy);
}

// ACTIVIDAD 2
bool puntoEnCircuncirculo(const Punto &p, const Punto &a, const Punto &b,
                          const Punto &c) {
  Punto centro = circuncentro(a, b, c);
  double radio2 = centro.distanciaCuadrado(a);
  double dist2 = centro.distanciaCuadrado(p);
  return dist2 <= radio2 + EPS;
}

class Circuncirculo {
public:
  Punto centro;
  double radio2;
  Circuncirculo(const Punto &a, const Punto &b, const Punto &c)
      : centro(circuncentro(a, b, c)), radio2(centro.distanciaCuadrado(a)) {}
  bool contiene(const Punto &p) const {
    return centro.distanciaCuadrado(p) <= radio2 + EPS;
  }
};

// Algoritmo Bowyer-Watson
vector<Triangulo> delaunayBowyerWatson(vector<Punto> &puntos) {
  double minX = puntos[0].x, minY = puntos[0].y;
  double maxX = puntos[0].x, maxY = puntos[0].y;
  for (const auto &p : puntos) {
    minX = min(minX, p.x);
    maxX = max(maxX, p.x);
    minY = min(minY, p.y);
    maxY = max(maxY, p.y);
  }
  double dx = maxX - minX, dy = maxY - minY;
  double margin = max(dx, dy) * 10.0;
  Punto supA(minX - margin, minY - margin);
  Punto supB(maxX + margin, minY - margin);
  Punto supC(minX - margin, maxY + margin);

  int idxA = puntos.size();
  int idxB = idxA + 1;
  int idxC = idxB + 1;
  puntos.push_back(supA);
  puntos.push_back(supB);
  puntos.push_back(supC);

  struct TrianguloIndices {
    int a, b, c;
    TrianguloIndices(int a, int b, int c) : a(a), b(b), c(c) {}
  };
  vector<TrianguloIndices> triangulosIdx;
  triangulosIdx.emplace_back(idxA, idxB, idxC);

  for (int i = 0; i < (int)puntos.size() - 3; ++i) {
    const Punto &p = puntos[i];
    vector<int> invalidos;
    for (int j = 0; j < (int)triangulosIdx.size(); ++j) {
      const auto &t = triangulosIdx[j];
      if (puntoEnCircuncirculo(p, puntos[t.a], puntos[t.b], puntos[t.c])) {
        invalidos.push_back(j);
      }
    }
    // Cavidad
    map<pair<int, int>, int> contadorAristas;
    for (int idx : invalidos) {
      const auto &t = triangulosIdx[idx];
      pair<int, int> aristas[3] = {{min(t.a, t.b), max(t.a, t.b)},
                                   {min(t.b, t.c), max(t.b, t.c)},
                                   {min(t.c, t.a), max(t.c, t.a)}};
      for (auto &ar : aristas)
        contadorAristas[ar]++;
    }
    vector<pair<int, int>> borde;
    for (auto &[ar, cnt] : contadorAristas) {
      if (cnt == 1)
        borde.push_back(ar);
    }
    sort(invalidos.rbegin(), invalidos.rend());
    for (int idx : invalidos) {
      triangulosIdx.erase(triangulosIdx.begin() + idx);
    }
    for (auto &ar : borde) {
      triangulosIdx.emplace_back(ar.first, ar.second, i);
    }
  }

  vector<Triangulo> resultado;
  for (const auto &tIdx : triangulosIdx) {
    if (tIdx.a >= idxA || tIdx.b >= idxA || tIdx.c >= idxA)
      continue;
    resultado.emplace_back(puntos[tIdx.a], puntos[tIdx.b], puntos[tIdx.c]);
  }
  puntos.pop_back();
  puntos.pop_back();
  puntos.pop_back();
  return resultado;
}

// Obtener aristas del diagrama de Voronoi (finitas)
vector<Arista> obtenerAristasVoronoi(const vector<Punto> &puntos,
                                     const vector<Triangulo> &triangulos) {
  vector<Punto> centros;
  for (const auto &t : triangulos) {
    centros.push_back(circuncentro(t.a, t.b, t.c));
  }

  map<pair<double, double>, int> puntoAIndex;
  for (int i = 0; i < (int)puntos.size(); ++i) {
    puntoAIndex[{puntos[i].x, puntos[i].y}] = i;
  }

  auto obtenerIndice = [&](const Punto &p) -> int {
    return puntoAIndex[{p.x, p.y}];
  };

  map<pair<int, int>, vector<int>> aristaATriangulos;
  for (int i = 0; i < (int)triangulos.size(); ++i) {
    const auto &t = triangulos[i];
    int i1 = obtenerIndice(t.a);
    int i2 = obtenerIndice(t.b);
    int i3 = obtenerIndice(t.c);
    pair<int, int> aristas[3] = {{min(i1, i2), max(i1, i2)},
                                 {min(i2, i3), max(i2, i3)},
                                 {min(i3, i1), max(i3, i1)}};
    for (auto &ar : aristas) {
      aristaATriangulos[ar].push_back(i);
    }
  }

  vector<Arista> aristasVoronoi;
  for (auto &[ar, tris] : aristaATriangulos) {
    if (tris.size() == 2) {
      aristasVoronoi.emplace_back(centros[tris[0]], centros[tris[1]]);
    }
  }
  return aristasVoronoi;
}

// ================== EXPORTACIÓN A CSV ==================
void exportarPuntosCSV(const vector<Punto> &puntos, const string &nombre) {
  ofstream f(nombre);
  f << "x,y\n";
  for (const auto &p : puntos)
    f << p.x << "," << p.y << "\n";
  f.close();
}

void exportarTriangulosAristasCSV(const vector<Triangulo> &triangulos,
                                  const string &nombre) {
  ofstream f(nombre);
  f << "x1,y1,x2,y2\n";
  for (const auto &t : triangulos) {
    auto arista = [&](const Punto &a, const Punto &b) {
      f << a.x << "," << a.y << "," << b.x << "," << b.y << "\n";
    };
    arista(t.a, t.b);
    arista(t.b, t.c);
    arista(t.c, t.a);
  }
  f.close();
}

void exportarAristasVoronoiCSV(const vector<Arista> &aristas,
                               const string &nombre) {
  ofstream f(nombre);
  f << "x1,y1,x2,y2\n";
  for (const auto &a : aristas)
    f << a.p1.x << "," << a.p1.y << "," << a.p2.x << "," << a.p2.y << "\n";
  f.close();
}

// ACTIVIDAD 8 (medición de tiempos)
void actividad8() {
  vector<int> tamanios = {50, 100, 150, 200, 250};
  cout << "\n=== ACTIVIDAD 8: MEDICIÓN DE TIEMPOS ===\n";
  for (int n : tamanios) {
    string nombreArchivo = "data/puntos_" + to_string(n) + ".txt";
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
      cout << "No se encontró " << nombreArchivo << ". Simulando con " << n
           << " puntos aleatorios.\n";
      continue;
    }
    vector<Punto> puntos;
    double x, y;
    while (archivo >> x >> y)
      puntos.emplace_back(x, y);
    archivo.close();
    if ((int)puntos.size() != n) {
      cout << "Advertencia: " << nombreArchivo << " tiene " << puntos.size()
           << " puntos, se esperaban " << n << "\n";
    }
    vector<Punto> copia = puntos;
    auto start = chrono::high_resolution_clock::now();
    vector<Triangulo> triangulos = delaunayBowyerWatson(copia);
    auto end = chrono::high_resolution_clock::now();
    double tiempo = chrono::duration<double>(end - start).count();
    cout << "n = " << n << ": " << triangulos.size()
         << " triangulos, tiempo = " << tiempo << " s\n";
  }
}

int main() {
  vector<Punto> puntos;
  ifstream archivo("data/puntos.txt");
  if (!archivo.is_open()) {
    cerr << "Error: no se pudo abrir puntos.txt. Usando puntos de ejemplo.\n";
    puntos = {Punto(10, 20), Punto(35, 42), Punto(70, 11), Punto(15, 60),
              Punto(80, 75), Punto(55, 30), Punto(40, 90)};
  } else {
    double x, y;
    while (archivo >> x >> y) {
      puntos.emplace_back(x, y);
    }
    archivo.close();
  }

  cout << "Puntos cargados:\n";
  for (const auto &p : puntos)
    cout << "(" << p.x << ", " << p.y << ")\n";
  cout << "\n";

  // Actividades 1 y 2
  if (puntos.size() >= 3) {
    Punto A = puntos[0], B = puntos[1], C = puntos[2];
    Punto centro = circuncentro(A, B, C);
    double radio = A.distancia(centro);
    cout << "ACTIVIDAD 1 y 2\n";
    cout << "Triangulo: (" << A.x << "," << A.y << ") (" << B.x << "," << B.y
         << ") (" << C.x << "," << C.y << ")\n";
    cout << "Circuncentro: (" << centro.x << ", " << centro.y << ")\n";
    cout << "Radio del circuncirculo: " << radio << "\n";
    Punto prueba(50, 50);
    bool dentro = puntoEnCircuncirculo(prueba, A, B, C);
    cout << "¿Punto (50,50) dentro del circuncirculo? "
         << (dentro ? "Si" : "No") << "\n\n";
  }

  // Triangulación de Delaunay
  vector<Punto> copiaPuntos = puntos;
  vector<Triangulo> triangulos = delaunayBowyerWatson(copiaPuntos);

  cout << "ACTIVIDAD 3: Triangulación de Delaunay\n";
  cout << "Numero de triangulos: " << triangulos.size() << "\n";
  for (size_t i = 0; i < triangulos.size(); ++i) {
    const auto &t = triangulos[i];
    cout << "Triangulo " << i << ": (" << t.a.x << "," << t.a.y << ") ("
         << t.b.x << "," << t.b.y << ") (" << t.c.x << "," << t.c.y << ")\n";
  }
  cout << "\n";

  // Obtener aristas de Voronoi y exportar a CSV
  vector<Arista> aristasVoronoi = obtenerAristasVoronoi(puntos, triangulos);
  exportarPuntosCSV(puntos, "puntos.csv");
  exportarTriangulosAristasCSV(triangulos, "triangulos.csv");
  exportarAristasVoronoiCSV(aristasVoronoi, "voronoi_aristas.csv");

  cout << "Archivos CSV generados:\n";
  cout << " - puntos.csv\n - triangulos.csv\n - voronoi_aristas.csv\n";

  // Estadísticas de Voronoi
  cout << "\nRESULTADOS DIAGRAMA DE VORONOI\n";
  cout << "numero de regiones: " << puntos.size() << "\n";
  cout << "numero de vertices Voronoi: " << aristasVoronoi.size()
       << " (aprox)\n";
  cout << "numero de aristas Voronoi finitas: " << aristasVoronoi.size()
       << "\n";

  actividad8();

  return 0;
}
