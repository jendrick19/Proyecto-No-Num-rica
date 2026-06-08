template <class Tipo>
Pila<Tipo>::Pila() { Tope = NULL; }

template <class Tipo>
bool Pila<Tipo>::Vacia() { return Tope == NULL; }

template <class Tipo>
bool Pila<Tipo>::Llena() {
    nodo<Tipo> *p = new nodo<Tipo>;

    if (p == NULL) return true;

    delete p;

    return false;
}

template <class Tipo>
bool Pila<Tipo>::Insertar(Tipo Valor) {
    if (!Llena()) {
        nodo<Tipo> *nuevo = new nodo<Tipo>;

        nuevo->AsigInfo(Valor);
        nuevo->AsigArriba(Tope);
  
        Tope = nuevo;  
             
        return true;
    }

    return false;
}

template <class Tipo>
bool Pila<Tipo>::Remover(Tipo &Valor) {
    if (!Vacia()) {
        nodo<Tipo> *primero = Tope;

        Valor = primero->ObtInfo();
        Tope = primero->ObtArriba();
       
        delete primero;   

        return true;
    }

    return false;
}

template <class Tipo>
int Pila<Tipo>::Total() {
    nodo<Tipo> *p = Tope;

    int cont = 0;

    while (p != NULL) {
        cont++;
        p = p->ObtArriba();
    }

    return cont;
}

template <class Tipo>
bool Pila<Tipo>::VerTope(Tipo &Valor) {
    if (!Vacia()) {

        Valor = Tope->ObtInfo();

        return true;
    }

    return false;
}

template <class Tipo>
bool Pila<Tipo>::VerFondo(Tipo &Valor) {
    if (!Vacia()) {
        nodo<Tipo> *p = Tope;

        while (p->ObtArriba() != NULL) p = p->ObtArriba();

        Valor = p->ObtInfo();

        return true;
    }

    return false;
}

template <class Tipo>
Pila<Tipo>::~Pila() {

    Tipo aux;
    
    while (!Vacia()) Remover(aux);
}
