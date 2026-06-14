#include "punto.h"

Punto::Punto(double x, double y) : x(x), y(y) {}

double Punto::distancia(const Punto &otro) const {
  double dx = x - otro.x, dy = y - otro.y;
  return std::sqrt(dx * dx + dy * dy);
}

double Punto::distanciaCuadrado(const Punto &otro) const {
  double dx = x - otro.x, dy = y - otro.y;
  return dx * dx + dy * dy;
}

bool Punto::operator==(const Punto &otro) const {
  const double EPS = 1e-9;
  return std::fabs(x - otro.x) < EPS && std::fabs(y - otro.y) < EPS;
}
