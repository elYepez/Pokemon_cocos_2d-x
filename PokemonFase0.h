#ifndef __POKEMON_FASE0_H__
#define __POKEMON_FASE0_H__

#include "cocos2d.h"
#include <set> // Para manejar las teclas presionadas
#include "Mapa.h"
#include "Pokemon.h"


USING_NS_CC;

class PokemonFase0 : public Scene
{
public:
   // Layer* cameraLayer; //La camara que sigue al personaje


    // Métodos estáticos
    static cocos2d::Scene* createScene();

    // Inicialización de la escena
    virtual bool init() override;

    // Método update llamado en cada frame
    void update(float dt);

    // Métodos para manejar las teclas
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

    // Métodos para crear animaciones de caminar
    void crearAnimacionCaminarDerecha();
    void crearAnimacionCaminarIzquierda();
    void crearAnimacionCaminarArriba();
    void crearAnimacionCaminarAbajo();

    // Métodos para inicializar elementos de la escena
    void agregarFondo();
    void inicializarElementos();
    bool puedeMoverse(int x, int y);
    void inicializarPokemones();
    void cambiarMapa(const std::string& direccion);
private:
    // Miembros de la clase
    Sprite* personaje;  // Personaje principal
    std::set<EventKeyboard::KeyCode> keysPressed;  // Conjunto para las teclas presionadas


    cocos2d::Node* cameraNode;
    cocos2d::Follow* cameraFollow;
    void setupCamera();
    void escalarTodo();

    // Variables de control de animaciones
    bool animacionActiva;
    bool isPressed;
    int tiles;
    

    Mapa* mapaActual;

    // Implementación de CREATE_FUNC para esta clase
    CREATE_FUNC(PokemonFase0);
};

#endif // __POKEMON_FASE0_H__
