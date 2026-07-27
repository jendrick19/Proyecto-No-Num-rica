// =====================================================================
//  VERSION 2: LA SIMULACION AVANZA DE A UN CLIENTE Y UN PASO
//
//  Cada vez que se pulsa "Avanzar simulacion" se mueve UNA sola persona
//  UNA sola etapa. Se atiende primero la etapa mas avanzada, de modo que
//  un cliente completa todo su recorrido (taquilla -> sala -> funcion ->
//  salida) antes de que empiece el siguiente. Sirve para narrar la
//  dramatizacion persona por persona.
// =====================================================================

#include <iostream>
#include "core/Simulacion.h"

using namespace std;

// Resumen de una linea para saber donde esta parada la gente sin tener que
// abrir el resumen completo.
static void mostrarEstadoCompacto(Sede &sede) {
    int enTaquillas = 0;
    Lista<Taquilla> &taquillas = sede.cine.ObtListaTaquillas();
    nodo<Taquilla> *p = taquillas.ObtPrimero();
    while (p != NULL) {
        enTaquillas += p->ObtInfo().cantidadClientesEnEspera();
        p = p->ObtDer();
    }

    int enPuertaSalas = 0;
    int totalSalas = sede.salas.TotalSalas();
    for (int i = 0; i < totalSalas; ++i) {
        int nSala;
        string pelicula;
        if (sede.salas.ObtenerSalaPorIndice(i, nSala, pelicula)) {
            enPuertaSalas += sede.salas.TotalClientes(nSala);
        }
    }

    cout << "-----------------------------------------" << endl;
    cout << "Lobby: " << sede.esperaGeneral.Total()
         << " | Taquillas: " << enTaquillas
         << " | Puerta de salas: " << enPuertaSalas
         << " | Viendo funcion: " << sede.enFuncion.Total() << endl;
    cout << "-----------------------------------------" << endl;
}

int main() {
    Sede sedes[MAX_CINES];
    int numSedes = 0;
    int sedeActiva = -1;

    precargarDatos(sedes, numSedes, sedeActiva);

    int opcion = 0;
    while (opcion != 8) {
        limpiarPantalla();
        cout << "=========================================" << endl;
        cout << " SISTEMA DE GESTION DE CINE (PASO A PASO)" << endl;
        cout << "=========================================" << endl;

        if (sedeActiva >= 0) {
            cout << "CINE ACTIVO: " << sedes[sedeActiva].cine.ObtNombre()
                 << " (ID: " << sedes[sedeActiva].cine.ObtId() << ")" << endl;
            mostrarEstadoCompacto(sedes[sedeActiva]);
        } else {
            cout << "CINE ACTIVO: Ninguno" << endl;
            cout << "-----------------------------------------" << endl;
        }

        cout << "1) Gestionar cines" << endl;
        cout << "2) Gestionar taquillas" << endl;
        cout << "3) Gestionar salas" << endl;
        cout << "4) Llega un cliente al cine" << endl;
        cout << "5) Avanzar simulacion (1 paso)" << endl;
        cout << "6) Ver resumen de colas" << endl;
        cout << "7) Ver cines registrados" << endl;
        cout << "8) Salir" << endl;
        cout << "=========================================" << endl;

        opcion = leerEntero("Opcion: ");
        if (opcion == -1) break; // entrada cerrada

        bool necesitaCine = (opcion >= 2 && opcion <= 6);
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
            case 5: {
                limpiarPantalla();
                cout << "--- AVANZANDO UN PASO ---\n" << endl;
                if (!avanzarUnPaso(sedes[sedeActiva])) {
                    cout << "No hay nada que avanzar: no queda ningun cliente "
                         << "dentro del cine." << endl;
                    cout << "Use la opcion 4 para que llegue alguien." << endl;
                } else {
                    cout << endl;
                    mostrarEstadoCompacto(sedes[sedeActiva]);
                }
                pausar();
                break;
            }
            case 6:
                limpiarPantalla();
                mostrarResumen(sedes[sedeActiva]);
                pausar();
                break;
            case 7:
                limpiarPantalla();
                mostrarCinesRegistrados(sedes, numSedes);
                pausar();
                break;
            case 8:
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
