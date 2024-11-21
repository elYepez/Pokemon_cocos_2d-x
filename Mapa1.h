#ifndef __MAPA1_H__
#define __MAPA1_H__

#include "Mapa.h"  // Incluir la clase base

class Mapa1 : public Mapa {
public:
    Mapa1();  // Constructor

    // Implementación de los métodos virtuales
    void cargarFondo() override;
    void actualizar(float dt) override;
};

#endif // __MAPA1_H__
