#ifndef SALAS_H
#define SALAS_H

#include <iostream>
#include <string>
#include "../Lista/Lista.h"
#include "../cola/Cola.h"
#include "../persona/Persona.h"

struct Sala {
    int nSala;
    std::string pelicula;
    Cola<Persona> colaSala;

    Sala();
    Sala(int numero, const std::string &pel);
    Sala(const Sala &otra);
    Sala &operator=(const Sala &otra);

private:
    static void clonarCola(const Cola<Persona> &origen, Cola<Persona> &destino);
};

class Salas {
    static const int MAX_SALAS = 3;
    Lista<Sala> listaSalas;

    nodo<Sala> *buscarNodo(int nSala);
    bool insertarAlFinal(const Sala &sala);

public:
    Salas();

    bool AgregarSala(int nSala, const std::string &pelicula);
    bool SalaExiste(int nSala);
    bool CambiarPelicula(int nSala, const std::string &nuevaPelicula);
    bool ObtenerPelicula(int nSala, std::string &pelicula);

    bool EncolarCliente(int nSala, const Persona &cliente);
    bool AtenderCliente(int nSala, Persona &cliente);
    bool ObtenerCola(int nSala, Cola<Persona> *&cola);

    bool Vacia();
    bool Llena();
    int TotalSalas();
    int TotalClientes(int nSala);

    void Mostrar();
};

#endif
