CXX      = g++
CXXFLAGS = -std=c++11 -I. -Iinclude

# Cola.cpp, Lista.cpp y Pila.cpp NO se listan aqui: son cuerpos de plantilla
# que ya se incluyen desde su propio .h y no compilan por separado.
FUENTES = core/Simulacion.cpp Cine/Cine.cpp persona/Persona.cpp \
          salas/Salas.cpp taquilla/Taquilla.cpp

all: cine_app

cine_app:
	$(CXX) $(CXXFLAGS) main.cpp $(FUENTES) -o cine_app.exe

clean:
	rm -f cine_app.exe

.PHONY: all cine_app clean
