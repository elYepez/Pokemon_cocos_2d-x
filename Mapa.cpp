#include "Mapa.h"
#include <cstring>  // Agrega esta cabecera para usar memcpy

Mapa::Mapa() {
    // Inicialización común de todos los mapas
    memset(mapa, 0, sizeof(mapa));  // Inicializar la matriz con ceros
    fondo = nullptr;
}

void Mapa::setMapa(int nuevoMapa[20][12]) {
    memcpy(mapa, nuevoMapa, sizeof(mapa));  // Copia la nueva matriz al mapa
}

void Mapa::mostrarFondo() {
    if (fondo) {
        this->addChild(fondo, -1);  // Asegúrate de agregar el fondo a la escena
    }
}

int Mapa::obtenerValorEnPosicion(int x, int y) {
    return mapa[y][x];
}
