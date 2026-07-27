#include "Simulacion.h"

#include <iostream>
#include <limits>
#include <sstream>
#include <cstdlib>

using namespace std;

// =====================================================================
//  PANTALLA Y ENTRADA DE DATOS
// =====================================================================

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

string leerTexto(const string &mensaje) {
    cout << mensaje;
    string valor;
    if (!getline(cin >> ws, valor)) {
        return "";
    }
    return valor;
}

int leerEntero(const string &mensaje) {
    while (true) {
        cout << mensaje;
        string entrada;
        if (!getline(cin >> ws, entrada)) {
            // Entrada cerrada (EOF): se devuelve un valor imposible en vez de
            // quedarse en un bucle infinito pidiendo el dato.
            return -1;
        }
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
        if (!getline(cin >> ws, entrada)) {
            return 0.0;
        }
        stringstream ss(entrada);
        float valor;
        if (ss >> valor && ss.eof() && valor >= 0) {
            return valor;
        }
        cout << "Error: el precio debe ser un numero mayor o igual a 0." << endl;
    }
}

// =====================================================================
//  CONSULTAS SOBRE LAS ESTRUCTURAS
// =====================================================================

Taquilla *obtenerTaquillaPorIndice(Cine &cine, int indice) {
    if (indice < 0) return NULL;

    Lista<Taquilla> &taquillas = cine.ObtListaTaquillas();
    nodo<Taquilla> *p = taquillas.ObtPrimero();
    int actual = 0;

    while (p != NULL) {
        if (actual == indice) return &p->ObtInfo();
        p = p->ObtDer();
        ++actual;
    }

    return NULL;
}

// Devuelve el indice de la taquilla abierta con menos gente que todavia tenga
// cupo, o -1 si todas estan llenas o cerradas.
int elegirTaquillaConCupo(Cine &cine) {
    Lista<Taquilla> &taquillas = cine.ObtListaTaquillas();
    nodo<Taquilla> *p = taquillas.ObtPrimero();
    int mejor = -1;
    int menor = 0;
    int idx = 0;

    while (p != NULL) {
        Taquilla &t = p->ObtInfo();
        int ocupadas = t.cantidadClientesEnEspera();

        if (t.estaAbierta() && ocupadas < t.getLimitePersonas()) {
            if (mejor == -1 || ocupadas < menor) {
                menor = ocupadas;
                mejor = idx;
            }
        }

        p = p->ObtDer();
        ++idx;
    }

    return mejor;
}

// La taquilla no decide la sala por su numero: busca en la cartelera donde se
// proyecta la pelicula que pidio el cliente. Si la misma pelicula esta en
// varias salas, manda a la que tenga menos gente esperando.
int resolverSalaParaPelicula(Salas &salas, const string &pelicula) {
    int total = salas.TotalSalas();
    int elegida = 0;
    int menorCola = -1;

    for (int i = 0; i < total; ++i) {
        int nSala;
        string peliculaSala;
        if (!salas.ObtenerSalaPorIndice(i, nSala, peliculaSala)) continue;
        if (peliculaSala != pelicula) continue;

        int enCola = salas.TotalClientes(nSala);
        if (menorCola < 0 || enCola < menorCola) {
            menorCola = enCola;
            elegida = nSala;
        }
    }

    return elegida; // 0 = la pelicula ya no esta en cartelera
}

// =====================================================================
//  VISUALIZACION
// =====================================================================

// Recorre la cola sin destruirla: la vacia en una auxiliar y la devuelve
// respetando el orden original.
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
             << " | " << persona.ObtenerEstatusActual() << endl;
        copia.Insertar(persona);
    }

    while (copia.Remover(persona)) {
        cola.Insertar(persona);
    }
}

// Muestra SOLO las peliculas: el cliente que llega al cine no sabe en que sala
// dan cada una, eso se lo informan en la taquilla.
void mostrarCartelera(Salas &salas) {
    int total = salas.TotalSalas();

    if (total == 0) {
        cout << "No hay peliculas en cartelera." << endl;
        return;
    }

    cout << "\n--- CARTELERA ---" << endl;
    for (int i = 0; i < total; ++i) {
        int nSala;
        string pelicula;
        if (salas.ObtenerSalaPorIndice(i, nSala, pelicula)) {
            cout << (i + 1) << ") " << pelicula << endl;
        }
    }
}

void mostrarTaquillas(Cine &cine) {
    Lista<Taquilla> &taquillas = cine.ObtListaTaquillas();
    nodo<Taquilla> *p = taquillas.ObtPrimero();
    int idx = 1;

    cout << "\n--- TAQUILLAS ---" << endl;
    while (p != NULL) {
        Taquilla &t = p->ObtInfo();
        cout << idx << ") " << t.getNombreTaquilla()
             << " | $" << t.getPrecioBoleto()
             << " | Fila: " << t.cantidadClientesEnEspera() << "/" << t.getLimitePersonas()
             << (t.estaAbierta() ? "" : " (CERRADA)") << endl;
        p = p->ObtDer();
        ++idx;
    }
}

void mostrarResumen(Sede &sede) {
    cout << "=========================================" << endl;
    cout << "        RESUMEN DEL CINE ACTIVO          " << endl;
    cout << "=========================================" << endl;
    cout << "Cine: " << sede.cine.ObtNombre() << " (ID " << sede.cine.ObtId() << ")" << endl;
    cout << "Precio del dia: $" << sede.cine.ObtPrecioDelDia() << endl;
    cout << "-----------------------------------------" << endl;

    cout << "[1] FILA GENERAL DEL CINE (todas las taquillas llenas): "
         << sede.esperaGeneral.Total() << endl;
    mostrarColaClientes(sede.esperaGeneral, "");
    cout << "-----------------------------------------" << endl;

    cout << "[2] FILAS DE TAQUILLA (comprando boleto)" << endl;
    Lista<Taquilla> &taquillas = sede.cine.ObtListaTaquillas();
    nodo<Taquilla> *pTaquilla = taquillas.ObtPrimero();
    int idx = 1;

    if (pTaquilla == NULL) {
        cout << "   (no hay taquillas registradas)" << endl;
    }

    while (pTaquilla != NULL) {
        Taquilla &t = pTaquilla->ObtInfo();
        cout << " " << idx << ") " << t.getNombreTaquilla()
             << " | Ocupacion: " << t.cantidadClientesEnEspera() << "/" << t.getLimitePersonas() << endl;
        mostrarColaClientes(t.getColaTaquilla(), "");
        pTaquilla = pTaquilla->ObtDer();
        ++idx;
    }
    cout << "-----------------------------------------" << endl;

    cout << "[3] FILAS DE SALA (esperando para entrar)" << endl;
    int totalSalas = sede.salas.TotalSalas();

    if (totalSalas == 0) {
        cout << "   (no hay salas registradas)" << endl;
    }

    for (int i = 0; i < totalSalas; ++i) {
        int nSala;
        string pelicula;
        if (!sede.salas.ObtenerSalaPorIndice(i, nSala, pelicula)) continue;

        cout << " Sala " << nSala << " | " << pelicula
             << " | En puerta: " << sede.salas.TotalClientes(nSala) << endl;

        Cola<Persona> *colaSala = NULL;
        if (sede.salas.ObtenerCola(nSala, colaSala) && colaSala != NULL) {
            mostrarColaClientes(*colaSala, "");
        }
    }
    cout << "-----------------------------------------" << endl;

    cout << "[4] VIENDO LA FUNCION: " << sede.enFuncion.Total() << endl;
    mostrarColaClientes(sede.enFuncion, "");
    cout << "=========================================\n" << endl;
}

// =====================================================================
//  ETAPA 1: EL CLIENTE LLEGA AL CINE
// =====================================================================

bool registrarCliente(Sede &sede) {
    if (sede.salas.TotalSalas() == 0 || sede.cine.ObtListaTaquillas().Total() == 0) {
        cout << "Error: configure salas y taquillas antes de recibir clientes." << endl;
        return false;
    }

    string cedula = leerTexto("Cedula del cliente: ");
    string nombre = leerTexto("Nombre del cliente: ");

    // El cliente pide una PELICULA, no una sala.
    mostrarCartelera(sede.salas);
    int opcionPelicula = leerEntero("Que pelicula quiere ver: ");

    int nSalaInformativo;
    string pelicula;
    if (!sede.salas.ObtenerSalaPorIndice(opcionPelicula - 1, nSalaInformativo, pelicula)) {
        cout << "Error: esa pelicula no esta en cartelera." << endl;
        return false;
    }

    // Sala 0 = todavia no sabe a que sala va; eso se lo diran en la taquilla.
    Persona cliente(cedula, nombre, pelicula, 0);
    cliente.registrarEstatus("Llego al cine a ver: " + pelicula);

    mostrarTaquillas(sede.cine);
    int seleccion = leerEntero("En que taquilla se pone (0 = la que tenga menos gente): ");

    int indice = (seleccion == 0) ? elegirTaquillaConCupo(sede.cine) : seleccion - 1;
    Taquilla *taquilla = obtenerTaquillaPorIndice(sede.cine, indice);

    if (taquilla != NULL && taquilla->estaAbierta() &&
        taquilla->cantidadClientesEnEspera() < taquilla->getLimitePersonas()) {
        taquilla->recibirCliente(cliente);
        cout << "\n[LLEGADA] " << nombre << " se puso en la fila de "
             << taquilla->getNombreTaquilla() << " para ver " << pelicula << "." << endl;
        return true;
    }

    // Todas llenas (o eligio una llena): espera en el lobby hasta que se libere
    // cupo. repartirEsperaGeneral lo movera solo.
    cliente.registrarEstatus("Esperando en el lobby (taquillas llenas)");
    sede.esperaGeneral.Insertar(cliente);
    cout << "\n[LLEGADA] " << nombre << " no consiguio cupo en taquilla y quedo "
         << "en la fila general del cine." << endl;
    return true;
}

void repartirEsperaGeneral(Sede &sede) {
    while (!sede.esperaGeneral.Vacia()) {
        int idx = elegirTaquillaConCupo(sede.cine);
        if (idx < 0) break; // sigue sin haber cupo en ninguna taquilla

        Taquilla *taquilla = obtenerTaquillaPorIndice(sede.cine, idx);
        if (taquilla == NULL) break;

        Persona cliente;
        if (!sede.esperaGeneral.Remover(cliente)) break;

        if (!taquilla->recibirCliente(cliente)) {
            sede.esperaGeneral.Insertar(cliente);
            break;
        }

        cout << "[LOBBY] " << cliente.getNombre() << " paso de la fila general a "
             << taquilla->getNombreTaquilla() << "." << endl;
    }
}

// =====================================================================
//  ETAPA 2: COMPRA EL BOLETO Y LE INDICAN LA SALA
// =====================================================================

bool atenderUnCliente(Sede &sede, int indiceTaquilla) {
    Taquilla *taquilla = obtenerTaquillaPorIndice(sede.cine, indiceTaquilla);

    if (taquilla == NULL) {
        cout << "Esa taquilla no existe." << endl;
        return false;
    }

    Persona cliente;
    if (!taquilla->atenderSiguienteCliente(cliente)) {
        return false; // fila vacia
    }

    int nSala = resolverSalaParaPelicula(sede.salas, cliente.getPelicula());

    if (nSala == 0) {
        cliente.registrarEstatus("Se retiro: " + cliente.getPelicula() + " salio de cartelera");
        cout << "[TAQUILLA] " << cliente.getNombre() << ": la pelicula "
             << cliente.getPelicula() << " ya no esta en cartelera. Se retira." << endl;
        repartirEsperaGeneral(sede);
        return true;
    }

    // Aqui es donde el cliente se entera de su sala.
    cliente.setSala(nSala);
    cliente.registrarEstatus("Boleto comprado. Debe ir a la Sala " + to_string(nSala));

    cout << "[TAQUILLA] " << taquilla->getNombreTaquilla() << " atendio a "
         << cliente.getNombre() << ": boleto de " << cliente.getPelicula()
         << " por $" << taquilla->getPrecioBoleto() << "." << endl;
    cout << "           Se le indica que vaya a la SALA " << nSala << "." << endl;

    // Se encola DESPUES de dejarle el estatus puesto, para que la copia que
    // queda en la cola conserve el historial completo.
    if (!sede.salas.EncolarCliente(nSala, cliente)) {
        cout << "           Error: no se pudo enviar a la sala " << nSala << "." << endl;
        return true;
    }

    // Se libero un puesto en la taquilla: puede entrar alguien del lobby.
    repartirEsperaGeneral(sede);
    return true;
}

// =====================================================================
//  ETAPA 3: ENTRA A LA SALA
// =====================================================================

bool ingresarUnCliente(Sede &sede, int nSala) {
    Persona cliente;
    if (!sede.salas.AtenderCliente(nSala, cliente)) {
        return false; // nadie esperando en la puerta de esa sala
    }

    cliente.registrarEstatus("Viendo " + cliente.getPelicula() + " en la Sala " + to_string(nSala));
    sede.enFuncion.Insertar(cliente);

    cout << "[SALA " << nSala << "] " << cliente.getNombre()
         << " entro a ver " << cliente.getPelicula() << "." << endl;
    return true;
}

// =====================================================================
//  ETAPA 4: TERMINA LA FUNCION Y SE VA
// =====================================================================

bool salirUnCliente(Sede &sede) {
    Persona cliente;
    if (!sede.enFuncion.Remover(cliente)) {
        return false; // no hay nadie en funcion
    }

    cliente.registrarEstatus("Termino la funcion y salio del cine");

    cout << "[SALIDA] " << cliente.getNombre() << " termino "
         << cliente.getPelicula() << " y abandona el cine." << endl;
    cliente.mostrarHistorialEstatus();
    cout << "---------------------------------------------------" << endl;
    return true;
}

// =====================================================================
//  AVANCE DE UN SOLO PASO (usado por la version paso a paso)
// =====================================================================

// Se atiende primero la etapa mas avanzada para que cada cliente complete su
// recorrido antes de que empiece el siguiente.
bool avanzarUnPaso(Sede &sede) {
    if (salirUnCliente(sede)) return true;

    int totalSalas = sede.salas.TotalSalas();
    for (int i = 0; i < totalSalas; ++i) {
        int nSala;
        string pelicula;
        if (!sede.salas.ObtenerSalaPorIndice(i, nSala, pelicula)) continue;
        if (ingresarUnCliente(sede, nSala)) return true;
    }

    int totalTaquillas = sede.cine.ObtListaTaquillas().Total();
    for (int i = 0; i < totalTaquillas; ++i) {
        Taquilla *t = obtenerTaquillaPorIndice(sede.cine, i);
        if (t == NULL || t->getColaTaquilla().Vacia()) continue;
        if (atenderUnCliente(sede, i)) return true;
    }

    if (!sede.esperaGeneral.Vacia()) {
        repartirEsperaGeneral(sede);
        return true;
    }

    return false;
}

// =====================================================================
//  MENUS DE CONFIGURACION
// =====================================================================

void menuGestionCines(Sede sedes[], int &numSedes, int &sedeActiva) {
    limpiarPantalla();
    cout << "--- GESTION DE CINES ---" << endl;
    cout << "1) Crear cine" << endl;
    cout << "2) Seleccionar cine activo" << endl;
    cout << "3) Modificar cine activo" << endl;
    cout << "4) Volver" << endl;
    int sub = leerEntero("Opcion: ");

    if (sub == 1) {
        if (numSedes >= MAX_CINES) {
            cout << "Error: maximo " << MAX_CINES << " cines." << endl;
        } else {
            int id = leerEntero("ID del cine: ");
            string nombre = leerTexto("Nombre del cine: ");
            sedes[numSedes].cine.AsigId(id);
            sedes[numSedes].cine.AsigNombre(nombre);
            ++numSedes;
            sedeActiva = numSedes - 1;
            cout << "Cine creado y activado." << endl;
        }
    } else if (sub == 2) {
        if (numSedes == 0) {
            cout << "No hay cines registrados." << endl;
        } else {
            mostrarCinesRegistrados(sedes, numSedes);
            int idx = leerEntero("Seleccione cine: ");
            if (idx >= 1 && idx <= numSedes) {
                sedeActiva = idx - 1;
                cout << "Cine activo cambiado." << endl;
            } else {
                cout << "Opcion invalida." << endl;
            }
        }
    } else if (sub == 3 && sedeActiva >= 0) {
        int id = leerEntero("Nuevo ID: ");
        string nombre = leerTexto("Nuevo nombre: ");
        sedes[sedeActiva].cine.AsigId(id);
        sedes[sedeActiva].cine.AsigNombre(nombre);
    }
}

void menuGestionTaquillas(Sede &sede) {
    limpiarPantalla();
    cout << "--- GESTION DE TAQUILLAS ---" << endl;
    cout << "1) Agregar taquilla" << endl;
    cout << "2) Abrir / cerrar taquilla" << endl;
    cout << "3) Establecer precio del dia" << endl;
    cout << "4) Ver taquillas" << endl;
    cout << "5) Volver" << endl;
    int sub = leerEntero("Opcion: ");

    Lista<Taquilla> &taquillas = sede.cine.ObtListaTaquillas();

    if (sub == 1) {
        if (taquillas.Total() >= 3) {
            cout << "Error: maximo 3 taquillas." << endl;
        } else {
            int numero = taquillas.Total() + 1;
            int limite = leerEntero("Limite de personas en la fila: ");
            Taquilla t("Taquilla " + to_string(numero), sede.cine.ObtPrecioDelDia(), limite);
            sede.cine.AgregarTaquilla(t);
            cout << "Taquilla " << numero << " agregada." << endl;
        }
    } else if (sub == 2) {
        mostrarTaquillas(sede.cine);
        int idx = leerEntero("Numero de taquilla: ");
        Taquilla *t = obtenerTaquillaPorIndice(sede.cine, idx - 1);
        if (t == NULL) {
            cout << "Esa taquilla no existe." << endl;
        } else {
            t->setAbierta(!t->estaAbierta());
            cout << t->getNombreTaquilla() << " ahora esta "
                 << (t->estaAbierta() ? "ABIERTA" : "CERRADA") << "." << endl;
        }
    } else if (sub == 3) {
        float p = leerPrecio("Precio del dia: ");
        sede.cine.AsigPrecioDelDia(p);
        nodo<Taquilla> *n = taquillas.ObtPrimero();
        while (n != NULL) {
            n->ObtInfo().setPrecioBoleto(p);
            n = n->ObtDer();
        }
        cout << "Precio actualizado en todas las taquillas." << endl;
    } else if (sub == 4) {
        mostrarTaquillas(sede.cine);
    }
}

void menuGestionSalas(Sede &sede) {
    limpiarPantalla();
    cout << "--- GESTION DE SALAS ---" << endl;
    cout << "1) Agregar sala" << endl;
    cout << "2) Cambiar pelicula de una sala" << endl;
    cout << "3) Ver salas" << endl;
    cout << "4) Volver" << endl;
    int sub = leerEntero("Opcion: ");

    if (sub == 1) {
        int n = leerEntero("Numero de sala (1 a 3): ");
        string p = leerTexto("Pelicula: ");
        if (sede.salas.AgregarSala(n, p)) {
            cout << "Sala " << n << " agregada con " << p << "." << endl;
        } else {
            cout << "No se pudo agregar (numero repetido o limite alcanzado)." << endl;
        }
    } else if (sub == 2) {
        sede.salas.Mostrar();
        int n = leerEntero("Numero de sala: ");
        string p = leerTexto("Nueva pelicula: ");
        if (sede.salas.CambiarPelicula(n, p)) {
            cout << "Cartelera actualizada." << endl;
        } else {
            cout << "Esa sala no existe." << endl;
        }
    } else if (sub == 3) {
        sede.salas.Mostrar();
    }
}

void mostrarCinesRegistrados(Sede sedes[], int numSedes) {
    cout << "Cines registrados:" << endl;
    for (int i = 0; i < numSedes; ++i) {
        cout << (i + 1) << ") " << sedes[i].cine.ObtNombre()
             << " (ID " << sedes[i].cine.ObtId() << ")" << endl;
    }
}

// =====================================================================
//  DATOS DE ARRANQUE PARA LA DRAMATIZACION
// =====================================================================

void precargarDatos(Sede sedes[], int &numSedes, int &sedeActiva) {
    numSedes = 1;
    sedeActiva = 0;

    sedes[0].cine.AsigId(101);
    sedes[0].cine.AsigNombre("Cines Unidos Trinitarias");
    sedes[0].cine.AsigPrecioDelDia(5.0);

    // Dos taquillas con fila corta, para que se vea llenarse y usar el lobby.
    Taquilla t1("Taquilla 1", 5.0, 3);
    Taquilla t2("Taquilla 2", 5.0, 3);
    sedes[0].cine.AgregarTaquilla(t1);
    sedes[0].cine.AgregarTaquilla(t2);

    sedes[0].salas.AgregarSala(1, "Avengers: Endgame");
    sedes[0].salas.AgregarSala(2, "Spider-Man: No Way Home");
}
