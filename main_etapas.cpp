// =====================================================================
//  VERSION 1: LA SIMULACION AVANZA POR ETAPAS SEPARADAS
//
//  Cada etapa del recorrido del cliente es una opcion distinta del menu,
//  asi entre una y otra se puede consultar el resumen y ver donde quedo
//  parada la gente: en el lobby, en la fila de taquilla, en la puerta de
//  la sala o dentro de la funcion.
// =====================================================================

#include <iostream>
#include "core/Simulacion.h"

using namespace std;

// Atiende la fila completa de una taquilla: todos compran su boleto y pasan
// a la fila de la sala que les corresponda segun su pelicula.
static void etapaAtenderTaquilla(Sede &sede) {
    if (sede.cine.ObtListaTaquillas().Total() == 0) {
        cout << "Error: no hay taquillas registradas." << endl;
        return;
    }

    mostrarTaquillas(sede.cine);
    int seleccion = leerEntero("Que taquilla desea atender: ");

    Taquilla *taquilla = obtenerTaquillaPorIndice(sede.cine, seleccion - 1);
    if (taquilla == NULL) {
        cout << "Esa taquilla no existe." << endl;
        return;
    }

    if (taquilla->getColaTaquilla().Vacia()) {
        cout << taquilla->getNombreTaquilla() << " no tiene gente en la fila." << endl;
        return;
    }

    cout << "\n--- VENDIENDO BOLETOS EN " << taquilla->getNombreTaquilla() << " ---" << endl;

    int atendidos = 0;
    while (atenderUnCliente(sede, seleccion - 1)) {
        ++atendidos;
    }

    cout << "\nSe atendieron " << atendidos << " clientes." << endl;
    cout << "Ahora estan esperando en la puerta de su sala." << endl;
}

// Abre las puertas de una sala: todos los que compraron boleto para esa
// pelicula entran a verla.
static void etapaIniciarFuncion(Sede &sede) {
    if (sede.salas.TotalSalas() == 0) {
        cout << "Error: no hay salas registradas." << endl;
        return;
    }

    sede.salas.Mostrar();
    int nSala = leerEntero("Que sala inicia funcion: ");

    if (!sede.salas.SalaExiste(nSala)) {
        cout << "Esa sala no existe." << endl;
        return;
    }

    if (sede.salas.TotalClientes(nSala) == 0) {
        cout << "No hay nadie esperando en la puerta de la Sala " << nSala << "." << endl;
        return;
    }

    cout << "\n--- ABRIENDO PUERTAS DE LA SALA " << nSala << " ---" << endl;

    int entraron = 0;
    while (ingresarUnCliente(sede, nSala)) {
        ++entraron;
    }

    cout << "\nEntraron " << entraron << " personas a la Sala " << nSala << "." << endl;
}

// Termina la pelicula: todos los que estaban viendo la funcion salen del cine
// y se imprime el historial completo que quedo en su pila de estatus.
static void etapaFinalizarFuncion(Sede &sede) {
    if (sede.enFuncion.Vacia()) {
        cout << "No hay nadie viendo una funcion en este momento." << endl;
        return;
    }

    cout << "\n--- FIN DE LA FUNCION ---" << endl;

    int salieron = 0;
    while (salirUnCliente(sede)) {
        ++salieron;
    }

    cout << "\nSalieron " << salieron << " personas del cine." << endl;
}

int main() {
    Sede sedes[MAX_CINES];
    int numSedes = 0;
    int sedeActiva = -1;

    precargarDatos(sedes, numSedes, sedeActiva);

    int opcion = 0;
    while (opcion != 10) {
        limpiarPantalla();
        cout << "=========================================" << endl;
        cout << "   SISTEMA DE GESTION DE CINE (ETAPAS)   " << endl;
        cout << "=========================================" << endl;

        if (sedeActiva >= 0) {
            cout << "CINE ACTIVO: " << sedes[sedeActiva].cine.ObtNombre()
                 << " (ID: " << sedes[sedeActiva].cine.ObtId() << ")" << endl;
        } else {
            cout << "CINE ACTIVO: Ninguno" << endl;
        }

        cout << "-----------------------------------------" << endl;
        cout << " CONFIGURACION" << endl;
        cout << "  1) Gestionar cines" << endl;
        cout << "  2) Gestionar taquillas" << endl;
        cout << "  3) Gestionar salas" << endl;
        cout << " RECORRIDO DEL CLIENTE" << endl;
        cout << "  4) Llega un cliente al cine" << endl;
        cout << "  5) Atender taquilla      (compran boleto -> fila de sala)" << endl;
        cout << "  6) Iniciar funcion       (fila de sala -> viendo pelicula)" << endl;
        cout << "  7) Finalizar funcion     (viendo pelicula -> salen del cine)" << endl;
        cout << " CONSULTAS" << endl;
        cout << "  8) Ver resumen de colas" << endl;
        cout << "  9) Ver cines registrados" << endl;
        cout << " 10) Salir" << endl;
        cout << "=========================================" << endl;

        opcion = leerEntero("Opcion: ");
        if (opcion == -1) break; // entrada cerrada

        bool necesitaCine = (opcion >= 2 && opcion <= 8);
        if (necesitaCine && sedeActiva < 0) {
            limpiarPantalla();
            cout << "Error: seleccione un cine activo primero." << endl;
            pausar();
            continue;
        }

        switch (opcion) {
            case 1:
                menuGestionCines(sedes, numSedes, sedeActiva);
                pausar();
                break;
            case 2:
                menuGestionTaquillas(sedes[sedeActiva]);
                pausar();
                break;
            case 3:
                menuGestionSalas(sedes[sedeActiva]);
                pausar();
                break;
            case 4:
                limpiarPantalla();
                cout << "--- LLEGA UN CLIENTE ---" << endl;
                registrarCliente(sedes[sedeActiva]);
                pausar();
                break;
            case 5:
                limpiarPantalla();
                etapaAtenderTaquilla(sedes[sedeActiva]);
                pausar();
                break;
            case 6:
                limpiarPantalla();
                etapaIniciarFuncion(sedes[sedeActiva]);
                pausar();
                break;
            case 7:
                limpiarPantalla();
                etapaFinalizarFuncion(sedes[sedeActiva]);
                pausar();
                break;
            case 8:
                limpiarPantalla();
                mostrarResumen(sedes[sedeActiva]);
                pausar();
                break;
            case 9:
                limpiarPantalla();
                mostrarCinesRegistrados(sedes, numSedes);
                pausar();
                break;
            case 10:
                cout << "Saliendo del programa..." << endl;
                break;
            default:
                limpiarPantalla();
                cout << "Opcion invalida." << endl;
                pausar();
                break;
        }
    }

    return 0;
}
