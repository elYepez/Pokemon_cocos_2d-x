#include "Mapa2.h"

Mapa2::Mapa2() {
    // Inicializar la matriz del mapa (por ejemplo)

    //2 pasto
    //1 arbol
    //6y9 tp
    int mapa2[20][12] = {
    {01,01,01,01,01,01,01,01,01,01,01,01},
    {01,01,01,01,01,01,01,01,01,01,01,01},
    {01,00,00,00,01,01,01,01,00,00,00,01},
    {01,00,00,00,00,00,00,00,00,00,00,01},
    {01,00,00,00,00,00,00,00,00,01,01,01},
    {01,00,00,00,00,00,00,00,00,01,01,01},
    {01,00,00,00,00,00,00,00,00,01,01,01},
    {01,02,02,00,02,02,02,00,00,00,00,01},
    {01,02,02,00,02,02,02,02,02,02,02,01},
    {01,02,02,00,00,00,00,00,00,00,02,01},
    {01,02,02,00,00,00,00,00,00,00,02,01},
    {01,01,01,01,01,01,01,01,02,00,02,01},
    {01,01,01,01,01,01,01,01,02,00,02,01},
    {01,00,00,00,00,00,00,00,00,00,02,01},
    {01,00,00,00,00,00,00,00,00,00,02,01},
    {01,01,01,00,00,00,00,02,02,01,01,01},
    {01,01,01,00,00,00,00,02,02,01,01,01},
    {01,01,01,01,00,00,00,01,01,01,01,01},
    {01,01,01,01,01,14,14,01,01,01,01,01},
    {01,01,01,01,01,01,01,01,01,01,01,01}
    };
    setMapa(mapa2);
}

void Mapa2::cargarFondo() {
    fondo = cocos2d::Sprite::create("mapa_pokemon_1.png");
    if (fondo != nullptr) {
        fondo->setPosition(cocos2d::Director::getInstance()->getVisibleSize() / 2);
        this->addChild(fondo, -1);  // Agregar el fondo al nodo
    }
}

void Mapa2::actualizar(float dt) {
}
