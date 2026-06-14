#ifndef ARISTA_H
#define ARISTA_H

#include "punto.h"

class Arista {
public:
  Punto p1, p2;

  Arista(const Punto &p1, const Punto &p2);
};

#endif
