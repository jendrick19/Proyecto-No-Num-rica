#include <iostream>
#include <string>
#include "pila/Pila.h"
#include "cola/Cola.h"
#include "lista/Lista.h"

using namespace std;

// --- RUTINAS A NIVEL DE APLICACIÓN ---

// Impresión de Pila: Usa una pila auxiliar para no destruir los datos
void probarPila(Pila<string> &p) {
    Pila<string> aux;
    string valor;
    cout << "TOP -> ";
    while (p.Remover(valor)) {
        cout << "[" << valor << "] " << endl;
        aux.Insertar(valor);
    }
    while (aux.Remover(valor)) p.Insertar(valor); // Restaurar
}

// Impresión de Cola: Usa la técnica de la "marca"
void probarCola(Cola<string> &c) {
    string marca = "###";
    string valor;
    bool fin = false;
    if (c.Vacia()) return;
    c.Insertar(marca); // Insertar marca al final
    cout << "FRENTE -> ";
    while (!fin) {
        c.Remover(valor);
        if (valor == marca) fin = true;
        else {
            cout << "[" << valor << "] -> ";
            c.Insertar(valor); // Reinsertar
        }
    }
    cout << "FINAL" << endl;
}

// Impresión de Lista: Recorrido directo por punteros
void probarLista(Lista<string> &l) {
    nodo<string> *p = l.ObtPrimero();
    cout << "INICIO -> ";
    while (p != NULL) {
        cout << "[" << p->ObtInfo() << "] -> ";
        p = p->ObtDer(); // Avanzar
    }
    cout << "NULL" << endl;
}

int main() {
    Pila<string> libros;
    Cola<string> banco;
    Lista<string> tareas;
    int opcion, sub;
    string dato;

    do {
        cout << "\n==== LABORATORIO DE ESTRUCTURAS (UCLA) ====" << endl;
        cout << "1. Probar PILA (Libros)" << endl;
        cout << "2. Probar COLA (Banco)" << endl;
        cout << "3. Probar LISTA (Tareas)" << endl;
        cout << "4. Salir" << endl;
        cout << "Seleccion: "; cin >> opcion;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        if (opcion == 1) { // Lógica LIFO
            cout << "1. Push (Añadir) | 2. Pop (Sacar) | 3. Ver Todo | 4. Total" << endl;
            cin >> sub;
            if (sub == 1) { cout << "Libro: "; getline(cin >> ws, dato); libros.Insertar(dato); }
            else if (sub == 2) { if (libros.Remover(dato)) cout << "Salió: " << dato << endl; }
            else if (sub == 3) probarPila(libros);
            else if (sub == 4) cout << "Total: " << libros.Total() << endl;
        } 
        else if (opcion == 2) { // Lógica FIFO
            cout << "1. Encolar | 2. Atender | 3. Ver Fila | 4. Total" << endl;
            cin >> sub;
            if (sub == 1) { cout << "Cliente: "; getline(cin >> ws, dato); banco.Insertar(dato); }
            else if (sub == 2) { if (banco.Remover(dato)) cout << "Atendido: " << dato << endl; }
            else if (sub == 3) probarCola(banco);
            else if (sub == 4) cout << "Total: " << banco.Total() << endl;
        }
        else if (opcion == 3) { // Acceso Secuencial
            cout << "1. Ins. Comienzo | 2. Ins. Derecho (después del 1ro) | 3. Ver Lista" << endl;
            cin >> sub;
            if (sub == 1) { cout << "Tarea: "; getline(cin >> ws, dato); tareas.InsComienzo(dato); }
            else if (sub == 2) { cout << "Tarea: "; getline(cin >> ws, dato); tareas.InsDerecho(tareas.ObtPrimero(), dato); }
            else if (sub == 3) probarLista(tareas);
        }
    } while (opcion != 4);

    return 0;
}