#ifndef __MAPA2_H__
#define __MAPA2_H__

#include "Mapa.h"  // Incluir la clase base

class Mapa2 : public Mapa {
public:
    Mapa2();  // Constructor

    // Implementación de los métodos virtuales
    void cargarFondo() override;
    void actualizar(float dt) override;
};

#endif 
#pragma once
