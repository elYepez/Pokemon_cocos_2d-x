#include "BatallaPokemon.h"
#include "ui/CocosGUI.h"
#include "cocos2d.h"
#include <vector>

USING_NS_CC;

BatallaPokemon* BatallaPokemon::create(Pokemon* pokemonJugador, Pokemon* pokemonSalvaje, cocos2d::Vector<Pokemon*> galeriaPokemones) {
    BatallaPokemon* scene = new BatallaPokemon();
    if (scene && scene->init(pokemonJugador, pokemonSalvaje, galeriaPokemones)) {
        scene->autorelease();
        return scene;
    }
    CC_SAFE_DELETE(scene);
    return nullptr;
}

bool BatallaPokemon::init(Pokemon* pokemonJugador, Pokemon* pokemonSalvaje, cocos2d::Vector<Pokemon*> galeriaPokemones) {
    if (!Scene::init()) {
        return false;
    }

    audioManager = AudioManager::getInstance();

    //AudioManager::getInstance()->playBackgroundMusic("Batalla.mp3", false);
    this->pokemonJugador = pokemonJugador;
    this->pokemonSalvaje = pokemonSalvaje;
    this->galeriaPokemones = galeriaPokemones;
    pokemonSalvaje->sanar();

    setupInterface();

    return true;
}

void BatallaPokemon::setupInterface() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // fondo de batalla
    auto panelBatalla = Sprite::create("fondo_batalla.png");  // Necesitarás crear este sprite
    panelBatalla->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    this->addChild(panelBatalla);


    // sprites

    spritePokemonJugador = Sprite::create(pokemonJugador->getSpriteNombre());
    spritePokemonJugador->setScale(2.3f);
    spritePokemonJugador->setPosition(Vec2(2 * visibleSize.width / 9, visibleSize.height / 9));
    this->addChild(spritePokemonJugador);
    

    spritePokemonSalvaje = Sprite::create(pokemonSalvaje->getSpriteNombre());
    spritePokemonSalvaje->setScale(1.3f);
    spritePokemonSalvaje->setPosition(Vec2(9 * visibleSize.width / 11, 4* visibleSize.height / 10));
    this->addChild(spritePokemonSalvaje);

    // datos de los pokemones
    labelInfoJugador = Label::createWithTTF(pokemonJugador->getNombre() + " Niv: " + std::to_string(pokemonJugador->getNivel()),
        "fonts/arial.ttf", 22);
    labelInfoJugador->setPosition(Vec2(2 * visibleSize.width / 9, 3 *visibleSize.height / 11));
    this->addChild(labelInfoJugador);

    labelInfoSalvaje = Label::createWithTTF(pokemonSalvaje->getNombre() + "  Niv: " +
        std::to_string(pokemonSalvaje->getNivel()), "fonts/arial.ttf", 20);
    labelInfoSalvaje->setPosition(Vec2(7 * visibleSize.width / 9 ,3 * visibleSize.height / 11));
    labelInfoSalvaje->setTextColor(Color4B::BLACK);
    this->addChild(labelInfoSalvaje);

    
    //botones de ataque
    auto btnAtaque = MenuItemSprite::create(
        //Label::createWithTTF("Atacar", "fonts/arial.ttf", 20),
        Sprite::create("botonNoPresionado.png"),
        Sprite::create("botonSiPresionado.png"),
        CC_CALLBACK_1(BatallaPokemon::onAtaquePressed, this)
    );

    auto btnCapturar = MenuItemSprite::create(
        //Label::createWithTTF("Atacar", "fonts/arial.ttf", 20),
        Sprite::create("botonNoPresionado.png"),
        Sprite::create("botonSiPresionado.png"),
        CC_CALLBACK_1(BatallaPokemon::onCapturarPressed, this)
    );

    auto btnHuir = MenuItemSprite::create(
        //Label::createWithTTF("Atacar", "fonts/arial.ttf", 20),
        Sprite::create("botonNoPresionado.png"),
        Sprite::create("botonSiPresionado.png"),
        CC_CALLBACK_1(BatallaPokemon::onHuirPressed, this)
    );

    auto textoAtaque = Label::createWithTTF("Atacar", "fonts/arial.ttf", 20);
    textoAtaque->setTextColor(Color4B::BLACK);
    btnAtaque->addChild(textoAtaque);
    textoAtaque->setPosition(btnAtaque->getContentSize() / 2);

    auto textoCapturar = Label::createWithTTF("Capturar", "fonts/arial.ttf", 20);
    textoCapturar->setTextColor(Color4B::BLACK);
    btnCapturar->addChild(textoCapturar);
    textoCapturar->setPosition(btnCapturar->getContentSize() / 2);


    auto textoHuir = Label::createWithTTF("Huir", "fonts/arial.ttf", 20);
    textoHuir->setTextColor(Color4B::BLACK);
    btnHuir->addChild(textoHuir);
    textoHuir->setPosition(btnHuir->getContentSize() / 2);

    menuAcciones = Menu::create(btnAtaque, btnCapturar, btnHuir, nullptr);
    //menuAcciones->setColor(Color3B::BLACK);
    menuAcciones->alignItemsVerticallyWithPadding(0.2f);
    menuAcciones->setPosition(Vec2(7 * visibleSize.width / 9, 1 * visibleSize.height / 10));
    this->addChild(menuAcciones);

    // panel de ataques
    panelAtaques = Menu::create();
    panelAtaques->setPosition(Vec2(7 * visibleSize.width / 9, 1 * visibleSize.height / 10));
    this->addChild(panelAtaques);
    panelAtaques->setVisible(false);
    
    // crear botones para cada ataque
    crearBotonesAtaque();

    keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(BatallaPokemon::onKeyPressed, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);


}

void BatallaPokemon::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
    audioManager->playEffect("Seleccion.mp3");
    int index = -1;

    if (keyCode >= EventKeyboard::KeyCode::KEY_0 && keyCode <= EventKeyboard::KeyCode::KEY_9) {
        index = static_cast<int>(keyCode) - static_cast<int>(EventKeyboard::KeyCode::KEY_0);
        cambiarPokemon(index);
    }
}

void BatallaPokemon::cambiarPokemon(int index) {
    // Verificar si el índice es valido
    if (index >= 0 && index < galeriaPokemones.size()) {
        // Guardar la vida actual del pokemon actual
        int vidaActual = pokemonJugador->getVidaActual();

        // Cambiar al nuevo pokemon
        pokemonJugador = galeriaPokemones.at(index);

        // Actualizar el sprite
        removeChild(spritePokemonJugador);
        spritePokemonJugador = Sprite::create(pokemonJugador->getSpriteNombre());
        spritePokemonJugador->setScale(2.3f);
        spritePokemonJugador->setPosition(Vec2(2 * Director::getInstance()->getVisibleSize().width / 9,
            Director::getInstance()->getVisibleSize().height / 9));
        addChild(spritePokemonJugador);

        // Actualizar la información en pantalla
        actualizarInfoPokemon();

        // Recrear los botones de ataque con los nuevos movimientos
        panelAtaques->removeAllChildren();
        crearBotonesAtaque();
    }
}


void BatallaPokemon::crearBotonesAtaque() {
    const auto& ataques = pokemonJugador->ataques;
    float padding = 10.0f;
    float posY = -30.0f;

    for (const auto& ataque : ataques) {
        auto btnAtaque = MenuItemSprite::create(
            Sprite::create("botonNoPresionado.png"),
            Sprite::create("botonSiPresionado.png"),
            [this, ataque](Ref* sender) {
                ejecutarAtaque(ataque.nombre);
            }
        );

        auto textoAtaque = Label::createWithTTF(ataque.nombre, "fonts/arial.ttf", 20);
        textoAtaque->setTextColor(Color4B::BLACK);
        btnAtaque->addChild(textoAtaque);
        textoAtaque->setPosition(btnAtaque->getContentSize() / 2);

        btnAtaque->setPosition(0, posY);
        panelAtaques->addChild(btnAtaque);
        posY += btnAtaque->getContentSize().height + padding;
    }


    //Boton regresar
    auto btnRegresar = MenuItemSprite::create(
        Sprite::create("botonNoPresionado.png"),
        Sprite::create("botonSiPresionado.png"),
        CC_CALLBACK_1(BatallaPokemon::mostrarMenuPrincipal, this)
    );
    auto textoRegresar = Label::createWithTTF("Regresar", "fonts/arial.ttf", 20);
    textoRegresar->setTextColor(Color4B::BLACK);
    btnRegresar->addChild(textoRegresar);
    textoRegresar->setPosition(btnRegresar->getContentSize() / 2);
    btnRegresar->setPosition(-215, 157);
    panelAtaques->addChild(btnRegresar);
}

void BatallaPokemon::onAtaquePressed(Ref* sender) {
    audioManager->playEffect("Seleccion.mp3");
    menuAcciones->setVisible(false);
    panelAtaques->setVisible(true);
}

void BatallaPokemon::mostrarMenuPrincipal(Ref* sender) {
    audioManager->playEffect("Seleccion.mp3");
    panelAtaques->setVisible(false);
    menuAcciones->setVisible(true);
}

void BatallaPokemon::ejecutarAtaque(const std::string& nombreAtaque) {
    int danio = pokemonJugador->usarMovimiento(pokemonSalvaje, nombreAtaque);

    // Animacion de ataque
    auto moveRight = MoveBy::create(0.2f, Vec2(20, 0));
    auto moveBack = moveRight->reverse();
    auto sequence = Sequence::create(moveRight, moveBack, nullptr);
    spritePokemonJugador->runAction(sequence);

    // Mostrar daño
    auto labelDanio = Label::createWithTTF("-" + std::to_string(danio), "fonts/arial.ttf", 24);
    labelDanio->setPosition(spritePokemonSalvaje->getPosition() + Vec2(0, 30));
    this->addChild(labelDanio);

    // Animacion del texto de daño
    labelDanio->runAction(Sequence::create(
        MoveBy::create(1.0f, Vec2(0, 20)),
        FadeOut::create(0.5f),
        RemoveSelf::create(),
        nullptr
    ));

    actualizarInfoPokemon();

    // Verificar si el Pokemon salvaje fue derrotado
    if (pokemonSalvaje->getVidaActual() <= 0) {
        finalizarBatalla(false);
    }
    else {
        // Turno del Pokemon salvaje
        realizarAtaqueSalvaje();
    }

    mostrarMenuPrincipal(nullptr);
}

void BatallaPokemon::realizarAtaqueSalvaje() {

    const auto& ataques = pokemonSalvaje->ataques;
    int ataqueAleatorio = rand() % ataques.size();
    int danio = pokemonSalvaje->usarMovimiento(pokemonJugador, ataques[ataqueAleatorio].nombre);

    // Animacion similar a la del jugador
    auto moveLeft = MoveBy::create(0.2f, Vec2(-20, 0));
    auto moveBack = moveLeft->reverse();
    auto sequence = Sequence::create(moveLeft, moveBack, nullptr);
    spritePokemonSalvaje->runAction(sequence);

    // Mostrar daño
    auto labelDanio = Label::createWithTTF("-" + std::to_string(danio), "fonts/arial.ttf", 24);
    labelDanio->setPosition(spritePokemonJugador->getPosition() + Vec2(0, 30));
    labelDanio->setTextColor(Color4B::RED);
    this->addChild(labelDanio);

    labelDanio->runAction(Sequence::create(
        MoveBy::create(1.0f, Vec2(0, 20)),
        FadeOut::create(0.5f),
        RemoveSelf::create(),
        nullptr
    ));

    actualizarInfoPokemon();

    if (pokemonJugador->getVidaActual() <= 0) {
        finalizarBatalla(false);
    }
}

void BatallaPokemon::onCapturarPressed(Ref* sender) {
    
    // Probabilidad de captura basada en la vida restante del Pokémon salvaje
    float probabilidad = (1.0f - (pokemonSalvaje->getVidaActual() / pokemonSalvaje->getVidaMaxima())) * 0.7f;
    float random = static_cast<float>(rand()) / RAND_MAX;

    if (random < probabilidad) {
        // exito en la captura
        CCLOG("Pokemon capturado!");
        audioManager->playEffect("Atrapado.mp3");
        finalizarBatalla(true);
    }
    else {
        // fallo en la captura
        // mostrar mensaje de fallo
        auto label = Label::createWithTTF("El Pokemon se ha escapado", "fonts/arial.ttf", 24);
        audioManager->playEffect("Seleccion.mp3");
        label->setPosition(Director::getInstance()->getVisibleSize() / 2);
        this->addChild(label);
        label->runAction(Sequence::create(
            FadeIn::create(0.5f),
            DelayTime::create(1.0f),
            FadeOut::create(0.5f),
            RemoveSelf::create(),
            nullptr
        ));
    }
}

void BatallaPokemon::onHuirPressed(Ref* sender) {
    audioManager->playEffect("Seleccion.mp3");
    finalizarBatalla();
}

void BatallaPokemon::actualizarInfoPokemon() {
    // Actualizar las etiquetas con la información actual de los Pokémon
    labelInfoJugador->setString(pokemonJugador->getNombre() + " Niv: " +
        std::to_string(pokemonJugador->getNivel()) + "\nVida: " +
        std::to_string(pokemonJugador->getVidaActual()));

    labelInfoSalvaje->setString(pokemonSalvaje->getNombre() +
        " Niv: " + std::to_string(pokemonSalvaje->getNivel()) +
        "\nVida: " + std::to_string(pokemonSalvaje->getVidaActual()));
}

void BatallaPokemon::finalizarBatalla(bool capturado) {
    _eventDispatcher->removeEventListener(keyboardListener);
    if (capturado && onPokemonCapturado) {
        CCLOG("llamando el callback exitosamente");
        onPokemonCapturado(pokemonSalvaje);
    }
    //audioManager->playBackgroundMusic("Fondo_1.mp3");
    AudioManager::getInstance()->restorePreviousMusic();
    
    Director::getInstance()->popScene();
}
