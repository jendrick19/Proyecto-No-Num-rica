#ifndef LISTA_H
#define LISTA_H
#include "../include/nodo.h"

template <class Tipo>
class Lista {
    nodo<Tipo> *primero;

public:
    Lista();
    ~Lista();
    nodo<Tipo>* ObtPrimero();
    void AsigPrimero(nodo<Tipo> *p);
    bool Vacia();
    bool Llena();
    

    bool InsComienzo(Tipo Valor);
    bool EliComienzo(Tipo &Valor);
    bool InsDerecho(nodo<Tipo> *p, Tipo Valor);
    bool EliDerecho(nodo<Tipo> *p, Tipo &Valor);
    
 
    int Total();
};

#include "Lista.cpp"
#endif
