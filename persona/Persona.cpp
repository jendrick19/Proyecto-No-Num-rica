#include "Persona.h"

Persona::Persona() {
    cedula = "";
    nombre = "";
    pelicula = "";
    sala = 0;
    asientoAsignado = 0;
}

Persona::Persona(string c, string n, string p, int s) {
    cedula = c;
    nombre = n;
    pelicula = p;
    sala = s;
    asientoAsignado = 0;
}

Persona::Persona(const Persona &otra) {
    cedula = otra.cedula;
    nombre = otra.nombre;
    pelicula = otra.pelicula;
    sala = otra.sala;
    asientoAsignado = otra.asientoAsignado;

    Pila<string> aux;
    Pila<string> &otraPila = const_cast<Pila<string> &>(otra.pilaEstatus);
    string valor;
    while (otraPila.Remover(valor)) {
        aux.Insertar(valor);
    }
    while (aux.Remover(valor)) {
        pilaEstatus.Insertar(valor);
        otraPila.Insertar(valor);
    }
}

Persona &Persona::operator=(const Persona &otra) {
    if (this != &otra) {
        cedula = otra.cedula;
        nombre = otra.nombre;
        pelicula = otra.pelicula;
        sala = otra.sala;
        asientoAsignado = otra.asientoAsignado;

        Pila<string> aux;
        Pila<string> &otraPila = const_cast<Pila<string> &>(otra.pilaEstatus);
        string valor;
        while (pilaEstatus.Remover(valor)) {
        }
        while (otraPila.Remover(valor)) {
            aux.Insertar(valor);
        }
        while (aux.Remover(valor)) {
            pilaEstatus.Insertar(valor);
            otraPila.Insertar(valor);
        }
    }
    return *this;
}

Persona::~Persona() {
}

string Persona::getCedula() { return cedula; }
string Persona::getNombre() { return nombre; }
string Persona::getPelicula() { return pelicula; }
int Persona::getSala() { return sala; }
int Persona::getAsientoAsignado() { return asientoAsignado; }

void Persona::setCedula(string c) { cedula = c; }
void Persona::setNombre(string n) { nombre = n; }
void Persona::setPelicula(string p) { pelicula = p; }
void Persona::setSala(int s) {
    if (s >= 1 && s <= 3) {
        sala = s;
    } else {
        sala = 0;
    }
}
void Persona::setAsientoAsignado(int a) { asientoAsignado = a; }

void Persona::AsigCedula(const string &c) { cedula = c; }
string Persona::ObtCedula() const { return cedula; }
void Persona::AsigNombre(const string &n) { nombre = n; }
string Persona::ObtNombre() const { return nombre; }
void Persona::AsigPelicula(const string &p) { pelicula = p; }
string Persona::ObtPelicula() const { return pelicula; }
void Persona::AsigSala(int s) { setSala(s); }
int Persona::ObtSala() const { return sala; }

void Persona::registrarDatos() {
    cout << "\n--- Registro de Cliente ---" << endl;
    cout << "Ingrese Cedula: ";
    getline(cin >> ws, cedula);
    cout << "Ingrese Nombre: ";
    getline(cin >> ws, nombre);

    registrarEstatus("Datos Registrados en el Sistema");
}

void Persona::registrarEstatus(string estatus) {
    AgregarEstatus(estatus);
}

void Persona::AgregarEstatus(const string &estatus) {
    pilaEstatus.Insertar(estatus);
}

string Persona::obtenerEstatusActual() {
    string estatus;
    if (pilaEstatus.VerTope(estatus)) {
        return estatus;
    }
    return "Sin Estatus";
}

string Persona::ObtenerEstatusActual() const {
    Persona *self = const_cast<Persona *>(this);
    return self->obtenerEstatusActual();
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
