#include <iostream>
#include <string>
#include "Cola.h"

using namespace std;

template <class Tipo>
void mostrarCola(Cola<Tipo> &c, Tipo marca) { // El PDF pide pasar una marca
    if (c.Vacia()) {
        cout << "La fila esta vacia." << endl;
        return;
    }

    Tipo valor;
    bool encontro = false;

    c.Insertar(marca); // Se inserta la marca al final antes de empezar
    cout << "FRENTE -> ";

    while (!encontro) {
        c.Remover(valor); // Sacamos el elemento
        
        if (valor == marca) { // Si es la marca, ya dimos la vuelta completa
            encontro = true;
        } else {
            cout << "[" << valor << "] -> "; // Procesamos/Imprimimos
            c.Insertar(valor); // Lo reinsertamos al final
        }
    }
    cout << "FINAL" << endl;
}

int main() {
    // Creamos una cola de strings para los nombres de las personas
    Cola<string> filaCine;
    string persona;
    int opcion;

    do {
        cout << "\n--- TAQUILLA DE CINE ---" << endl;
        cout << "1. Llegada de persona a la fila" << endl;
        cout << "2. Atender a la siguiente persona" << endl;
        cout << "3. Ver quien es el proximo en la fila" << endl;
        cout << "4. Ver total de personas esperando" << endl;
        cout << "5. Imprimir cola" << endl;
        cout << "6. Salir" << endl;
        cout << "Opcion: ";
        cin >> opcion;
        
        if (cin.fail()) {
            cout << "\nError: Se debe ingresar un caracter numerico. Cerrando sismta..." << endl;
            break;
        }

        switch (opcion) {
            case 1:
                cout << "Nombre de la persona: ";
                cin >> persona;
                if (filaCine.Insertar(persona))
                    cout << persona << " se ha unido a la fila." << endl;
                break;

            case 2:
                if (filaCine.Remover(persona))
                    cout << "Atendiendo a: " << persona << ". Disfrute la pelicula!" << endl;
                else
                    cout << "No hay nadie en la fila." << endl;
                break;

            case 3:
                if (filaCine.VerFrente(persona))
                    cout << "El siguiente en ser atendido es: " << persona << endl;
                else
                    cout << "La fila esta vacia." << endl;
                break;

            case 4:
                cout << "Personas en espera: " << filaCine.Total() << endl;
                break;
            case 5:
                cout << "Imprimir cola: " << endl;
                mostrarCola(filaCine, (string)"$"); // Usamos "$" como marca de fin
                break;
        }
    } while (opcion != 6);

    return 0;
}