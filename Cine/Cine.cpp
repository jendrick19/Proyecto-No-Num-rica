#include "Cine.h"

Cine::Cine() {
    idCine = 0;
    nombreCine = "Sin Nombre";
}

Cine::Cine(int id, std::string nombre) {
    idCine = id;
    nombreCine = nombre;
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

bool Cine::AgregarTaquilla(Taquilla nuevaTaquilla) {
    if (listaTaquillas.Total() < 3) {
        return listaTaquillas.InsComienzo(nuevaTaquilla);
    }
    return false; 
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