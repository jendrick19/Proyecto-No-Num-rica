#ifndef PERSONA_H
#define PERSONA_H

#include <string>
#include "../pila/Pila.h"

class Persona {
    std::string cedula;
    std::string nombre;
    std::string pelicula;
    int sala;
    Pila<std::string> pilaEstatus;

public:
    Persona();
    Persona(const std::string &ced, const std::string &nom, const std::string &pel, int s);
    Persona(const Persona &otra);
    Persona &operator=(const Persona &otra);
    ~Persona();

    void AsigCedula(const std::string &ced);
    std::string ObtCedula() const;

    void AsigNombre(const std::string &nom);
    std::string ObtNombre() const;

    void AsigPelicula(const std::string &pel);
    std::string ObtPelicula() const;

    void AsigSala(int s);
    int ObtSala() const;

    void AgregarEstatus(const std::string &estatus);
    bool PopEstatus(std::string &estatus);
    int TotalEstatus();
    void MostrarEstatus() const;
};

#endif
