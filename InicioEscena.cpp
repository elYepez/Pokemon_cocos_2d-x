#include "InicioEscena.h"
#include "ui/CocosGUI.h"
#include "Pokemon.h"
#include <cocos2d.h>

USING_NS_CC;


Scene* InicioEscena::createScene() {
	return InicioEscena::create();
}

bool InicioEscena::init() {
	if (!Scene::init()) {
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto fondo = Sprite::create("fondoInicioEscena.png");
	fondo->setPosition(Point((visibleSize.width / 2), visibleSize.height / 2));
	this->addChild(fondo, 0);

	audioManager = AudioManager::getInstance();

	crearIniciales();
	setupUI();

	

	return true;
}

void InicioEscena::crearIniciales() {

	pokemonesIniciales = {

		new Pokemon("Bulbasaur", 5, 45, 49, 49, 45, "bulbasaur.png",
				   "Tras nacer, crece alimentandose durante un tiempo de los nutrientes que contiene el bulbo de su lomo."),//https://www.pokemon.com/el/pokedex/bulbasaur

		new Pokemon("Charmander", 5, 39, 52, 43, 65, "charmander.png",
				   "La llama de su cola indica su fuerza vital. Si esta debil, la llama ardera mas tenue"),
				   //https://www.pokemon.com/el/pokedex/charmander

		new Pokemon("Squirtle", 5, 44, 48, 65, 43, "squirtle.png",
				   "Tras nacer, se le hincha el lomo y se le forma un caparazon. Escupe poderosa espuma por la boca.")//https://www.pokemon.com/el/pokedex/squirtle

	};
}

void InicioEscena::setupUI() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto titleLabel = Label::createWithTTF("Escoge tu primer pokemon!!!", "fonts/Marker Felt.ttf", 24);
	titleLabel->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - titleLabel->getContentSize().height));
	this->addChild(titleLabel, 1);


	float spacing = visibleSize.width / 4;
	float yPos = visibleSize.height * 0.50f;

	for (int i = 0; i < 3; i++) {
		pokemonSprites[i] = pokemonesIniciales[i]->getSprite();
		pokemonSprites[i]->setPosition(Vec2(spacing * (i + 1), yPos));
		pokemonSprites[i]->setScale(1.4f);

		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = [this, i](Touch* touch, Event* event) {
			Vec2 touchPos = touch->getLocation();
			if (pokemonSprites[i]->getBoundingBox().containsPoint(touchPos)) {
				audioManager->playEffect("Seleccion.mp3");
				acutalizarEleccion(i); //checar
				return true;
				}
			return false;

			};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, pokemonSprites[i]);
		this->addChild(pokemonSprites[i], 1);
	}

	//cuadro indicador de seleccion
	cuadroSeleccionador = Sprite::create("seleccionado.png");
	cuadroSeleccionador->setScale(1.2f);
	cuadroSeleccionador->setVisible(false);
	this->addChild(cuadroSeleccionador, 2);


	pokemonEtiquetaNombre = Label::createWithTTF("click en algun \npokemon*", "fonts/Marker Felt.ttf", 20);
	pokemonEtiquetaNombre->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.83f));
	pokemonEtiquetaNombre->setAlignment(TextHAlignment::CENTER);
	this->addChild(pokemonEtiquetaNombre, 1);

	pokemonEtiquetaEstadisticas = Label::createWithTTF("Comienza la aventura...", "fonts/Marker Felt.ttf", 16);
	pokemonEtiquetaEstadisticas->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.3f));
	pokemonEtiquetaEstadisticas->setAlignment(TextHAlignment::CENTER);
	this->addChild(pokemonEtiquetaEstadisticas, 1);


	//ui::Button* botonConfirmar;
	botonConfirmar = ui::Button::create("Seleccionar.png", "SeleccionarNegado.png");
	//botonConfirmar->setTitleText("ESCOGE ESTE POKEMON!");
	botonConfirmar->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.15f));
	botonConfirmar->addClickEventListener(CC_CALLBACK_1(InicioEscena::PokemonEleccionFinal, this));
	botonConfirmar->setVisible(false);
	this->addChild(botonConfirmar, 1);
}

void InicioEscena::acutalizarEleccion(int numero) {
	seleccionActual = numero;
	Pokemon* seleccionado = pokemonesIniciales[numero];

	CCLOG("Nivel: %d", seleccionado->getNivel());
	CCLOG("Vida: %d", seleccionado->getVidaMaxima());
	CCLOG("Ataque: %d", seleccionado->getAtaque());
	CCLOG("Defensa: %d", seleccionado->getDefensa());
	CCLOG("Velocidad: %d", seleccionado->getVelocidad());
	CCLOG("Descripcion: %s", seleccionado->getDescripcion().c_str());

	cuadroSeleccionador->setPosition(pokemonSprites[numero]->getPosition());
	cuadroSeleccionador->setVisible(true);
	botonConfirmar->setVisible(true);
	

	pokemonEtiquetaNombre->setString(seleccionado->getNombre());

	std::string estadisticas = "Nivel: " + std::to_string(seleccionado->getNivel()) + "\n" +
		"Vida MAX: " + std::to_string(seleccionado->getVidaMaxima()) + "\n" +
		"Ataque: " + std::to_string(seleccionado->getAtaque()) + "\n" +
		"Defensa: " + std::to_string(seleccionado->getDefensa()) + "\n" +
		"Velocidad: " + std::to_string(seleccionado->getVelocidad());
		//"\n\n" + seleccionado->getDescripcion();

	pokemonEtiquetaEstadisticas->setString(estadisticas);
}

void InicioEscena::PokemonEleccionFinal(Ref* sender) {
	audioManager->playEffect("Seleccion_0.mp3");

	Pokemon* pokemonInicial = pokemonesIniciales[seleccionActual];

	auto scene = PokemonFase0::createScene(pokemonInicial);
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, scene));
}
