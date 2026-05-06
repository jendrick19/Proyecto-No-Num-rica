#ifndef PILA_H
#define PILA_H
#include "nodo.h"

template <class Tipo>
class Pila {
    nodo<Tipo> *Tope; // Apuntador al último elemento insertado

public:
    Pila();
    ~Pila();
    bool Vacia();
    bool Llena();
    bool Insertar(Tipo Valor); // Operación PUSH
    bool Remover(Tipo &Valor); // Operación POP
    
    // Métodos de eficiencia (Nivel de Implementación)
    int Total();               // Cuenta elementos sin usar auxiliares
    bool VerTope(Tipo &Valor); // Permite "espiar" el tope sin sacarlo
    bool VerFondo(Tipo &Valor);// Actividad sugerida en PDF
};

#include "Pila.cpp"
#endif