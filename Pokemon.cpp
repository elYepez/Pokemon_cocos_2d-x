#include "Pokemon.h"

// Constructor


Pokemon::Pokemon()
    : nombre(""), nivel(1), vidaMaxima(10), vidaActual(10), ataque(5), defensa(5), velocidad(5), descripcion("") {
    sprite = nullptr;  
}


Pokemon::Pokemon(const std::string& nombre, int nivel, int vidaMaxima, int ataque, int defensa, int velocidad, const std::string& spriteFile, const std::string& descripcion)
    : nombre(nombre), nivel(nivel), vidaMaxima(vidaMaxima), vidaActual(vidaMaxima), ataque(ataque), defensa(defensa), velocidad(velocidad), descripcion(descripcion) {

    sprite = cocos2d::Sprite::create(spriteFile);
    nombreSprite = spriteFile;

    if (nombre == "Bulbasaur") {
        ataques = {
            {"Madrazo",40,100,"Normal"},
            {"Enredadera",45,100,"Planta"}
        };
    }
    else if (nombre == "Charmander") {
        ataques = {
            {"Zarpazo",40,100,"Normal"},
            {"Quemadura",45,100,"Fuego"}
        };
    }
    else if (nombre == "Squirtle") {
        ataques = {
            {"Moridida",40,100,"Normal"},
            {"Water Gun",45,100,"Agua"},
            {"Salpicar",42,100,"Agua"},

        };
    }


    //Pokemones salvajes

    else if (nombre == "Caterpie") {
        ataques = {
            {"Picotazo",40,100,"Bicho"},
            {"Placaje",50,100,"Normal"}
        };
    }
    else if (nombre == "Pidgey") {
        ataques = {
            {"Tornado",40,100,"Volador"},
            {"Ataque Ala",60,100,"Volador"}
        };
    }
    else if (nombre == "Oddish") {
        ataques = {
            {"Absorber",20,100,"Planta"},
            {"Acido",40,100,"Veneno"}
        };
    }

}


Pokemon::~Pokemon() {
    if (sprite) {
        sprite->autorelease();
        sprite = nullptr;
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

    if (rand() % 100 >= eleccion->precision) {
        return 0;
    }

    float factorNivel = (2.0f * nivel + 10) / 250.0f;
    float factorDefensaEnemigo = float(ataque) / float(enemigo->getDefensa());
    float danioBase = (factorNivel * factorDefensaEnemigo * eleccion->poder + 2);

    float multiplicadorRandom = (85 + (rand() % 16)) / 100.0f;

    int danioFinal = static_cast<int>(danioBase * multiplicadorRandom);

    enemigo->recibirDanio(danioFinal);

    return danioFinal;

}
