#ifndef __MAPA_H__
#define __MAPA_H__

#include "cocos2d.h"

class Mapa : public cocos2d::Node {
protected:
    int mapa[20][12];  // Matriz que define el mapa
    cocos2d::Sprite* fondo;  // Fondo del mapa

public:
    Mapa();

    // Método virtual para cargar el fondo del mapa
    virtual void cargarFondo() = 0;

    // Método virtual para actualizar el mapa (en cada frame)
    virtual void actualizar(float dt) = 0;

    // Método para mostrar el fondo
    void mostrarFondo();

    // Método para establecer el mapa (utilizado por las clases derivadas)
    void setMapa(int nuevoMapa[20][12]);

    // Metodo para tener posicion
    int obtenerValorEnPosicion(int x, int y);
};

#endif // __MAPA_H__
