#ifndef SIMULACION_H
#define SIMULACION_H

#include <string>
#include "../Cine/Cine.h"
#include "../salas/Salas.h"
#include "../taquilla/Taquilla.h"
#include "../persona/Persona.h"
#include "../cola/Cola.h"

const int MAX_CINES = 3;

// Una sede agrupa todo lo que pertenece a un mismo cine.
// Las colas de taquilla viven dentro de cada Taquilla y las colas de sala
// dentro de cada Sala; aqui solo quedan las dos que no tienen dueno natural.
struct Sede {
    Cine cine;
    Salas salas;
    Cola<Persona> esperaGeneral; // llegaron y no cupieron en ninguna taquilla
    Cola<Persona> enFuncion;     // ya entraron a la sala, estan viendo la pelicula
};

// --- Pantalla y entrada de datos ---
void limpiarPantalla();
void pausar();
std::string leerTexto(const std::string &mensaje);
int leerEntero(const std::string &mensaje);
float leerPrecio(const std::string &mensaje);

// --- Consultas sobre las estructuras ---
Taquilla *obtenerTaquillaPorIndice(Cine &cine, int indice);
int elegirTaquillaConCupo(Cine &cine);
int resolverSalaParaPelicula(Salas &salas, const std::string &pelicula);

// --- Visualizacion ---
void mostrarColaClientes(Cola<Persona> &cola, const std::string &titulo);
void mostrarCartelera(Salas &salas);
void mostrarTaquillas(Cine &cine);
void mostrarResumen(Sede &sede);

// --- Etapas del recorrido del cliente ---
// 1. Llega al cine, pide una pelicula y se pone en la fila de una taquilla.
bool registrarCliente(Sede &sede);
// 2. Compra el boleto: la taquilla le INDICA la sala y pasa a la fila de esa sala.
bool atenderUnCliente(Sede &sede, int indiceTaquilla);
// 3. Entra a la sala: deja la fila de la sala y pasa a ver la funcion.
bool ingresarUnCliente(Sede &sede, int nSala);
bool iniciarFuncionSala(Sede &sede, int nSala);
// 4. Termina la funcion y abandona el cine (aqui se muestra su historial).
bool salirUnCliente(Sede &sede);

// Mueve un unico cliente por la etapa mas avanzada que tenga trabajo pendiente.
bool avanzarUnPaso(Sede &sede);

// Reubica gente de la fila general apenas se libera cupo en alguna taquilla.
void repartirEsperaGeneral(Sede &sede);

// --- Menus de configuracion (identicos en ambas versiones) ---
void menuGestionCines(Sede sedes[], int &numSedes, int &sedeActiva);
void menuGestionTaquillas(Sede &sede);
void menuGestionSalas(Sede &sede);
void mostrarCinesRegistrados(Sede sedes[], int numSedes);

void precargarDatos(Sede sedes[], int &numSedes, int &sedeActiva);

#endif
