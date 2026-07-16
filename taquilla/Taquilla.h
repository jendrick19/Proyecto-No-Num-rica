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
    int limitePersonas;

public:
    // Constructor y Destructor
    Taquilla(string nombre = "Sin Nombre", float precio = 0.0, int limite = 10);
    ~Taquilla();

    // Getters y Setters (Con const solo en retornos primitivos seguros)
    string getNombreTaquilla() const;
    void setNombreTaquilla(string nombre);
    float getPrecioBoleto() const;
    void setPrecioBoleto(float precio);
    bool estaAbierta() const;
    void setAbierta(bool estado);
    int cantidadClientesEnEspera() const;
    int getLimitePersonas() const;
    void setLimitePersonas(int limite);
    
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