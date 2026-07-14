#include "Lista/Lista.h"
#include "cola/Cola.h"
#include "persona/Persona.h"
#include "pila/Pila.h"
#include "salas/Salas.h"
#include <iostream>
#include <string>

using namespace std;

// Funciones de visualización para las estructuras base originales
void mostrarPila(Pila<string> &p) {
  Pila<string> aux;
  string valor;
  cout << "TOPE -> ";
  while (p.Remover(valor)) {
    cout << "[" << valor << "] ";
    aux.Insertar(valor);
  }
  while (aux.Remover(valor)) {
    p.Insertar(valor);
  }
  cout << endl;
}

void mostrarCola(Cola<string> &c) {
  if (c.Vacia()) {
    cout << "Cola vacia." << endl;
    return;
  }

  const string marca = "###";
  string valor;

  c.Insertar(marca);
  cout << "FRENTE -> ";
  while (c.Remover(valor) && valor != marca) {
    cout << "[" << valor << "] -> ";
    c.Insertar(valor);
  }
  cout << "FINAL" << endl;
}

void mostrarLista(Lista<string> &l) {
  nodo<string> *p = l.ObtPrimero();
  cout << "INICIO -> ";
  while (p != NULL) {
    cout << "[" << p->ObtInfo() << "] -> ";
    p = p->ObtDer();
  }
  cout << "NULL" << endl;
}

// Menú de pruebas de estructuras base originales (Caso 4)
void probarEstructurasBase() {
  Pila<string> pila;
  Cola<string> cola;
  Lista<string> lista;
  string dato;
  int opcion = 0;

  while (opcion != 4) {
    cout << "\n--- ESTRUCTURAS DE DATOS BASE ---" << endl;
    cout << "1) Pila  2) Cola  3) Lista  4) Regresar al Menu "
            "Principal\nOpcion: ";
    cin >> opcion;

    if (cin.fail()) {
      cin.clear();
      cin.ignore(1000, '\n');
      cout << "Entrada invalida." << endl;
      continue;
    }

    switch (opcion) {
    case 1: {
      int sub = 0;
      cout << "1) Push  2) Pop  3) Ver  4) Total\nOpcion: ";
      cin >> sub;
      if (sub == 1) {
        cout << "Libro: ";
        getline(cin >> ws, dato);
        pila.Insertar(dato);
      } else if (sub == 2) {
        if (pila.Remover(dato))
          cout << "Sale: " << dato << endl;
        else
          cout << "Pila vacia." << endl;
      } else if (sub == 3) {
        mostrarPila(pila);
      } else if (sub == 4) {
        cout << "Total: " << pila.Total() << endl;
      }
      break;
    }
    case 2: {
      int sub = 0;
      cout << "1) Encolar  2) Atender  3) Ver  4) Total\nOpcion: ";
      cin >> sub;
      if (sub == 1) {
        cout << "Cliente: ";
        getline(cin >> ws, dato);
        cola.Insertar(dato);
      } else if (sub == 2) {
        if (cola.Remover(dato))
          cout << "Atendido: " << dato << endl;
        else
          cout << "Cola vacia." << endl;
      } else if (sub == 3) {
        mostrarCola(cola);
      } else if (sub == 4) {
        cout << "Total: " << cola.Total() << endl;
      }
      break;
    }
    case 3: {
      int sub = 0;
      cout << "1) Insertar al inicio  2) Insertar despues del primero  3) "
              "Ver\nOpcion: ";
      cin >> sub;
      if (sub == 1) {
        cout << "Tarea: ";
        getline(cin >> ws, dato);
        lista.InsComienzo(dato);
      } else if (sub == 2) {
        if (lista.ObtPrimero() != NULL) {
          cout << "Tarea: ";
          getline(cin >> ws, dato);
          lista.InsDerecho(lista.ObtPrimero(), dato);
        } else {
          cout << "Lista vacia, usa primero la opcion 1." << endl;
        }
      } else if (sub == 3) {
        mostrarLista(lista);
      }
      break;
    }
    case 4:
      cout << "Regresando..." << endl;
      break;
    default:
      cout << "Opcion no valida." << endl;
      break;
    }
  }
}

// Opción 5: Prueba de Estabilidad Automatizada
void ejecutarPruebaEstabilidad(Salas &cine) {
  int nSalaPrueba = 99;
  string pelPrueba = "Interestelar";

  cout << "\n--- INICIANDO PRUEBA DE ESTABILIDAD AUTOMATICA (10 ENTRAN, 10 "
          "SALEN) ---"
       << endl;

  // Crear la sala de prueba si no existe
  if (!cine.SalaExiste(nSalaPrueba)) {
    cout << "Creando sala de prueba " << nSalaPrueba << " para '" << pelPrueba
         << "'..." << endl;
    cine.AgregarSala(nSalaPrueba, pelPrueba);
  }

  cout << "\n1. Ingresando (Encolando) 10 clientes a la Sala " << nSalaPrueba
       << ":" << endl;
  for (int i = 1; i <= 10; i++) {
    string cedula = "V-00" + to_string(i);
    string nombre = "Cliente_" + to_string(i);

    Persona p(cedula, nombre, pelPrueba, nSalaPrueba);
    p.AgregarEstatus("Llego al cine");
    p.AgregarEstatus("Compro boleto para " + pelPrueba);
    p.AgregarEstatus("Entro a la cola de espera de la Sala " +
                     to_string(nSalaPrueba));

    cout << "   [+] Insertando: " << p.ObtNombre() << " (" << p.ObtCedula()
         << ") -> ";
    if (cine.EncolarCliente(nSalaPrueba, p)) {
      cout << "Exitoso. Clientes en cola: " << cine.TotalClientes(nSalaPrueba)
           << endl;
    } else {
      cout << "FALLIDO." << endl;
    }
  }

  cout << "\nEstado actual de las salas:" << endl;
  cine.Mostrar();

  cout << "\n2. Retirando (Atendiendo) 10 clientes de la Sala " << nSalaPrueba
       << ":" << endl;
  for (int i = 1; i <= 10; i++) {
    Persona pAtendida;
    cout << "   [-] Atendiendo cliente " << i << ": ";
    if (cine.AtenderCliente(nSalaPrueba, pAtendida)) {
      cout << pAtendida.ObtNombre() << " (" << pAtendida.ObtCedula()
           << ") atendido." << endl;
      // Registrar cambio de estatus
      pAtendida.AgregarEstatus("Atendido y salio de la cola");
      cout << "       Historial de Estatus: ";
      pAtendida.MostrarEstatus();
      cout << endl;
    } else {
      cout << "FALLIDO (Cola vacia)." << endl;
    }
  }

  cout << "\nEstado final de las salas:" << endl;
  cine.Mostrar();
  cout << "--- PRUEBA COMPLETADA EXITOSAMENTE ---" << endl;
}

// Submenú de Administrar Salas y Clientes (Caso 1)
void administrarSalas(Salas &cine) {
  int subOpcion = 0;
  while (subOpcion != 5) {
    cout << "\n--- ADMINISTRAR SALAS DE CINE ---" << endl;
    cout << "1. Agregar/Crear Sala" << endl;
    cout << "2. Registrar/Ingresar Cliente manualmente" << endl;
    cout << "3. Atender Cliente de sala" << endl;
    cout << "4. Mostrar estado de Salas y Clientes" << endl;
    cout << "5. Volver al Menu Principal" << endl;
    cout << "Elija una opcion: ";
    cin >> subOpcion;

    if (cin.fail()) {
      cin.clear();
      cin.ignore(1000, '\n');
      cout << "Entrada invalida." << endl;
      continue;
    }

    switch (subOpcion) {
    case 1: {
      int nSala;
      string pelicula;
      cout << "Ingrese el numero de la sala: ";
      cin >> nSala;
      if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Numero de sala invalido." << endl;
        break;
      }
      cout << "Ingrese la pelicula asignada: ";
      getline(cin >> ws, pelicula);

      if (cine.AgregarSala(nSala, pelicula)) {
        cout << "Sala " << nSala << " agregada con la pelicula '" << pelicula
             << "'." << endl;
      } else {
        cout << "Error: No se pudo agregar la sala (Sala duplicada o limite de "
                "salas alcanzado)."
             << endl;
      }
      break;
    }
    case 2: {
      if (cine.TotalSalas() == 0) {
        cout << "Error: No hay salas creadas. Primero cree una sala." << endl;
        break;
      }
      int nSala;
      string cedula, nombre;
      cout << "Ingrese el numero de sala donde encolar: ";
      cin >> nSala;
      if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Numero de sala invalido." << endl;
        break;
      }
      if (!cine.SalaExiste(nSala)) {
        cout << "Error: La sala " << nSala << " no existe." << endl;
        break;
      }
      cout << "Cedula del cliente: ";
      getline(cin >> ws, cedula);
      cout << "Nombre del cliente: ";
      getline(cin >> ws, nombre);

      string pelicula;
      cine.ObtenerPelicula(nSala, pelicula);

      Persona p(cedula, nombre, pelicula, nSala);
      p.AgregarEstatus("Llego al cine");
      p.AgregarEstatus("Espera en cola de Sala " + to_string(nSala));

      if (cine.EncolarCliente(nSala, p)) {
        cout << "Cliente encolado correctamente en sala " << nSala << "."
             << endl;
      } else {
        cout << "Error al encolar el cliente." << endl;
      }
      break;
    }
    case 3: {
      if (cine.TotalSalas() == 0) {
        cout << "Error: No hay salas creadas." << endl;
        break;
      }
      int nSala;
      cout << "Ingrese el numero de sala para atender cliente: ";
      cin >> nSala;
      if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Numero de sala invalido." << endl;
        break;
      }

      Persona pAtendida;
      if (cine.AtenderCliente(nSala, pAtendida)) {
        cout << "\nCliente atendido en Sala " << nSala << ":" << endl;
        cout << "Nombre: " << pAtendida.ObtNombre()
             << " | Cedula: " << pAtendida.ObtCedula() << endl;
        pAtendida.AgregarEstatus("Atendido y retirado de la cola");
        cout << "Historial de estatus: ";
        pAtendida.MostrarEstatus();
        cout << endl;
      } else {
        cout << "No hay clientes en la cola o la sala no existe." << endl;
      }
      break;
    }
    case 4:
      cout << "\n--- ESTADO DE SALAS ---" << endl;
      cine.Mostrar();
      break;
    case 5:
      cout << "Volviendo..." << endl;
      break;
    default:
      cout << "Opcion no valida." << endl;
      break;
    }
  }
}

int main() {
  Salas cine;
  int opcion = 0;

  while (opcion != 4) {
    cout << "\n==============================================" << endl;
    cout << "      SISTEMA DE GESTION DE SALAS DE CINE" << endl;
    cout << "==============================================" << endl;
    cout << "1. Gestionar Salas y Clientes (Modulo Salas)" << endl;
    cout << "2. Ejecutar Prueba de Estabilidad (10 Entran, 10 Salen)" << endl;
    cout << "3. Probar Estructuras de Datos Base (Original)" << endl;
    cout << "4. Salir del Programa" << endl;
    cout << "Elija una opcion: ";
    cin >> opcion;

    if (cin.fail()) {
      cin.clear();
      cin.ignore(1000, '\n');
      cout << "Entrada invalida." << endl;
      continue;
    }

    switch (opcion) {
    case 1:
      administrarSalas(cine);
      break;
    case 2:
      ejecutarPruebaEstabilidad(cine);
      break;
    case 3:
      probarEstructurasBase();
      break;
    case 4:
      cout << "Saliendo del programa. ¡Hasta luego!" << endl;
      break;
    default:
      cout << "Opcion no valida." << endl;
      break;
    }
  }

  return 0;
}