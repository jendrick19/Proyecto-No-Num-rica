#ifndef PERSONA_H
#define PERSONA_H

#include <iostream>
#include <string>
#include "../pila/Pila.h"

using namespace std;

class Persona {
private:
    string cedula;
    string nombre;
    string pelicula;
    int sala;
    int asientoAsignado;
    Pila<string> pilaEstatus;

public:
    Persona();
    Persona(string c, string n, string p, int s);
    Persona(const Persona &otra);
    Persona &operator=(const Persona &otra);
    ~Persona();

    string getCedula();
    string getNombre();
    string getPelicula();
    int getSala();
    int getAsientoAsignado();

    void setCedula(string c);
    void setNombre(string n);
    void setPelicula(string p);
    void setSala(int s);
    void setAsientoAsignado(int a);

    void AsigCedula(const string &c);
    string ObtCedula() const;
    void AsigNombre(const string &n);
    string ObtNombre() const;
    void AsigPelicula(const string &p);
    string ObtPelicula() const;
    void AsigSala(int s);
    int ObtSala() const;

    void registrarDatos();
    void registrarEstatus(string estatus);
    void AgregarEstatus(const string &estatus);
    string obtenerEstatusActual();
    string ObtenerEstatusActual() const;
    void mostrarHistorialEstatus();
    void mostrarDatos();
};

#endif
