#ifndef __INICIO_ESCENA_H__
#define __INICIO_ESCENA_H__

#include "cocos2d.h"
#include "Pokemon.h" 
#include "PokemonFase0.h"
#include <vector>
#include "ui/CocosGUI.h"
#include "AudioManager.h"

//class Pokemon;

class InicioEscena : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();

    virtual bool init(); 

    CREATE_FUNC(InicioEscena);


    // metodo de callback para seleccionar un Pokémon
    void seleccionarPokemon(cocos2d::Ref* sender, const std::string& pokemonNombre);

private:
    AudioManager* audioManager;

    cocos2d::Sprite* cuadroSeleccionador;
    int seleccionActual;
    cocos2d::ui::Button* botonConfirmar;
    std::vector<Pokemon*> pokemonesIniciales;


    void setupUI();
    void crearIniciales(); //creamos los 3 pokemones iniciales
    void acutalizarEleccion(int numero); 
    void guardarEleccion();

    //sender funciona como un auto
    void PokemonSeleccionado(cocos2d::Ref* sender, int indicePokemon);
    void PokemonEleccionFinal(cocos2d::Ref* sender);
    
    cocos2d::Label* pokemonEtiquetaNombre;
    cocos2d::Label* pokemonEtiquetaEstadisticas;
    cocos2d::Sprite* pokemonSprites[3];
};

#endif
