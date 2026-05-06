template <class Tipo>
Cola<Tipo>::Cola() {
    frente = NULL;
    final = NULL;
}

template <class Tipo>
bool Cola<Tipo>::Vacia() {
    return frente == NULL;
}

template <class Tipo>
bool Cola<Tipo>::Llena() {
    nodo<Tipo> *p = new nodo<Tipo>;
    if (p == NULL) return true;
    delete p;
    return false;
}

template <class Tipo>
bool Cola<Tipo>::Insertar(Tipo valor) {
    if (!Llena()) {
        nodo<Tipo> *nuevo = new nodo<Tipo>;
        nuevo->AsigInfo(valor);
        nuevo->AsigDer(NULL);
        if (final == NULL) frente = nuevo;
        else final->AsigDer(nuevo);
        final = nuevo;
        return true;
    }
    return false;
}

template <class Tipo>
bool Cola<Tipo>::Remover(Tipo &valor) {
    if (!Vacia()) {
        nodo<Tipo> *primero = frente;
        valor = primero->ObtInfo();
        frente = primero->ObtDer();
        if (frente == NULL) final = NULL;
        delete primero;
        return true;
    }
    return false;
}

template <class Tipo>
int Cola<Tipo>::Total() {
    nodo<Tipo> *p = frente;
    int cont = 0;
    while (p != NULL) {
        cont++;
        p = p->ObtDer();
    }
    return cont;
}

template <class Tipo>
bool Cola<Tipo>::VerFrente(Tipo &valor) {
    if (!Vacia()) {
        valor = frente->ObtInfo();
        return true;
    }
    return false;
}

template <class Tipo>
Cola<Tipo>::~Cola() {
    Tipo aux;
    while (!Vacia()) Remover(aux);
}
