#include "Persona.h"
#include <iostream>

Persona::Persona() : cedula(""), nombre(""), pelicula(""), sala(0) {}

Persona::Persona(const std::string &ced, const std::string &nom, const std::string &pel, int s)
    : cedula(ced), nombre(nom), pelicula(pel), sala(s) {}

Persona::Persona(const Persona &otra) {
    cedula = otra.cedula;
    nombre = otra.nombre;
    pelicula = otra.pelicula;
    sala = otra.sala;

    // Copia profunda de la pila de estatus
    Pila<std::string> &o = const_cast<Pila<std::string> &>(otra.pilaEstatus);
    Pila<std::string> aux;
    std::string est;
    while (o.Remover(est)) {
        aux.Insertar(est);
    }
    while (aux.Remover(est)) {
        o.Insertar(est);
        pilaEstatus.Insertar(est);
    }
}

Persona &Persona::operator=(const Persona &otra) {
    if (this == &otra) return *this;

    cedula = otra.cedula;
    nombre = otra.nombre;
    pelicula = otra.pelicula;
    sala = otra.sala;

    // Vaciar pila actual
    std::string aux_est;
    while (pilaEstatus.Remover(aux_est));

    // Copia profunda de la nueva pila
    Pila<std::string> &o = const_cast<Pila<std::string> &>(otra.pilaEstatus);
    Pila<std::string> aux;
    while (o.Remover(aux_est)) {
        aux.Insertar(aux_est);
    }
    while (aux.Remover(aux_est)) {
        o.Insertar(aux_est);
        pilaEstatus.Insertar(aux_est);
    }

    return *this;
}

Persona::~Persona() {}

void Persona::AsigCedula(const std::string &ced) { cedula = ced; }
std::string Persona::ObtCedula() const { return cedula; }

void Persona::AsigNombre(const std::string &nom) { nombre = nom; }
std::string Persona::ObtNombre() const { return nombre; }

void Persona::AsigPelicula(const std::string &pel) { pelicula = pel; }
std::string Persona::ObtPelicula() const { return pelicula; }

void Persona::AsigSala(int s) { sala = s; }
int Persona::ObtSala() const { return sala; }

void Persona::AgregarEstatus(const std::string &estatus) {
    pilaEstatus.Insertar(estatus);
}

bool Persona::PopEstatus(std::string &estatus) {
    return pilaEstatus.Remover(estatus);
}

int Persona::TotalEstatus() {
    return pilaEstatus.Total();
}

void Persona::MostrarEstatus() const {
    Pila<std::string> &p = const_cast<Pila<std::string> &>(pilaEstatus);
    if (p.Vacia()) {
        std::cout << "(sin historial)" << std::endl;
        return;
    }

    Pila<std::string> aux;
    std::string est;
    while (p.Remover(est)) {
        std::cout << "[" << est << "] ";
        aux.Insertar(est);
    }
    while (aux.Remover(est)) {
        p.Insertar(est);
    }
}
