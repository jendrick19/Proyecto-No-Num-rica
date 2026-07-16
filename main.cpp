#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include "Cine/Cine.h"
#include "salas/Salas.h"
#include "taquilla/Taquilla.h"
#include "persona/Persona.h"

using namespace std;

void limpiarEntrada() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string leerTexto(const string &mensaje) {
    cout << mensaje;
    string valor;
    getline(cin >> ws, valor);
    return valor;
}

int leerEntero(const string &mensaje) {
    while (true) {
        cout << mensaje;
        string entrada;
        getline(cin >> ws, entrada);
        stringstream ss(entrada);
        int valor;
        if (ss >> valor && ss.eof()) {
            return valor;
        }
        cout << "Error: solo se aceptan numeros enteros." << endl;
    }
}

float leerPrecio(const string &mensaje) {
    while (true) {
        cout << mensaje;
        string entrada;
        getline(cin >> ws, entrada);
        stringstream ss(entrada);
        float valor;
        if (ss >> valor && ss.eof() && valor >= 0) {
            return valor;
        }
        cout << "Error: el precio debe ser un numero mayor o igual a 0." << endl;
    }
}

void mostrarColaClientes(Cola<Persona> &cola, const string &titulo) {
    cout << titulo << endl;
    if (cola.Vacia()) {
        cout << "  (vacia)" << endl;
        return;
    }

    Cola<Persona> copia;
    Persona persona;
    while (cola.Remover(persona)) {
        cout << "  - " << persona.getNombre() << " (" << persona.getCedula() << ")"
             << " | " << persona.ObtenerEstatusActual() << endl;
        copia.Insertar(persona);
    }

    while (copia.Remover(persona)) {
        cola.Insertar(persona);
    }
}

void mostrarResumen(Cine &cine, Salas &salas, Cola<Persona> &espera) {
    cout << "\n=== RESUMEN DEL CINE ACTIVO ===" << endl;
    cout << "Cine: " << cine.ObtNombre() << " (ID " << cine.ObtId() << ")" << endl;

    cout << "Clientes en espera general: " << espera.Total() << endl;
    mostrarColaClientes(espera, "Fila general:");

    cout << "Taquillas registradas: " << cine.ObtListaTaquillas().Total() << endl;
    Lista<Taquilla> &taquillas = cine.ObtListaTaquillas();
    nodo<Taquilla> *pTaquilla = taquillas.ObtPrimero();
    int idx = 1;
    while (pTaquilla != NULL) {
        Taquilla &t = pTaquilla->ObtInfo();
        cout << " - " << idx << ") " << t.getNombreTaquilla()
             << " | Precio: $" << t.getPrecioBoleto()
             << " | En espera: " << t.cantidadClientesEnEspera() << "/" << t.getLimitePersonas() << endl;
        if (t.cantidadClientesEnEspera() > 0) {
            mostrarColaClientes(t.getColaTaquilla(), "   Fila de " + t.getNombreTaquilla() + ":");
        }
        pTaquilla = pTaquilla->ObtDer();
        ++idx;
    }

    cout << "Salas registradas: " << salas.TotalSalas() << endl;
    salas.Mostrar();
}

Taquilla* obtenerTaquillaPorIndice(Cine &cine, int indice) {
    Lista<Taquilla> &taquillas = cine.ObtListaTaquillas();
    nodo<Taquilla> *p = taquillas.ObtPrimero();
    int actual = 0;
    while (p != NULL) {
        if (actual == indice) {
            return &p->ObtInfo();
        }
        p = p->ObtDer();
        ++actual;
    }
    return NULL;
}

void renumerarTaquillas(Cine &cine) {
    Lista<Taquilla> &taquillas = cine.ObtListaTaquillas();
    nodo<Taquilla> *p = taquillas.ObtPrimero();
    int numero = 1;
    while (p != NULL) {
        p->ObtInfo().setNombreTaquilla("Taquilla " + to_string(numero));
        p = p->ObtDer();
        ++numero;
    }
}

bool eliminarTaquilla(Cine &cine, int indice) {
    Lista<Taquilla> &taquillas = cine.ObtListaTaquillas();
    if (indice < 0 || indice >= taquillas.Total()) {
        return false;
    }

    // 1. Find the taquilla to be deleted
    Taquilla *tEliminar = obtenerTaquillaPorIndice(cine, indice);
    if (tEliminar == NULL) {
        return false;
    }

    // 2. If it has people in queue, find the other open taquilla with the fewest people
    if (!tEliminar->getColaTaquilla().Vacia()) {
        Taquilla *target = NULL;
        int minClientes = 999999;
        nodo<Taquilla> *p = taquillas.ObtPrimero();
        int idx = 0;
        while (p != NULL) {
            if (idx != indice) {
                Taquilla &t = p->ObtInfo();
                if (t.estaAbierta()) {
                    int count = t.cantidadClientesEnEspera();
                    if (count < minClientes) {
                        minClientes = count;
                        target = &t;
                    }
                }
            }
            p = p->ObtDer();
            idx++;
        }

        // If we found a target, move them all to the front
        if (target != NULL) {
            Cola<Persona> &qEliminar = tEliminar->getColaTaquilla();
            Cola<Persona> &qTarget = target->getColaTaquilla();

            // Store target's queue in temp
            Cola<Persona> temp;
            Persona pTemp;
            while (qTarget.Remover(pTemp)) {
                temp.Insertar(pTemp);
            }

            // Move eliminated queue to target's queue (they will be at the front)
            int countMovidos = 0;
            while (qEliminar.Remover(pTemp)) {
                pTemp.registrarEstatus("Reubicado a " + target->getNombreTaquilla());
                qTarget.Insertar(pTemp);
                countMovidos++;
            }

            // Move temp back to target's queue
            while (temp.Remover(pTemp)) {
                qTarget.Insertar(pTemp);
            }

            if (countMovidos > 0) {
                cout << "Se reubicaron " << countMovidos << " clientes a " << target->getNombreTaquilla() << " al frente de la cola." << endl;
            }
        } else {
            cout << "Advertencia: No hay otra taquilla abierta disponible para transferir los clientes." << endl;
        }
    }

    // 3. Now proceed with the actual deletion
    if (indice == 0) {
        Taquilla aux;
        if (taquillas.EliComienzo(aux)) {
            renumerarTaquillas(cine);
            return true;
        }
        return false;
    }

    nodo<Taquilla> *anterior = NULL;
    nodo<Taquilla> *actual = taquillas.ObtPrimero();
    int actualIndex = 0;
    while (actual != NULL) {
        if (actualIndex == indice) {
            if (anterior != NULL) {
                anterior->AsigDer(actual->ObtDer());
                delete actual;
                renumerarTaquillas(cine);
                return true;
            }
        }
        anterior = actual;
        actual = actual->ObtDer();
        ++actualIndex;
    }
    return false;
}

bool validarCliente(Cine &cine, Salas &salas, int salaNumero, string &mensajeError) {
    if (cine.ObtId() <= 0) {
        mensajeError = "Error: no existe un cine configurado.";
        return false;
    }

    string peliculaActual;
    if (!salas.ObtenerPelicula(salaNumero, peliculaActual)) {
        mensajeError = "Error: la sala ingresada no existe.";
        return false;
    }

    if (cine.ObtListaTaquillas().Total() == 0) {
        mensajeError = "Error: no hay taquillas configuradas para este cine.";
        return false;
    }

    mensajeError = "";
    return true;
}

int elegirTaquillaMasVacia(Cine &cine) {
    Lista<Taquilla> &taquillas = cine.ObtListaTaquillas();
    nodo<Taquilla> *p = taquillas.ObtPrimero();
    int mejor = -1;
    int menor = 999999;
    int idx = 0;
    while (p != NULL) {
        int ocupadas = p->ObtInfo().cantidadClientesEnEspera();
        if (ocupadas < menor) {
            menor = ocupadas;
            mejor = idx;
        }
        p = p->ObtDer();
        ++idx;
    }
    return mejor;
}

void repartirEspera(Cine &cine, Salas &salas, Cola<Persona> &espera) {
    while (!espera.Vacia()) {
        int idx = elegirTaquillaMasVacia(cine);
        if (idx < 0) {
            break;
        }
        Taquilla *taquilla = obtenerTaquillaPorIndice(cine, idx);
        if (taquilla == NULL) {
            break;
        }
        Persona cliente;
        if (!espera.Remover(cliente)) {
            break;
        }
        if (!taquilla->recibirCliente(cliente)) {
            espera.Insertar(cliente);
            break;
        }
    }
}

bool atenderCliente(Cine &cine, Salas &salas, Cola<Persona> &espera) {
    Lista<Taquilla> &taquillas = cine.ObtListaTaquillas();
    if (taquillas.Total() == 0) {
        cout << "Error: no hay taquillas registradas." << endl;
        return false;
    }

    // List all taquillas with customer count
    cout << "\nTaquillas registradas:" << endl;
    nodo<Taquilla> *p = taquillas.ObtPrimero();
    int idx = 1;
    while (p != NULL) {
        Taquilla &t = p->ObtInfo();
        cout << idx << ") " << t.getNombreTaquilla() << " (Clientes en cola: " << t.cantidadClientesEnEspera() << ")" << endl;
        p = p->ObtDer();
        idx++;
    }

    int seleccion = leerEntero("Seleccione el numero de taquilla a atender: ");
    Taquilla *taquillaElegida = obtenerTaquillaPorIndice(cine, seleccion - 1);
    if (taquillaElegida == NULL) {
        cout << "Error: seleccion invalida." << endl;
        return false;
    }

    if (taquillaElegida->getColaTaquilla().Vacia()) {
        cout << "La taquilla " << taquillaElegida->getNombreTaquilla() << " no tiene clientes en cola." << endl;
        return false;
    }

    int countAtendidos = 0;
    Persona cliente;
    while (taquillaElegida->atenderSiguienteCliente(cliente)) {
        if (cliente.getSala() > 0) {
            if (!salas.EncolarCliente(cliente.getSala(), cliente)) {
                cout << "Error: no fue posible enviar el cliente " << cliente.getNombre() << " a la sala " << cliente.getSala() << "." << endl;
            }
        }
        cout << "Cliente " << cliente.getNombre() << " (V-" << cliente.getCedula() << ") atendido en " 
             << taquillaElegida->getNombreTaquilla() << " para ver la pelicula: " << cliente.getPelicula() << endl;
        countAtendidos++;
    }

    cout << "Se atendieron " << countAtendidos << " clientes en total de " << taquillaElegida->getNombreTaquilla() << "." << endl;
    repartirEspera(cine, salas, espera);
    return true;
}

int main() {
    Cine cines[3];
    Salas salas[3];
    Cola<Persona> esperas[3];
    int numCines = 0;
    int cineActivo = -1;
    int opcion = 0;

    while (opcion != 8) {
        cout << "\n=== MENU INTEGRADO ===" << endl;
        cout << "1) Gestionar cines" << endl;
        cout << "2) Gestionar taquillas" << endl;
        cout << "3) Gestionar salas" << endl;
        cout << "4) Registrar cliente" << endl;
        cout << "5) Atender cliente" << endl;
        cout << "6) Ver resumen" << endl;
        cout << "7) Ver cines registrados" << endl;
        cout << "8) Salir" << endl;
        opcion = leerEntero("Opcion: ");

        switch (opcion) {
            case 1: {
                int sub = 0;
                cout << "\n--- CINES ---" << endl;
                cout << "1) Crear cine" << endl;
                cout << "2) Seleccionar cine activo" << endl;
                cout << "3) Modificar cine activo" << endl;
                cout << "4) Volver" << endl;
                sub = leerEntero("Opcion: ");

                if (sub == 1) {
                    if (numCines >= 3) {
                        cout << "Error: maximo 3 cines." << endl;
                    } else {
                        int id = leerEntero("ID del cine: ");
                        string nombre = leerTexto("Nombre del cine: ");
                        bool existe = false;
                        for (int i = 0; i < numCines; ++i) {
                            if (cines[i].ObtId() == id) {
                                existe = true;
                                break;
                            }
                        }
                        if (existe) {
                            cout << "Error: ya existe un cine con ese ID." << endl;
                        } else {
                            cines[numCines].AsigId(id);
                            cines[numCines].AsigNombre(nombre);
                            ++numCines;
                            cineActivo = numCines - 1;
                            cout << "Cine creado correctamente." << endl;
                        }
                    }
                } else if (sub == 2) {
                    if (numCines == 0) {
                        cout << "No hay cines registrados." << endl;
                    } else {
                        cout << "\nCines disponibles:" << endl;
                        for (int i = 0; i < numCines; ++i) {
                            cout << (i + 1) << ") " << cines[i].ObtNombre() << " (ID " << cines[i].ObtId() << ")" << endl;
                        }
                        int idx = leerEntero("Numero del cine a activar: ");
                        if (idx < 1 || idx > numCines) {
                            cout << "Error: numero de cine invalido." << endl;
                        } else {
                            cineActivo = idx - 1;
                            cout << "Cine activo: " << cines[cineActivo].ObtNombre() << endl;
                        }
                    }
                } else if (sub == 3) {
                    if (cineActivo < 0 || cineActivo >= numCines) {
                        cout << "Error: no hay un cine activo." << endl;
                    } else {
                        int id = leerEntero("Nuevo ID del cine: ");
                        string nombre = leerTexto("Nuevo nombre del cine: ");
                        cines[cineActivo].AsigId(id);
                        cines[cineActivo].AsigNombre(nombre);
                        cout << "Cine modificado correctamente." << endl;
                    }
                }
                break;
            }
            case 2: {
                if (cineActivo < 0 || cineActivo >= numCines) {
                    cout << "Error: debe crear o seleccionar un cine antes de gestionar taquillas." << endl;
                    break;
                }
                int sub = 0;
                cout << "\n--- TAQUILLAS ---" << endl;
                cout << "1) Agregar taquilla" << endl;
                cout << "2) Modificar precio" << endl;
                cout << "3) Eliminar taquilla" << endl;
                cout << "4) Ver taquillas" << endl;
                cout << "5) Establecer precio del dia" << endl;
                cout << "6) Volver" << endl;
                sub = leerEntero("Opcion: ");
                if (sub == 1) {
                    if (cines[cineActivo].ObtPrecioDelDia() < 0) {
                        cout << "Error: no se puede crear la taquilla porque no se ha definido el precio del dia." << endl;
                    } else if (cines[cineActivo].ObtListaTaquillas().Total() >= 3) {
                        cout << "Error: maximo 3 taquillas por cine." << endl;
                    } else {
                        int numero = cines[cineActivo].ObtListaTaquillas().Total() + 1;
                        string nombre = "Taquilla " + to_string(numero);
                        float precio = cines[cineActivo].ObtPrecioDelDia();
                        int limite = leerEntero("Limite de personas para esta taquilla: ");
                        Taquilla taquilla(nombre, precio, limite);
                        if (cines[cineActivo].AgregarTaquilla(taquilla)) {
                            cout << "taquilla " << numero << " creada con precio: " << precio << endl;
                        } else {
                            cout << "No se pudo agregar la taquilla." << endl;
                        }
                    }
                } else if (sub == 2) {
                    float precioDia = cines[cineActivo].ObtPrecioDelDia();
                    if (precioDia < 0) {
                        cout << "Error: no se puede actualizar el precio porque no se ha definido el precio del dia." << endl;
                    } else {
                        Lista<Taquilla> &taquillas = cines[cineActivo].ObtListaTaquillas();
                        nodo<Taquilla> *p = taquillas.ObtPrimero();
                        int actualizados = 0;
                        while (p != NULL) {
                            p->ObtInfo().setPrecioBoleto(precioDia);
                            actualizados++;
                            p = p->ObtDer();
                        }
                        cout << "Se actualizo el precio de " << actualizados << " taquillas al precio del dia: $" << precioDia << endl;
                    }
                } else if (sub == 3) {
                    mostrarResumen(cines[cineActivo], salas[cineActivo], esperas[cineActivo]);
                    int index = leerEntero("Numero de taquilla a eliminar: ");
                    if (eliminarTaquilla(cines[cineActivo], index - 1)) {
                        cout << "Taquilla eliminada." << endl;
                    } else {
                        cout << "Error: no se pudo eliminar la taquilla." << endl;
                    }
                } else if (sub == 4) {
                    mostrarResumen(cines[cineActivo], salas[cineActivo], esperas[cineActivo]);
                } else if (sub == 5) {
                    float precio = leerPrecio("Precio del dia para las taquillas: ");
                    cines[cineActivo].AsigPrecioDelDia(precio);
                    cout << "Precio del dia establecido correctamente." << endl;
                }
                break;
            }
            case 3: {
                if (cineActivo < 0 || cineActivo >= numCines) {
                    cout << "Error: debe crear o seleccionar un cine antes de gestionar salas." << endl;
                    break;
                }
                int sub = 0;
                cout << "\n--- SALAS ---" << endl;
                cout << "1) Agregar sala" << endl;
                cout << "2) Ver salas" << endl;
                cout << "3) Volver" << endl;
                sub = leerEntero("Opcion: ");
                if (sub == 1) {
                    int nSala = leerEntero("Numero de sala: ");
                    string pelicula = leerTexto("Pelicula: ");
                    if (salas[cineActivo].AgregarSala(nSala, pelicula)) {
                        cout << "Sala agregada correctamente." << endl;
                    } else {
                        cout << "No se pudo agregar la sala." << endl;
                    }
                } else if (sub == 2) {
                    salas[cineActivo].Mostrar();
                }
                break;
            }
            case 4: {
                if (cineActivo < 0 || cineActivo >= numCines) {
                    cout << "Error: no existe un cine activo para registrar clientes." << endl;
                    break;
                }
                if (salas[cineActivo].TotalSalas() == 0) {
                    cout << "Error: no hay salas registradas para este cine." << endl;
                    break;
                }
                if (cines[cineActivo].ObtListaTaquillas().Total() == 0) {
                    cout << "Error: no hay taquillas configuradas para este cine." << endl;
                    break;
                }

                string cedula = leerTexto("Cedula: ");
                string nombre = leerTexto("Nombre: ");
                int salaNumero = leerEntero("Numero de sala: ");

                string pelicula;
                if (!salas[cineActivo].ObtenerPelicula(salaNumero, pelicula)) {
                    cout << "Error: la sala ingresada no existe." << endl;
                    break;
                }

                string mensajeError;
                if (!validarCliente(cines[cineActivo], salas[cineActivo], salaNumero, mensajeError)) {
                    cout << mensajeError << endl;
                    break;
                }

                // Check available taquillas (open and not full)
                Lista<Taquilla> &taquillas = cines[cineActivo].ObtListaTaquillas();
                int disponiblesCount = 0;
                nodo<Taquilla> *p = taquillas.ObtPrimero();
                while (p != NULL) {
                    Taquilla &t = p->ObtInfo();
                    if (t.estaAbierta() && t.cantidadClientesEnEspera() < t.getLimitePersonas()) {
                        disponiblesCount++;
                    }
                    p = p->ObtDer();
                }

                if (disponiblesCount == 0) {
                    cout << "Error: no hay taquillas disponibles (todas estan llenas o cerradas)." << endl;
                    break;
                }

                // List all taquillas and show their status
                cout << "\nTaquillas disponibles:" << endl;
                p = taquillas.ObtPrimero();
                int idx = 1;
                while (p != NULL) {
                    Taquilla &t = p->ObtInfo();
                    cout << idx << ") " << t.getNombreTaquilla();
                    if (!t.estaAbierta()) {
                        cout << " [CERRADA]";
                    } else if (t.cantidadClientesEnEspera() >= t.getLimitePersonas()) {
                        cout << " [LLENA (" << t.cantidadClientesEnEspera() << "/" << t.getLimitePersonas() << ")]";
                    } else {
                        cout << " [DISPONIBLE (" << t.cantidadClientesEnEspera() << "/" << t.getLimitePersonas() << ")]";
                    }
                    cout << endl;
                    p = p->ObtDer();
                    idx++;
                }

                int seleccion = leerEntero("Seleccione el numero de taquilla que desea usar: ");
                Taquilla *taquillaElegida = obtenerTaquillaPorIndice(cines[cineActivo], seleccion - 1);
                if (taquillaElegida == NULL || !taquillaElegida->estaAbierta() || taquillaElegida->cantidadClientesEnEspera() >= taquillaElegida->getLimitePersonas()) {
                    cout << "Error: seleccion invalida, la taquilla esta cerrada o llena." << endl;
                    break;
                }

                Persona cliente(cedula, nombre, pelicula, salaNumero);
                cliente.registrarEstatus("En espera");
                if (taquillaElegida->recibirCliente(cliente)) {
                    cout << "Cliente registrado correctamente y enviado a la taquilla elegida." << endl;
                } else {
                    cout << "Error: no se pudo ingresar el cliente en la taquilla." << endl;
                }
                break;
            }
            case 5: {
                if (cineActivo < 0 || cineActivo >= numCines) {
                    cout << "Error: debe crear o seleccionar un cine antes de atender clientes." << endl;
                    break;
                }
                atenderCliente(cines[cineActivo], salas[cineActivo], esperas[cineActivo]);
                break;
            }
            case 6:
                if (cineActivo < 0 || cineActivo >= numCines) {
                    cout << "No hay un cine activo para mostrar." << endl;
                } else {
                    mostrarResumen(cines[cineActivo], salas[cineActivo], esperas[cineActivo]);
                }
                break;
            case 7:
                if (numCines == 0) {
                    cout << "No hay cines registrados." << endl;
                } else {
                    cout << "\nCines registrados:" << endl;
                    for (int i = 0; i < numCines; ++i) {
                        cout << (i + 1) << ") " << cines[i].ObtNombre() << " (ID " << cines[i].ObtId() << ")" << endl;
                    }
                }
                break;
            case 8:
                cout << "Hasta luego." << endl;
                break;
            default:
                cout << "Opcion no valida." << endl;
                break;
        }
    }

    return 0;
}