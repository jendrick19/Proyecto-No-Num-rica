#ifndef PILA_H
#define PILA_H
#include "../include/nodo.h"

template <class Tipo>
class Pila {
    nodo<Tipo> *Tope;

public:
    Pila();
    ~Pila();
    bool Vacia();
    bool Llena();
    bool Insertar(Tipo Valor); 
    bool Remover(Tipo &Valor); 
    
    int Total();               
    bool VerTope(Tipo &Valor); 
    bool VerFondo(Tipo &Valor);
};

#include "Pila.cpp"
#endif
