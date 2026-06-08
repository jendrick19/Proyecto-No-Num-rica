#include "Salas.h"

Sala::Sala() : nSala(0) {}

Sala::Sala(int numero, const std::string &pel)
    : nSala(numero), pelicula(pel) {}

Sala::Sala(const Sala &otra)
    : nSala(otra.nSala), pelicula(otra.pelicula) {
    clonarCola(otra.colaSala, colaSala);
}

Sala &Sala::operator=(const Sala &otra) {
    if (this == &otra) return *this;

    nSala = otra.nSala;
    pelicula = otra.pelicula;

    Persona aux;
    while (colaSala.Remover(aux));

    clonarCola(otra.colaSala, colaSala);

    return *this;
}

void Sala::clonarCola(const Cola<Persona> &origen, Cola<Persona> &destino) {
    Cola<Persona> &o = const_cast<Cola<Persona> &>(origen);
    if (o.Vacia()) return;

    Cola<Persona> aux;
    Persona marca;
    Persona persona;

    marca.AsigCedula("###MARCA_COLA###");
    o.Insertar(marca);

    while (o.Remover(persona)) {
        if (persona.ObtCedula() == "###MARCA_COLA###") break;
        destino.Insertar(persona);
        aux.Insertar(persona);
    }

    while (aux.Remover(persona)) o.Insertar(persona);
}

Salas::Salas() {}

nodo<Sala> *Salas::buscarNodo(int nSala) {
    nodo<Sala> *p = listaSalas.ObtPrimero();

    while (p != NULL) {
        if (p->ObtInfo().nSala == nSala) return p;
        p = p->ObtDer();
    }

    return NULL;
}

bool Salas::insertarAlFinal(const Sala &sala) {
    if (listaSalas.Vacia()) return listaSalas.InsComienzo(sala);

    nodo<Sala> *p = listaSalas.ObtPrimero();
    while (p->ObtDer() != NULL) p = p->ObtDer();

    return listaSalas.InsDerecho(p, sala);
}

bool Salas::AgregarSala(int nSala, const std::string &pelicula) {
    if (Llena()) return false;
    if (SalaExiste(nSala)) return false;

    return insertarAlFinal(Sala(nSala, pelicula));
}

bool Salas::SalaExiste(int nSala) {
    return buscarNodo(nSala) != NULL;
}

bool Salas::CambiarPelicula(int nSala, const std::string &nuevaPelicula) {
    nodo<Sala> *p = buscarNodo(nSala);
    if (p == NULL) return false;

    p->ObtInfo().pelicula = nuevaPelicula;
    return true;
}

bool Salas::ObtenerPelicula(int nSala, std::string &pelicula) {
    nodo<Sala> *p = buscarNodo(nSala);
    if (p == NULL) return false;

    pelicula = p->ObtInfo().pelicula;
    return true;
}

bool Salas::EncolarCliente(int nSala, const Persona &cliente) {
    nodo<Sala> *p = buscarNodo(nSala);
    if (p == NULL) return false;

    return p->ObtInfo().colaSala.Insertar(cliente);
}

bool Salas::AtenderCliente(int nSala, Persona &cliente) {
    nodo<Sala> *p = buscarNodo(nSala);
    if (p == NULL) return false;

    return p->ObtInfo().colaSala.Remover(cliente);
}

bool Salas::ObtenerCola(int nSala, Cola<Persona> *&cola) {
    nodo<Sala> *p = buscarNodo(nSala);
    if (p == NULL) {
        cola = NULL;
        return false;
    }

    cola = &(p->ObtInfo().colaSala);
    return true;
}

bool Salas::Vacia() { return listaSalas.Vacia(); }

bool Salas::Llena() { return listaSalas.Total() >= MAX_SALAS; }

int Salas::TotalSalas() { return listaSalas.Total(); }

int Salas::TotalClientes(int nSala) {
    nodo<Sala> *p = buscarNodo(nSala);
    if (p == NULL) return -1;

    return p->ObtInfo().colaSala.Total();
}

void Salas::Mostrar() {
    nodo<Sala> *p = listaSalas.ObtPrimero();

    if (p == NULL) {
        std::cout << "No hay salas registradas." << std::endl;
        return;
    }

    while (p != NULL) {
        const Sala &sala = p->ObtInfo();
        std::cout << "Sala " << sala.nSala
                  << " | Pelicula: " << sala.pelicula
                  << " | Clientes en cola: " << sala.colaSala.Total()
                  << std::endl;
        p = p->ObtDer();
    }
}
