#ifndef COLA_H
#define COLA_H
#include "nodo.h"

template <class Tipo>
class Cola {
    nodo<Tipo> *frente, *final;

public:
    Cola();
    ~Cola();
    bool Vacia();
    bool Llena();
    bool Insertar(Tipo Valor);
    bool Remover(Tipo &Valor);
    
    // Métodos de eficiencia añadidos
    int Total(); 
    bool VerFrente(Tipo &Valor); 
};

#include "Cola.cpp"
#endif