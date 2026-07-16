#include "Taquilla.h"

Taquilla::Taquilla(string nombre, float precio, int limite) {
    nombreTaquilla = nombre;
    precioBoleto = (precio >= 0) ? precio : 0.0;
    abierta = true;
    limitePersonas = (limite >= 1) ? limite : 10;
}

Taquilla::~Taquilla() {
    // La cola se limpia automáticamente invocando su propio destructor genérico
}

// Getters y Setters
string Taquilla::getNombreTaquilla() const { return nombreTaquilla; }
void Taquilla::setNombreTaquilla(string nombre) { nombreTaquilla = nombre; }
float Taquilla::getPrecioBoleto() const { return precioBoleto; }
void Taquilla::setPrecioBoleto(float precio) { 
    if (precio >= 0) precioBoleto = precio; 
}
bool Taquilla::estaAbierta() const { return abierta; }
void Taquilla::setAbierta(bool estado) { abierta = estado; }
int Taquilla::cantidadClientesEnEspera() const { return filaClientes.Total(); }
int Taquilla::getLimitePersonas() const { return limitePersonas; }
void Taquilla::setLimitePersonas(int limite) { 
    if (limite >= 1) limitePersonas = limite; 
}

Cola<Persona>& Taquilla::getColaTaquilla() { return filaClientes; }

// Recibe un cliente validando estado y límite físico estricto
bool Taquilla::recibirCliente(Persona p) {
    if (!abierta) {
        cout << nombreTaquilla << " esta CERRADA. No puede recibir clientes." << endl;
        return false;
    }
    
    if (filaClientes.Total() >= limitePersonas) {
        cout << nombreTaquilla << " tiene la fila llena (max. " << limitePersonas << " personas)." << endl;
        return false;
    }
    
    p.registrarEstatus("1: En cola de " + nombreTaquilla);
    return filaClientes.Insertar(p);
}

// Atiende al siguiente cliente de la fila (FIFO)
bool Taquilla::atenderSiguienteCliente(Persona &pAtendida) {
    if (filaClientes.Vacia()) {
        return false;
    }
    
    if (filaClientes.Remover(pAtendida)) {
        pAtendida.registrarEstatus("2: Atendido en " + nombreTaquilla + ". Boleto emitido.");
        return true;
    }
    
    return false;
}

// Muestra la fila aplicando el Método de la Marca (Recirculación pura de 360°)
void Taquilla::mostrarFilaTaquilla() {
    if (filaClientes.Vacia()) {
        cout << "[" << nombreTaquilla << "]: Fila vacia." << endl;
        return;
    }

    cout << "[" << nombreTaquilla << " - Precio: $" << precioBoleto << "] FRENTE -> ";
    
    Persona marca;
    filaClientes.Remover(marca);
    cout << "[" << marca.getNombre() << " (V-" << marca.getCedula() << ")] -> ";
    filaClientes.Insertar(marca);
    
    if (filaClientes.Total() == 1) {
        cout << "FINAL" << endl;
        return;
    }

    Persona pAux;
    while (filaClientes.VerFrente(pAux) && pAux.getCedula() != marca.getCedula()) {
        filaClientes.Remover(pAux);
        cout << "[" << pAux.getNombre() << " (V-" << pAux.getCedula() << ")] -> ";
        filaClientes.Insertar(pAux);
    }
    cout << "FINAL" << endl;
}

// Busca un cliente aplicando el Método de la Marca
bool Taquilla::buscarClientePorCedula(string cedula) {
    if (filaClientes.Vacia()) {
        cout << nombreTaquilla << " no tiene clientes en espera." << endl;
        return false;
    }

    bool encontrado = false;
    Persona marca;
    
    filaClientes.Remover(marca);
    if (marca.getCedula() == cedula) {
        cout << "\n[Cliente encontrado en " << nombreTaquilla << "]:" << endl;
        marca.mostrarDatos();
        encontrado = true;
    }
    filaClientes.Insertar(marca);
    
    if (filaClientes.Total() == 1) {
        if (!encontrado) cout << "Cliente con cedula " << cedula << " no esta en " << nombreTaquilla << "." << endl;
        return encontrado;
    }

    Persona pAux;
    while (filaClientes.VerFrente(pAux) && pAux.getCedula() != marca.getCedula()) {
        filaClientes.Remover(pAux);
        if (pAux.getCedula() == cedula) {
            cout << "\n[Cliente encontrado en " << nombreTaquilla << "]:" << endl;
            pAux.mostrarDatos();
            encontrado = true;
        }
        filaClientes.Insertar(pAux);
    }
    
    if (!encontrado) {
        cout << "Cliente con cedula " << cedula << " no esta en " << nombreTaquilla << "." << endl;
    }
    
    return encontrado;
}