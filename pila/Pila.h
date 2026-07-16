#ifndef PILA_H
#define PILA_H
#include "../include/nodo.h"

template <class Tipo>
class Pila {
    nodo<Tipo> *Tope;

public:
    Pila();
    ~Pila();
    bool Vacia() const;
    bool Llena() const;
    bool Insertar(Tipo Valor); 
    bool Remover(Tipo &Valor); 
    
    int Total() const;               
    bool VerTope(Tipo &Valor) const; 
    bool VerFondo(Tipo &Valor) const;
};

#include "Pila.cpp"
#endif
