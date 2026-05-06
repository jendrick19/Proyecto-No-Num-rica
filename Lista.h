#ifndef LISTA_H
#define LISTA_H
#include "nodo.h"

template <class Tipo>
class Lista {
    nodo<Tipo> *primero; // Apuntador externo para acceder a la lista

public:
    Lista();
    ~Lista();
    nodo<Tipo>* ObtPrimero();
    void AsigPrimero(nodo<Tipo> *p);
    bool Vacia();
    bool Llena();
    
    // Operaciones Lógicas
    bool InsComienzo(Tipo Valor);
    bool EliComienzo(Tipo &Valor);
    bool InsDerecho(nodo<Tipo> *p, Tipo Valor);
    bool EliDerecho(nodo<Tipo> *p, Tipo &Valor);
    
    // Método de eficiencia
    int Total(); // Actividad sugerida en PDF
};

#include "Lista.cpp"
#endif