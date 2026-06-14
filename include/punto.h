#ifndef PUNTO_H
#define PUNTO_H

#include <cmath>

class Punto {
public:
  double x, y;

  Punto(double x = 0, double y = 0);

  double distancia(const Punto &otro) const;
  double distanciaCuadrado(const Punto &otro) const;
  bool operator==(const Punto &otro) const;
};

#endif
