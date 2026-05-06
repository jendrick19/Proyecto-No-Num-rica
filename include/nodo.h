#ifndef NODO_H
#define NODO_H
#include <iostream>

template <class Tipo>
class nodo {
    Tipo info;
    nodo<Tipo> *der; // También llamado 'prox' en algunos ejemplos
public:
    nodo() { der = NULL; }
    void AsigInfo(Tipo v) { info = v; }
    Tipo ObtInfo() { return info; }
    void AsigDer(nodo<Tipo> *p) { der = p; }
    nodo<Tipo>* ObtDer() { return der; }
};
#endif
