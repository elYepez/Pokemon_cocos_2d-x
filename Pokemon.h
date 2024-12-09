#ifndef __POKEMON_H__
#define __POKEMON_H__

#include "cocos2d.h"
#include <string>
#include <vector>

class Pokemon : public cocos2d::Ref{
public:
    // Constructor
    Pokemon();
    Pokemon(const std::string& nombre, int nivel, int vidaMaxima, int ataque, int defensa, int velocidad, const std::string& spriteFile, const std::string& descripcion);
    

    ~Pokemon();

    // Métodos
    void recibirDanio(int danio);
    int calcularDanio(int ataqueRival);
    bool estaDerrotado() const;
    int usarMovimiento(Pokemon* enemigo, const std::string& nombreAtaque);
    

    void levelup();
    void curar(int cantidadCura);
    void sanar() { vidaActual = vidaMaxima; } //sanar cura toda la vida

    // Getters y Setters
    std::string getNombre() const { return nombre; }

    int getNivel() const { return nivel; }

    int getVidaMaxima() const { return vidaMaxima; }

    int getVidaActual() const { return vidaActual; }

    int getAtaque() const { return ataque; }

    int getDefensa() const { return defensa; }

    int getVelocidad() const { return velocidad; }

    std::string getDescripcion() const { return descripcion; }

    std::string getSpriteNombre() const { return nombreSprite; }

    cocos2d::Sprite* getSprite() const { return sprite; };

    // ATAQUES

    struct Ataque {
        std::string nombre;
        int poder;
        int precision; //probabilidad de acertar el ataque
        std::string tipo;
    };

    std::vector<Ataque> ataques; // Movimientos disponibles
    void aprenderAtaque(const Ataque& ataque);

private:
    // Atributos
    std::string nombre;
    int nivel;
    int vidaActual;
    int vidaMaxima;
    int ataque;
    int defensa;
    int velocidad;
    std::string nombreSprite;
    
    std::string descripcion;
    cocos2d::Sprite* sprite; // Sprite del Pokémon

    void initStats();
};

#endif
