#include "Persona.h"

// Constructor
Persona::Persona() {
    cedula = "";
    nombre = "";
    pelicula = "";
    sala = 0;
    asientoAsignado = 0;
}

// Constructor parametrizado
Persona::Persona(string c, string n, string p, int s) {
    cedula = c;
    nombre = n;
    pelicula = p;
    sala = s;
    asientoAsignado = 0; 
}

// Destructor
Persona::~Persona() {
    // La pilaEstatus se destruye automáticamente
}

// Getters
string Persona::getCedula() { return cedula; }
string Persona::getNombre() { return nombre; }
string Persona::getPelicula() { return pelicula; }
int Persona::getSala() { return sala; }
int Persona::getAsientoAsignado() { return asientoAsignado; }

// Setters
void Persona::setCedula(string c) { cedula = c; }
void Persona::setNombre(string n) { nombre = n; }
void Persona::setPelicula(string p) { pelicula = p; }
void Persona::setSala(int s) { 
    // Validación de que existen 3 salas
    if (s >= 1 && s <= 3) {
        sala = s; 
    } else {
        sala = 0; // 0 significa no asignada
    }
}
void Persona::setAsientoAsignado(int a) { asientoAsignado = a; }

void Persona::registrarDatos() {
    cout << "\n--- Registro de Cliente ---" << endl;
    cout << "Ingrese Cedula: ";
    getline(cin >> ws, cedula);
    cout << "Ingrese Nombre: ";
    getline(cin >> ws, nombre);
    
    registrarEstatus("Datos Registrados en el Sistema");
}

void Persona::registrarEstatus(string estatus) {
    pilaEstatus.Insertar(estatus);
}

string Persona::obtenerEstatusActual() {
    string estatus;
    if (pilaEstatus.VerTope(estatus)) {
        return estatus;
    }
    return "Sin Estatus"; 
}

void Persona::mostrarHistorialEstatus() {
    Pila<string> pilaAux;
    string valor;
    
    cout << "\nHistorial de Estatus de " << nombre << ":" << endl;
    
    while (pilaEstatus.Remover(valor)) {
        cout << " -> " << valor << endl;
        pilaAux.Insertar(valor);
    }
    
    while (pilaAux.Remover(valor)) {
        pilaEstatus.Insertar(valor);
    }
}

void Persona::mostrarDatos() {
    cout << "---------------------------------" << endl;
    cout << "Cedula: " << cedula << endl;
    cout << "Nombre: " << nombre << endl;
    cout << "Pelicula: " << (pelicula == "" ? "No seleccionada" : pelicula) << endl;
    cout << "Sala: " << (sala == 0 ? "No asignada" : to_string(sala)) << endl;
    cout << "Asiento: " << (asientoAsignado == 0 ? "Sin Asignar" : to_string(asientoAsignado)) << endl;
    cout << "Estatus Actual: " << obtenerEstatusActual() << endl;
    cout << "---------------------------------" << endl;
}
