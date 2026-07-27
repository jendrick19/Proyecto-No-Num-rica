CXX      = g++
CXXFLAGS = -std=c++11 -I. -Iinclude

# Cola.cpp, Lista.cpp y Pila.cpp NO se listan aqui: son cuerpos de plantilla
# que ya se incluyen desde su propio .h y no compilan por separado.
FUENTES = core/Simulacion.cpp Cine/Cine.cpp persona/Persona.cpp \
          salas/Salas.cpp taquilla/Taquilla.cpp

all: cine_etapas cine_pasoapaso

# Version 1: cada etapa del recorrido es una opcion distinta del menu.
cine_etapas:
	$(CXX) $(CXXFLAGS) main_etapas.cpp $(FUENTES) -o cine_etapas.exe

# Version 2: la simulacion avanza de a un cliente y un paso.
cine_pasoapaso:
	$(CXX) $(CXXFLAGS) main_pasoapaso.cpp $(FUENTES) -o cine_pasoapaso.exe

# Version original, se conserva como referencia.
cine_app:
	$(CXX) $(CXXFLAGS) main.cpp Cine/Cine.cpp persona/Persona.cpp \
	    salas/Salas.cpp taquilla/Taquilla.cpp -o cine_app.exe

clean:
	rm -f cine_etapas.exe cine_pasoapaso.exe cine_app.exe

.PHONY: all cine_etapas cine_pasoapaso cine_app clean
