#include <iostream>
#include <string>
#include "persona/Persona.h"

using namespace std;

int main() {
    int opcion = 0;
    Persona* cliente = NULL; 

    while (opcion != 4) {
        cout << "\n=========================================";
        cout << "\n           MODULO PERSONA    ";
        cout << "\n=========================================";
        cout << "\n1) Crear Cliente y Registrar Datos Iniciales";
        cout << "\n2) Simular Integracion: Asignar Sala y Pelicula (Setters)";
        cout << "\n3) Consultar Estado Actual e Historial de Auditoria (Pila)";
        cout << "\n4) Salir del Banco de Pruebas y Liberar Memoria";
        cout << "\n=========================================";
        cout << "\nSeleccione una opcion: ";
        cin >> opcion;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Entrada invalida. Intente de nuevo." << endl;
            continue;
        }

        switch (opcion) {
            case 1: {
                if (cliente != NULL) {
                    delete cliente;
                    cliente = NULL;
                }

               
                cliente = new Persona();
                
                cliente->registrarDatos(); 
                
                cout << "\n[+] Objeto Persona creado exitosamente en memoria." << endl;
                break;
            }

            case 2: {
                if (cliente == NULL) {
                    cout << "\n[-] Error: Primero debe crear un cliente en la opcion 1." << endl;
                    break;
                }

                string peliculaSeleccionada;
                int numeroSalaReal, numeroAsientoCalculado;

                cout << "\n--- SIMULACION DE ENCHUFES (INTEGRACION CON OTROS MODULOS) ---" << endl;
                cout << "Ingrese Pelicula (Dato que enviaria Taquilla): ";
                getline(cin >> ws, peliculaSeleccionada);

                cout << "Ingrese Sala 1-3 (Dato que validaria el Modulo Salas): ";
                cin >> numeroSalaReal;

                cout << "Ingrese Numero de Asiento (Asignado por el Modulo Salas): ";
                cin >> numeroAsientoCalculado;

                
                cliente->setPelicula(peliculaSeleccionada);
                cliente->setSala(numeroSalaReal);
                cliente->setAsientoAsignado(numeroAsientoCalculado);
                
                cliente->registrarEstatus("Boleto comprado. Asignado a Sala " + to_string(numeroSalaReal));

                cout << "\n[+] Datos simulados correctamente. Modulo conectado." << endl;
                break;
            }

            case 3: {
                if (cliente == NULL) {
                    cout << "\n[-] Error: No hay ningun cliente registrado para consultar." << endl;
                    break;
                }

                
                cout << "\n>>> CONSULTA DE ESTADO ACTUAL <<<" << endl;
                cliente->mostrarDatos();

                cout << "\n>>> AUDITORIA CRONOLOGICA DE TRANSICIONES <<<" << endl;
                cliente->mostrarHistorialEstatus();
                break;
            }

            case 4: {
                cout << "\nFinalizando el banco de pruebas..." << endl;
                if (cliente != NULL) {
                    cout << "Liberando memoria dinamica del objeto cliente..." << endl;
                    delete cliente; 
                    cliente = NULL;
                }
                cout << "[!] Memoria limpia. Prueba concluida con exito." << endl;
                break;
            }

            default:
                cout << "Opcion no valida. Seleccione un numero entre 1 y 4." << endl;
                break;
        }
    }

    return 0;
}