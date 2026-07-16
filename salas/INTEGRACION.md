# Integracion del modulo Salas

Este modulo gestiona la **lista de salas del cine** (maximo 3) usando `Lista<Sala>`.
Cada sala tiene un numero, una pelicula y una `Cola<Persona>` con los clientes.

La clase **Persona** la implementa otro integrante. Salas no crea personas ni maneja estatus.

---

## Estructura esperada

```
Lista<Sala>  (Salas)
    └── Sala
          ├── nSala      (int)
          ├── pelicula   (string)
          └── colaSala   (Cola<Persona>)
                └── Persona  (modulo externo)
                      └── PilaEstatus  (Pila<string>, modulo Pila)
```

---

## Requisitos del modulo Persona

Para que Salas compile y funcione, `persona/Persona.h` debe definir la clase con al menos:

| Requisito | Motivo |
|-----------|--------|
| Constructor por defecto | Clonado interno de colas |
| Constructor de copia y `operator=` | Al insertar en `Lista` y `Cola` se copia `Sala` y `Persona` |
| `void AsigCedula(const string&)` | Clonado interno de colas (marca temporal) |
| `string ObtCedula() const` | Clonado interno de colas |

Campos sugeridos segun el diagrama del proyecto:

- `cedula`, `nombre`, `pelicula`, `sala`
- `Pila<string> pilaEstatus` (o nombre equivalente)

La **copia profunda** de `Pila` y `Cola` anidadas es responsabilidad del modulo Persona
(en su constructor de copia), porque las librerias base no tienen copia profunda por defecto.

---

## Pasos para conectar

### 1. Crear el modulo Persona

Ubicacion sugerida:

```
persona/
  Persona.h
  Persona.cpp   (si aplica)
```

### 2. Activar el include en Salas.h

En `salas/Salas.h`, reemplazar el forward declaration por el include real:

```cpp
// Quitar o comentar:
// #ifndef PERSONA_H
// class Persona;
// #endif

// Agregar:
#include "../persona/Persona.h"
```

`Salas.cpp` ya incluye `../persona/Persona.h` al compilar.

### 3. Compilar

```bash
g++ -std=c++11 -o programa main.cpp persona/Persona.cpp
```

Los templates (`Lista`, `Cola`, `Pila`) se incluyen desde los headers; no requieren `.o` propio.

### 4. Usar desde main u otro modulo

```cpp
#include "salas/Salas.h"
#include "persona/Persona.h"

Salas cine;
cine.AgregarSala(1, "Avatar");

// El modulo Persona arma al cliente y su estatus inicial
Persona cliente("12345678", "Juan", "Avatar", 1);
cliente.AgregarEstatus("Compro entrada");

cine.EncolarCliente(1, cliente);

Persona atendido;
if (cine.AtenderCliente(1, atendido)) {
    atendido.AgregarEstatus("Atendido");  // logica de Persona
}

cine.Mostrar();
```

---

## API publica de Salas

| Metodo | Descripcion |
|--------|-------------|
| `AgregarSala(nSala, pelicula)` | Crea sala al final de la lista (max. 3) |
| `SalaExiste(nSala)` | Indica si la sala ya existe |
| `CambiarPelicula(nSala, pelicula)` | Actualiza la pelicula de una sala |
| `ObtenerPelicula(nSala, pelicula)` | Lee la pelicula actual |
| `EncolarCliente(nSala, cliente)` | Inserta un `Persona` en la cola de esa sala |
| `AtenderCliente(nSala, cliente)` | Saca el primero de la cola (FIFO) |
| `ObtenerCola(nSala, cola)` | Devuelve puntero a la cola para operaciones avanzadas |
| `TotalSalas()` / `TotalClientes(nSala)` | Contadores |
| `Mostrar()` | Lista salas, pelicula y cantidad de clientes (sin detalle de Persona) |

---

## Division de responsabilidades

| Modulo | Responsabilidad |
|--------|-----------------|
| **Salas** | Lista de salas, pelicula por sala, encolar/desencolar clientes |
| **Persona** | Datos del cliente, compra de entrada, pila de estatus |
| **Cola** | Estructura FIFO dentro de cada sala |
| **Lista** | Estructura de la lista de salas |
| **Pila** | Historial de estatus dentro de cada Persona |
| **main** | Menu y orquestacion entre modulos |

---

## Nota sobre nodo.h

`nodo::ObtInfo()` devuelve **referencia** (`Tipo&`) para que cambios en
`pelicula` o `colaSala` persistan dentro del nodo de la lista.

---

## Ejemplo de flujo completo (cine)

```
main
  ├─ Salas::AgregarSala(...)
  ├─ Persona::crear + AgregarEstatus("Compro entrada")
  ├─ Salas::EncolarCliente(nSala, persona)
  ├─ Salas::AtenderCliente(nSala, persona)
  ├─ Persona::AgregarEstatus("Atendido")
  └─ Salas::Mostrar()
```
