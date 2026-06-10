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
    // Constructores
    Persona();
    Persona(string c, string n, string p, int s);
    ~Persona();

    // Getters
    string getCedula();
    string getNombre();
    string getPelicula();
    int getSala();
    int getAsientoAsignado();

    // Setters
    void setCedula(string c);
    void setNombre(string n);
    void setPelicula(string p);
    void setSala(int s);
    void setAsientoAsignado(int a);
    void registrarDatos();
    void registrarEstatus(string estatus);
    string obtenerEstatusActual();
    void mostrarHistorialEstatus(); 
    void mostrarDatos();
};

#endif
