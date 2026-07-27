#include "Cine.h"

Cine::Cine() {
    idCine = 0;
    nombreCine = "Sin Nombre";
    precioDelDia = -1.0;
}

Cine::Cine(int id, std::string nombre) {
    idCine = id;
    nombreCine = nombre;
    precioDelDia = -1.0;
}

Cine::~Cine() {
}

std::string Cine::ObtNombre() {
    return nombreCine;
}

void Cine::AsigNombre(std::string nombre) {
    nombreCine = nombre;
}

int Cine::ObtId() {
    return idCine;
}

void Cine::AsigId(int id) {
    idCine = id;
}

float Cine::ObtPrecioDelDia() {
    return precioDelDia;
}

void Cine::AsigPrecioDelDia(float precio) {
    precioDelDia = (precio >= 0.0) ? precio : 0.0;
}

bool Cine::AgregarTaquilla(Taquilla nuevaTaquilla) {
    if (listaTaquillas.Total() >= 3) return false;

    // Se inserta al final para que el orden del listado coincida con el orden
    // en que se crearon las taquillas (InsComienzo las mostraba al reves).
    if (listaTaquillas.Vacia()) {
        return listaTaquillas.InsComienzo(nuevaTaquilla);
    }

    nodo<Taquilla> *p = listaTaquillas.ObtPrimero();
    while (p->ObtDer() != NULL) p = p->ObtDer();

    return listaTaquillas.InsDerecho(p, nuevaTaquilla);
}

Lista<Taquilla>& Cine::ObtListaTaquillas() {
    return listaTaquillas;
}

bool Cine::AgregarSala(Sala nuevaSala) {
    if (listaSalas.Total() < 3) {
        return listaSalas.InsComienzo(nuevaSala);
    }
    return false;
}

Lista<Sala>& Cine::ObtListaSalas() {
    return listaSalas;
}