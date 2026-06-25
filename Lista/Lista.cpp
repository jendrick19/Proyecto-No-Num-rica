#ifndef LISTA_CPP
#define LISTA_CPP

#include "Lista.h"

template <class Tipo>
Lista<Tipo>::Lista() { primero = NULL; }

template <class Tipo>
nodo<Tipo>* Lista<Tipo>::ObtPrimero() { return primero; }

template <class Tipo>
void Lista<Tipo>::AsigPrimero(nodo<Tipo> *p) { primero = p; }

template <class Tipo>
bool Lista<Tipo>::Vacia() { return primero == NULL; }

template <class Tipo>
bool Lista<Tipo>::Llena() {
    nodo<Tipo> *p = new nodo<Tipo>;
    if (p == NULL) return true;
    delete p;
    return false;
}

template <class Tipo>
bool Lista<Tipo>::InsComienzo(Tipo Valor) {
    if (!Llena()) {
        nodo<Tipo> *nuevo = new nodo<Tipo>;

        nuevo->AsigInfo(Valor);
        nuevo->AsigDer(primero);
       
        primero = nuevo;   
             
        return true;
    }

    return false;
}

template <class Tipo>
bool Lista<Tipo>::EliComienzo(Tipo &Valor) {
    if (!Vacia()) {
        nodo<Tipo> *viejo = primero;

        Valor = viejo->ObtInfo();
        primero = primero->ObtDer();

        delete viejo;

        return true;
    }

    return false;
}

template <class Tipo>
bool Lista<Tipo>::InsDerecho(nodo<Tipo> *p, Tipo Valor) {
    if (!Llena() && p != NULL) {
        nodo<Tipo> *nuevo = new nodo<Tipo>;

        nuevo->AsigInfo(Valor);
        nuevo->AsigDer(p->ObtDer());

        p->AsigDer(nuevo); 

        return true;
    }

    return false;
}

template <class Tipo>
bool Lista<Tipo>::EliDerecho(nodo<Tipo> *p, Tipo &Valor) {
    if (p != NULL && p->ObtDer() != NULL) {
        nodo<Tipo> *viejo = p->ObtDer();

        Valor = viejo->ObtInfo();

        p->AsigDer(viejo->ObtDer());

        delete viejo;

        return true;
    }

    return false;
}

template <class Tipo>
int Lista<Tipo>::Total() {
    nodo<Tipo> *p = primero;

    int cont = 0;

    while (p != NULL) { 
        cont++;
        p = p->ObtDer();
    }

    return cont;
}

template <class Tipo>
Lista<Tipo>::~Lista() {
    Tipo aux;
    while (!Vacia()) EliComienzo(aux);
}

#endif
