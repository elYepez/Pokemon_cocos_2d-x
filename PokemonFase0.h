#ifndef __POKEMON_FASE0_H__
#define __POKEMON_FASE0_H__

#include "cocos2d.h"
#include <set> // Para manejar las teclas presionadas
#include "Mapa.h"
#include "Pokemon.h"
#include <vector>
#include "AudioManager.h"

USING_NS_CC;

class PokemonFase0 : public Scene
{
public:
   // Layer* cameraLayer; //La camara que sigue al personaje


   
    static cocos2d::Scene* createScene(Pokemon* pokemon);

    // Inicializacion de la escena
    virtual bool init(Pokemon* pokemon);
    static PokemonFase0* create(Pokemon* pokemon) {
        PokemonFase0* ret = new (std::nothrow) PokemonFase0();
        if (ret && ret->init(pokemon)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    const cocos2d::Vector<Pokemon*>& getGaleriaPokemones() const { return galeriaPokemones; }
    void agregarPokemon(Pokemon* pokemon);

    Pokemon* getPokemonSalvajeEnPosicion(int index);//Sistema pokemon salvaje aleatorio
    int getCantidadPokemonesSalvajes() { return PokemonesSalvajesAleatorios.size(); }
    void agregarPokemonSalvaje(Pokemon* pokemonSalvaje);

    Pokemon* getPokemonEnPosicion(int index);
    Pokemon* getPokemonPorNombre(const std::string& nombre);
    int getCantidadPokemones() { return galeriaPokemones.size(); }
    void mostrarPokemones();
    void liberarPokemon(int index);
    void liberarPokemonPorNombre(const std::string& nombre);
    void iniciarBatalla(Pokemon* pokemonSalvaje);
    void capturarPokemon(Pokemon* pokemonSalvaje);

    void detenerPersonajeManual();

    
    void update(float dt);

    
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

    
    void crearAnimacionCaminarDerecha();
    void crearAnimacionCaminarIzquierda();
    void crearAnimacionCaminarArriba();
    void crearAnimacionCaminarAbajo();

    
    void agregarFondo();
    void inicializarElementos();
    bool puedeMoverse(int x, int y);
    void inicializarPokemones();
    void cambiarMapa(const std::string& direccion);
private:
    
    Pokemon* pokemonInicial;
   cocos2d::Vector<Pokemon*> galeriaPokemones; //guardar pokemones

   cocos2d::Vector<Pokemon*> PokemonesSalvajesAleatorios; //ruleta de pokemones salvajes


   AudioManager* audioManager;


    
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
    //CREATE_FUNC(PokemonFase0);  lo quitamos ya que creamos el nuestro para pasar parametros
};

#endif // __POKEMON_FASE0_H__
