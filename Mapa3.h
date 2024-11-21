#pragma once
#ifndef __MAPA3_H__
#define __MAPA3_H__

#include "Mapa.h"  // Incluir la clase base

class Mapa3 : public Mapa {
public:
    Mapa3();  // Constructor

    // Implementación de los métodos virtuales
    void cargarFondo() override;
    void actualizar(float dt) override;
};

#endif // __MAPA1_H__
#pragma once
