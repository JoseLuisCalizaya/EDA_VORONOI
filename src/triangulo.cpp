#include "triangulo.h"

Triangulo::Triangulo(const Punto &a, const Punto &b, const Punto &c)
    : a(a), b(b), c(c) {}

bool Triangulo::contieneVertice(const Punto &p) const {
  const double EPS = 1e-9;
  return contieneVertice(p, EPS);
}

bool Triangulo::contieneVertice(const Punto &p, double eps) const {
  return (a == p) || (b == p) || (c == p);
}
