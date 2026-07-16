#ifndef TAQUILLA_H
#define TAQUILLA_H

#include <iostream>
#include <string>
#include "../cola/Cola.h"
#include "../persona/Persona.h"

using namespace std;

class Taquilla {
private:
    string nombreTaquilla;   
    float precioBoleto;
    bool abierta;
    Cola<Persona> filaClientes;
    static const int MAX_PERSONAS = 10;

public:
    // Constructor y Destructor
    Taquilla(string nombre = "Sin Nombre", float precio = 0.0);
    ~Taquilla();

    // Getters y Setters (Con const solo en retornos primitivos seguros)
    string getNombreTaquilla() const;
    void setNombreTaquilla(string nombre);
    float getPrecioBoleto() const;
    void setPrecioBoleto(float precio);
    bool estaAbierta() const;
    void setAbierta(bool estado);
    int cantidadClientesEnEspera() const;
    
    // Retorna la referencia real de la cola
    Cola<Persona>& getColaTaquilla();

    // Gestión de Clientes
    bool recibirCliente(Persona p);
    bool atenderSiguienteCliente(Persona &pAtendida);
    
    // Visualización y Búsqueda
    void mostrarFilaTaquilla();
    bool buscarClientePorCedula(string cedula);
};

#endif