
#ifndef __BATALLA_POKEMON_H__
#define __BATALLA_POKEMON_H__
#include <functional>
#include <vector>
#include "cocos2d.h"
#include "Pokemon.h"
#include "AudioManager.h"


class BatallaPokemon : public cocos2d::Scene {
public:
    typedef std::function<void(Pokemon*)> PokemonCapturado;
    void setPokemonCapturado(const PokemonCapturado& callback) {
        onPokemonCapturado = callback;
    }

    static BatallaPokemon* create(Pokemon* pokemonJugador, Pokemon* pokemonSalvaje, cocos2d::Vector<Pokemon*> galeriaPokemones);
    virtual bool init(Pokemon* pokemonJugador, Pokemon* pokemonSalvaje, cocos2d::Vector<Pokemon*> galeriaPokemones);

    // Callbacks para los botones
    void onAtaquePressed(Ref* sender);
    void onHuirPressed(Ref* sender);
    void onCapturarPressed(Ref* sender);
    void mostrarMenuPrincipal(Ref* sender);

private:
    AudioManager* audioManager;

    PokemonCapturado onPokemonCapturado;
    
    Pokemon* pokemonJugador;
    Pokemon* pokemonSalvaje;
    cocos2d::Vector<Pokemon*> galeriaPokemones;

    cocos2d::Sprite* spritePokemonJugador;
    cocos2d::Sprite* spritePokemonSalvaje;
    cocos2d::Label* labelInfoJugador;
    cocos2d::Label* labelInfoSalvaje;
    cocos2d::Menu* menuAcciones;
    cocos2d::Menu* panelAtaques;

    void setupInterface();
    void actualizarInfoPokemon();
    void finalizarBatalla(bool capturado = false);
    void crearBotonesAtaque();
    void ejecutarAtaque(const std::string& nombreAtaque);
    void realizarAtaqueSalvaje();

    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void cambiarPokemon(int index);
    cocos2d::EventListenerKeyboard* keyboardListener;
};

#endif
