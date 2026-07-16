#ifndef NODO_H
#define NODO_H
#include <iostream>

template <class Tipo>
class nodo {
    Tipo info;
    nodo<Tipo> *der;
public:
    nodo() { der = NULL; }
    void AsigInfo(Tipo v) { info = v; }
    Tipo& ObtInfo() { return info; }
    const Tipo& ObtInfo() const { return info; }
    void AsigDer(nodo<Tipo> *p) { der = p; }
    nodo<Tipo>* ObtDer() { return der; }
    const nodo<Tipo>* ObtDer() const { return der; }
    void AsigSig(nodo<Tipo> *p) { der = p; }
    nodo<Tipo>* ObtSig() { return der; }
    const nodo<Tipo>* ObtSig() const { return der; }
    void AsigArriba(nodo<Tipo> *p) { der = p; }
    nodo<Tipo>* ObtArriba() { return der; }
    const nodo<Tipo>* ObtArriba() const { return der; }
};

#endif
