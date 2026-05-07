#include <iostream>
#include <string>
#include "pila/Pila.h"
#include "cola/Cola.h"
#include "lista/Lista.h"

using namespace std;

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

int main() {
    Pila<string> pila;
    Cola<string> cola;
    Lista<string> lista;
    string dato;
    int opcion = 0;

    while (opcion != 4) {
        cout << "\n1) Pila  2) Cola  3) Lista  4) Salir\nOpcion: ";
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
                    if (pila.Remover(dato)) cout << "Sale: " << dato << endl;
                    else cout << "Pila vacia." << endl;
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
                    if (cola.Remover(dato)) cout << "Atendido: " << dato << endl;
                    else cout << "Cola vacia." << endl;
                } else if (sub == 3) {
                    mostrarCola(cola);
                } else if (sub == 4) {
                    cout << "Total: " << cola.Total() << endl;
                }
                break;
            }
            case 3: {
                int sub = 0;
                cout << "1) Insertar al inicio  2) Insertar despues del primero  3) Ver\nOpcion: ";
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
                cout << "Hasta luego." << endl;
                break;
            default:
                cout << "Opcion no valida." << endl;
                break;
        }
    }

    return 0;
}