#ifndef CINE_H
#define CINE_H

#include <string>
#include "../Lista/Lista.h"
#include "../taquilla/Taquilla.h"
#include "../salas/Salas.h"

class Cine {
private:
    std::string nombreCine;
    int idCine;
    
    Lista<Taquilla> listaTaquillas;
    Lista<Sala> listaSalas;

public:
    Cine();
    Cine(int id, std::string nombre);
    ~Cine();

    // Métodos Getters y Setters básicos
    std::string ObtNombre();
    void AsigNombre(std::string nombre);
    
    int ObtId();
    void AsigId(int id);

    // Métodos para gestionar las estructuras internas
    // Se valida el límite máximo de 3 Taquillas por Cine
    bool AgregarTaquilla(Taquilla nuevaTaquilla);
    Lista<Taquilla>& ObtListaTaquillas();

    // Se valida el límite máximo de 3 Salas por Cine
    bool AgregarSala(Sala nuevaSala);
    Lista<Sala>& ObtListaSalas();
};

#endif