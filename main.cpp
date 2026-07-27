// =====================================================================
//  VERSION DE SIMULACION PASO A PASO (NUEVO MAIN PRINCIPAL)
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
    while (opcion != 11) {
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
        cout << "5) Avanzar colas de taquilla/lobby (1 paso)" << endl;
        cout << "6) Iniciar pelicula (Sala -> Funcion)" << endl;
        cout << "7) Salida de clientes (Funcion -> Terminar)" << endl;
        cout << "8) Procesar personas en Lobby" << endl;
        cout << "9) Ver resumen de colas" << endl;
        cout << "10) Ver cines registrados" << endl;
        cout << "11) Salir" << endl;
        cout << "=========================================" << endl;

        opcion = leerEntero("Opcion: ");
        if (opcion == -1) break; // entrada cerrada

        bool necesitaCine = (opcion >= 2 && opcion <= 9);
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
                cout << "--- AVANZANDO UN PASO (TAQUILLAS/LOBBY) ---\n" << endl;
                if (!avanzarUnPaso(sedes[sedeActiva])) {
                    cout << "No hay clientes en las colas de taquillas o en el lobby." << endl;
                } else {
                    cout << endl;
                    mostrarEstadoCompacto(sedes[sedeActiva]);
                }
                pausar();
                break;
            }
            case 6: {
                limpiarPantalla();
                cout << "--- INICIAR PELICULA ---" << endl;
                sedes[sedeActiva].salas.Mostrar();
                int nSala = leerEntero("\nIngrese el numero de sala para iniciar la pelicula: ");
                if (!iniciarFuncionSala(sedes[sedeActiva], nSala)) {
                    cout << "No hay clientes en la cola de la Sala " << nSala << " o la sala no existe." << endl;
                } else {
                    cout << endl;
                    mostrarEstadoCompacto(sedes[sedeActiva]);
                }
                pausar();
                break;
            }
            case 7: {
                limpiarPantalla();
                cout << "--- SALIDA DE CLIENTES ---\n" << endl;
                if (!salirUnCliente(sedes[sedeActiva])) {
                    cout << "No hay clientes viendo pelicula actualmente." << endl;
                } else {
                    cout << endl;
                    mostrarEstadoCompacto(sedes[sedeActiva]);
                }
                pausar();
                break;
            }
            case 8: {
                limpiarPantalla();
                cout << "--- PROCESANDO LOBBY ---\n" << endl;
                int inicial = sedes[sedeActiva].esperaGeneral.Total();
                repartirEsperaGeneral(sedes[sedeActiva]);
                int final = sedes[sedeActiva].esperaGeneral.Total();
                cout << "Se asignaron " << (inicial - final) << " personas a las taquillas." << endl;
                pausar();
                break;
            }
            case 9:
                limpiarPantalla();
                mostrarResumen(sedes[sedeActiva]);
                pausar();
                break;
            case 10:
                limpiarPantalla();
                mostrarCinesRegistrados(sedes, numSedes);
                pausar();
                break;
            case 11:
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