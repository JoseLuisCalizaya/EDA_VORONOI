#ifndef TRIANGULO_H
#define TRIANGULO_H

#include "punto.h"

class Triangulo {
public:
  Punto a, b, c;

  Triangulo(const Punto &a, const Punto &b, const Punto &c);

  bool contieneVertice(const Punto &p) const;
  bool contieneVertice(const Punto &p, double eps) const;
};

#endif
