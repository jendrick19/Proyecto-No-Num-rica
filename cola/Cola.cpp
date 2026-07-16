template <class Tipo>
Cola<Tipo>::Cola() {
    frente = NULL;
    final = NULL;
}

template <class Tipo>
bool Cola<Tipo>::Vacia() const {
    return frente == NULL;
}

template <class Tipo>
bool Cola<Tipo>::Llena() const {
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
        nuevo->AsigSig(NULL);

        if (final == NULL) frente = nuevo;

        else final->AsigSig(nuevo);

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
        frente = primero->ObtSig();

        if (frente == NULL) final = NULL;

        delete primero;

        return true;
    }

    return false;
}

template <class Tipo>
int Cola<Tipo>::Total() const {
    nodo<Tipo> *p = frente;

    int cont = 0;

    while (p != NULL) {
        cont++;
        p = p->ObtSig();
    }

    return cont;
}

template <class Tipo>
bool Cola<Tipo>::VerFrente(Tipo &valor) const {
    if (!Vacia()) {
        valor = frente->ObtInfo();
        return true;
    }

    return false;
}

template <class Tipo>
Cola<Tipo>::~Cola() {
    Tipo aux;

    while (!Vacia()) {

        Remover(aux);
        
    };
}
