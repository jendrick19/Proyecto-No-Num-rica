#ifndef COLA_H
#define COLA_H
#include "../include/nodo.h"

template <class Tipo>
class Cola {
    nodo<Tipo> *frente, *final;

public:
    Cola();
    ~Cola();
    bool Vacia() const;
    bool Llena() const;
    bool Insertar(Tipo Valor);
    bool Remover(Tipo &Valor);
    int Total() const; 
    bool VerFrente(Tipo &Valor) const; 
};

#include "Cola.cpp"
#endif
