#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <cstdlib> // Para system("cls") o system("clear")
#include "Cine/Cine.h"
#include "salas/Salas.h"
#include "taquilla/Taquilla.h"
#include "persona/Persona.h"

using namespace std;

// --- FUNCIONES DE LIMPIEZA Y PANTALLA ---

void limpiarPantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pausar() {
    cout << "\nPresione ENTER para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

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

// --- VISUALIZACIÓN DE ESTADOS ---

void mostrarColaClientes(Cola<Persona> &cola, const string &titulo) {
    cout << titulo << endl;
    if (cola.Vacia()) {
        cout << "   (vacia)" << endl;
        return;
    }

    Cola<Persona> copia;
    Persona persona;
    while (cola.Remover(persona)) {
        cout << "   - " << persona.getNombre() << " (V-" << persona.getCedula() << ")"
             << " | Estado: [" << persona.ObtenerEstatusActual() << "]" << endl;
        copia.Insertar(persona);
    }

    while (copia.Remover(persona)) {
        cola.Insertar(persona);
    }
}

void mostrarResumen(Cine &cine, Salas &salas, Cola<Persona> &espera) {
    cout << "=========================================" << endl;
    cout << "        RESUMEN DEL CINE ACTIVO          " << endl;
    cout << "=========================================" << endl;
    cout << "Cine: " << cine.ObtNombre() << " (ID " << cine.ObtId() << ")" << endl;
    cout << "Precio del Dia: $" << cine.ObtPrecioDelDia() << endl;
    cout << "-----------------------------------------" << endl;

    cout << "Clientes en espera general: " << espera.Total() << endl;
    mostrarColaClientes(espera, "Fila general:");
    cout << "-----------------------------------------" << endl;

    cout << "Taquillas registradas: " << cine.ObtListaTaquillas().Total() << endl;
    Lista<Taquilla> &taquillas = cine.ObtListaTaquillas();
    nodo<Taquilla> *pTaquilla = taquillas.ObtPrimero();
    int idx = 1;
    while (pTaquilla != NULL) {
        Taquilla &t = pTaquilla->ObtInfo();
        cout << " - " << idx << ") " << t.getNombreTaquilla()
             << " | Precio: $" << t.getPrecioBoleto()
             << " | Ocupacion: " << t.cantidadClientesEnEspera() << "/" << t.getLimitePersonas() << endl;
        if (t.cantidadClientesEnEspera() > 0) {
            mostrarColaClientes(t.getColaTaquilla(), "    Fila en " + t.getNombreTaquilla() + ":");
        }
        pTaquilla = pTaquilla->ObtDer();
        ++idx;
    }
    cout << "-----------------------------------------" << endl;

    cout << "Salas registradas: " << salas.TotalSalas() << endl;
    salas.Mostrar();
    cout << "=========================================\n" << endl;
}

// --- LOGICA DE NEGOCIO Y DATOS ---

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

    Taquilla *tEliminar = obtenerTaquillaPorIndice(cine, indice);
    if (tEliminar == NULL) return false;

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

        if (target != NULL) {
            Cola<Persona> &qEliminar = tEliminar->getColaTaquilla();
            Cola<Persona> &qTarget = target->getColaTaquilla();

            Cola<Persona> temp;
            Persona pTemp;
            while (qTarget.Remover(pTemp)) {
                temp.Insertar(pTemp);
            }

            int countMovidos = 0;
            while (qEliminar.Remover(pTemp)) {
                pTemp.registrarEstatus("Reubicado a " + target->getNombreTaquilla());
                qTarget.Insertar(pTemp);
                countMovidos++;
            }

            while (temp.Remover(pTemp)) {
                qTarget.Insertar(pTemp);
            }

            if (countMovidos > 0) {
                cout << "Se reubicaron " << countMovidos << " clientes a " << target->getNombreTaquilla() << endl;
            }
        } else {
            cout << "Advertencia: No hay otra taquilla abierta disponible." << endl;
        }
    }

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
        mensajeError = "Error: no hay taquillas configuradas.";
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
        if (idx < 0) break;

        Taquilla *taquilla = obtenerTaquillaPorIndice(cine, idx);
        if (taquilla == NULL) break;

        Persona cliente;
        if (!espera.Remover(cliente)) break;

        if (!taquilla->recibirCliente(cliente)) {
            espera.Insertar(cliente);
            break;
        }
    }
}

// SIMULACIÓN Y CAMBIO DE ESTADOS
bool atenderCliente(Cine &cine, Salas &salas, Cola<Persona> &espera) {
    Lista<Taquilla> &taquillas = cine.ObtListaTaquillas();
    if (taquillas.Total() == 0) {
        cout << "Error: no hay taquillas registradas." << endl;
        return false;
    }

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
    if (taquillaElegida == NULL || taquillaElegida->getColaTaquilla().Vacia()) {
        cout << "La taquilla seleccionada esta vacia o no existe." << endl;
        return false;
    }

    int countAtendidos = 0;
    Persona cliente;
    while (taquillaElegida->atenderSiguienteCliente(cliente)) {
        // TRANSICIÓN 1: Pasa de Taquilla a Espera de Sala
        cliente.registrarEstatus("Esperando Entrada a Sala " + to_string(cliente.getSala()));
        
        if (cliente.getSala() > 0) {
            if (salas.EncolarCliente(cliente.getSala(), cliente)) {
                cout << "[TAQUILLA] " << cliente.getNombre() << " compro su boleto." << endl;
                cout << "[TRANSICION] Estatus actualizado: " << cliente.ObtenerEstatusActual() << endl;
                
                // TRANSICIÓN 2: Entra a ver la película
                cliente.registrarEstatus("Viendo Pelicula: " + cliente.getPelicula());
                cout << "[SALA] " << cliente.getNombre() << " ha ingresado a la Sala " << cliente.getSala() << "." << endl;
                cout << "[TRANSICION] Estatus actualizado: " << cliente.ObtenerEstatusActual() << endl;

                // TRANSICIÓN 3: Finaliza el proceso y sale
                cliente.registrarEstatus("Proceso Finalizado (Salio del Cine)");
                cout << "[SALIDA] " << cliente.getNombre() << " termino la pelicula y abandona el cine." << endl;
                cout << "[TRANSICION] Estatus final: " << cliente.ObtenerEstatusActual() << endl;
                cout << "---------------------------------------------------" << endl;
            } else {
                cout << "Error: No fue posible enviar al cliente a la sala " << cliente.getSala() << "." << endl;
            }
        }
        countAtendidos++;
    }

    cout << "\nSe procesaron " << countAtendidos << " clientes en " << taquillaElegida->getNombreTaquilla() << "." << endl;
    repartirEspera(cine, salas, espera);
    return true;
}

// PRECARGA DE DATOS PARA AGILIZAR DRAMATIZACIÓN
void precargarDatos(Cine cines[], Salas salas[], int &numCines, int &cineActivo) {
    numCines = 1;
    cineActivo = 0;

    // Configurar Cine
    cines[0].AsigId(101);
    cines[0].AsigNombre("Cines Unidos Trinitarias");
    cines[0].AsigPrecioDelDia(5.0);

    // Configurar Taquillas
    Taquilla t1("Taquilla 1", 5.0, 5);
    Taquilla t2("Taquilla 2", 5.0, 5);
    cines[0].AgregarTaquilla(t1);
    cines[0].AgregarTaquilla(t2);

    // Configurar Salas
    salas[0].AgregarSala(1, "Avengers: Endgame");
    salas[0].AgregarSala(2, "Spider-Man: No Way Home");
}

int main() {
    Cine cines[3];
    Salas salas[3];
    Cola<Persona> esperas[3];
    int numCines = 0;
    int cineActivo = -1;

    // Cargar datos por defecto al iniciar
    precargarDatos(cines, salas, numCines, cineActivo);

    int opcion = 0;
    while (opcion != 8) {
        limpiarPantalla();
        cout << "=========================================" << endl;
        cout << "        SISTEMA DE GESTION DE CINE       " << endl;
        cout << "=========================================" << endl;
        if (cineActivo >= 0) {
            cout << "CINE ACTIVO: " << cines[cineActivo].ObtNombre() 
                 << " (ID: " << cines[cineActivo].ObtId() << ")" << endl;
        } else {
            cout << "CINE ACTIVO: Ninguno" << endl;
        }
        cout << "-----------------------------------------" << endl;
        cout << "1) Gestionar cines" << endl;
        cout << "2) Gestionar taquillas" << endl;
        cout << "3) Gestionar salas" << endl;
        cout << "4) Registrar cliente" << endl;
        cout << "5) Atender cliente (Simular Flujo)" << endl;
        cout << "6) Ver resumen de colas" << endl;
        cout << "7) Ver cines registrados" << endl;
        cout << "8) Salir" << endl;
        cout << "=========================================" << endl;
        opcion = leerEntero("Opcion: ");

        switch (opcion) {
            case 1: {
                limpiarPantalla();
                cout << "--- GESTION DE CINES ---" << endl;
                cout << "1) Crear cine" << endl;
                cout << "2) Seleccionar cine activo" << endl;
                cout << "3) Modificar cine activo" << endl;
                cout << "4) Volver" << endl;
                int sub = leerEntero("Opcion: ");

                if (sub == 1) {
                    if (numCines >= 3) {
                        cout << "Error: maximo 3 cines." << endl;
                    } else {
                        int id = leerEntero("ID del cine: ");
                        string nombre = leerTexto("Nombre del cine: ");
                        cines[numCines].AsigId(id);
                        cines[numCines].AsigNombre(nombre);
                        ++numCines;
                        cineActivo = numCines - 1;
                        cout << "Cine creado y activado." << endl;
                    }
                } else if (sub == 2) {
                    if (numCines == 0) {
                        cout << "No hay cines registrados." << endl;
                    } else {
                        for (int i = 0; i < numCines; ++i) {
                            cout << (i + 1) << ") " << cines[i].ObtNombre() << " (ID " << cines[i].ObtId() << ")" << endl;
                        }
                        int idx = leerEntero("Seleccione cine: ");
                        if (idx >= 1 && idx <= numCines) {
                            cineActivo = idx - 1;
                        }
                    }
                } else if (sub == 3 && cineActivo >= 0) {
                    int id = leerEntero("Nuevo ID: ");
                    string nombre = leerTexto("Nuevo nombre: ");
                    cines[cineActivo].AsigId(id);
                    cines[cineActivo].AsigNombre(nombre);
                }
                pausar();
                break;
            }
            case 2: {
                limpiarPantalla();
                if (cineActivo < 0) {
                    cout << "Error: Seleccione un cine primero." << endl;
                    pausar();
                    break;
                }
                cout << "--- GESTION DE TAQUILLAS ---" << endl;
                cout << "1) Agregar taquilla" << endl;
                cout << "2) Modificar precio" << endl;
                cout << "3) Eliminar taquilla" << endl;
                cout << "4) Ver taquillas" << endl;
                cout << "5) Establecer precio del dia" << endl;
                cout << "6) Volver" << endl;
                int sub = leerEntero("Opcion: ");

                if (sub == 1) {
                    if (cines[cineActivo].ObtListaTaquillas().Total() >= 3) {
                        cout << "Error: Maximo 3 taquillas." << endl;
                    } else {
                        int numero = cines[cineActivo].ObtListaTaquillas().Total() + 1;
                        int limite = leerEntero("Limite de personas: ");
                        Taquilla t("Taquilla " + to_string(numero), cines[cineActivo].ObtPrecioDelDia(), limite);
                        cines[cineActivo].AgregarTaquilla(t);
                    }
                } else if (sub == 3) {
                    int index = leerEntero("Numero de taquilla a eliminar: ");
                    eliminarTaquilla(cines[cineActivo], index - 1);
                } else if (sub == 4) {
                    mostrarResumen(cines[cineActivo], salas[cineActivo], esperas[cineActivo]);
                } else if (sub == 5) {
                    float p = leerPrecio("Precio del dia: ");
                    cines[cineActivo].AsigPrecioDelDia(p);
                }
                pausar();
                break;
            }
            case 3: {
                limpiarPantalla();
                if (cineActivo < 0) {
                    cout << "Error: Seleccione un cine primero." << endl;
                    pausar();
                    break;
                }
                cout << "--- GESTION DE SALAS ---" << endl;
                cout << "1) Agregar sala" << endl;
                cout << "2) Ver salas" << endl;
                cout << "3) Volver" << endl;
                int sub = leerEntero("Opcion: ");
                if (sub == 1) {
                    int n = leerEntero("Numero de sala: ");
                    string p = leerTexto("Pelicula: ");
                    salas[cineActivo].AgregarSala(n, p);
                } else if (sub == 2) {
                    salas[cineActivo].Mostrar();
                }
                pausar();
                break;
            }
            case 4: {
                limpiarPantalla();
                if (cineActivo < 0 || salas[cineActivo].TotalSalas() == 0 || cines[cineActivo].ObtListaTaquillas().Total() == 0) {
                    cout << "Error: Configure cine, salas y taquillas antes de registrar clientes." << endl;
                    pausar();
                    break;
                }

                string cedula = leerTexto("Cedula del Cliente: ");
                string nombre = leerTexto("Nombre del Cliente: ");
                int salaNumero = leerEntero("Numero de Sala: ");

                string pelicula;
                if (!salas[cineActivo].ObtenerPelicula(salaNumero, pelicula)) {
                    cout << "Error: La sala ingresada no existe." << endl;
                    pausar();
                    break;
                }

                Lista<Taquilla> &taquillas = cines[cineActivo].ObtListaTaquillas();
                nodo<Taquilla> *p = taquillas.ObtPrimero();
                int idx = 1;
                cout << "\nTaquillas Disponibles:" << endl;
                while (p != NULL) {
                    Taquilla &t = p->ObtInfo();
                    cout << idx << ") " << t.getNombreTaquilla() 
                         << " [" << t.cantidadClientesEnEspera() << "/" << t.getLimitePersonas() << "]" << endl;
                    p = p->ObtDer();
                    idx++;
                }

                int seleccion = leerEntero("Seleccione Taquilla: ");
                Taquilla *taquillaElegida = obtenerTaquillaPorIndice(cines[cineActivo], seleccion - 1);

                if (taquillaElegida != NULL && taquillaElegida->cantidadClientesEnEspera() < taquillaElegida->getLimitePersonas()) {
                    Persona cliente(cedula, nombre, pelicula, salaNumero);
                    // Estado Inicial
                    cliente.registrarEstatus("En espera por taquilla (" + taquillaElegida->getNombreTaquilla() + ")");
                    taquillaElegida->recibirCliente(cliente);
                    cout << "\n[OK] Cliente asignado correctamente con estatus: " << cliente.ObtenerEstatusActual() << endl;
                } else {
                    cout << "Error: Taquilla invalida o llena." << endl;
                }
                pausar();
                break;
            }
            case 5: {
                limpiarPantalla();
                if (cineActivo < 0) {
                    cout << "Error: Seleccione un cine activo." << endl;
                } else {
                    atenderCliente(cines[cineActivo], salas[cineActivo], esperas[cineActivo]);
                }
                pausar();
                break;
            }
            case 6:
                limpiarPantalla();
                if (cineActivo >= 0) {
                    mostrarResumen(cines[cineActivo], salas[cineActivo], esperas[cineActivo]);
                } else {
                    cout << "No hay cine activo." << endl;
                }
                pausar();
                break;
            case 7:
                limpiarPantalla();
                cout << "Cines Registrados:" << endl;
                for (int i = 0; i < numCines; ++i) {
                    cout << (i + 1) << ") " << cines[i].ObtNombre() << " (ID " << cines[i].ObtId() << ")" << endl;
                }
                pausar();
                break;
            case 8:
                cout << "Saliendo del programa..." << endl;
                break;
        }
    }

    return 0;
}