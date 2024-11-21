#include "Pokemon.h"

// Constructor
Pokemon::Pokemon(std::string& nombre, int nivel, int vidaMaxima, int ataque, int defensa, int velocidad, const std::string& spriteFile, const std::string& descripcion)
    : nombre(nombre), nivel(nivel), vidaMaxima(vidaMaxima), vidaActual(vidaMaxima), ataque(ataque), defensa(defensa), velocidad(velocidad), descripcion(descripcion) {

    sprite = cocos2d::Sprite::create(spriteFile);

    if (nombre == "Bulbasaur") {
        ataques = {
            {"Madrazo",40,100,"Normal"},
            {"Enredadera",45,100,"Planta"}
        };
    }
    else if (nombre == "Charmander") {
        ataques = {
            {"Zarpazo",40,100,"Normal"},
            {"Quemadura",45,100,"Planta"}
        };
    }
    else if (nombre == "Squirtle") {
        ataques = {
            {"Madrazo",40,100,"Normal"},
            {"Water Gun",45,100,"Planta"}
        };
    }

}


Pokemon::~Pokemon() {
    if (sprite) {
        sprite->release();
    }
}

void Pokemon::recibirDanio(int danio) {
    vidaActual = std::max(0, vidaActual - danio);
}

void Pokemon::curar(int cantidadCura) {
    vidaActual = std::min(vidaMaxima, vidaActual + cantidadCura);
}

void Pokemon::aprenderAtaque(const Ataque& ataque) {
    if (ataques.size() < 4) {
        ataques.push_back(ataque);
    }
}

void Pokemon::levelup() {
    nivel++;
    vidaMaxima += 2 + (rand() % 3);
    ataque += 1 + (rand() % 2);
    defensa += 1 + (rand() % 2);
    velocidad += 1 + (rand() % 2);

    vidaActual = vidaMaxima; //regenera la vida completamente
}

int Pokemon::usarMovimiento(Pokemon* enemigo, const std::string& nombreAtaque) {
    Ataque* eleccion = nullptr;
    for (auto& ataque : ataques) {
        if (ataque.nombre == nombreAtaque) {
            eleccion = &ataque;
            break;
        }
    }

    if (!eleccion) return 0;

    float factorNivel = (2.0f * nivel + 10) / 250.0f;
    float factorDefensaEnemigo = float(ataque) / float(enemigo->getDefensa());
    float danioBase = (factorNivel * factorDefensaEnemigo * eleccion->poder + 2);

    float multiplicadorRandom = (85 + (rand() % 16)) / 100.0f;

    return static_cast<int>(danioBase * multiplicadorRandom);

}
