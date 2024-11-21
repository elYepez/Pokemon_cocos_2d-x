#include "PokemonFase0.h"
#include "cocos2d.h"
#include "Mapa1.h"
#include "Mapa2.h"
#include "Mapa3.h"
#include "Pokemon.h"

//mal tp de regreso, lo coloca hasta abajo cuando debe ser arriba en el mapa_0

USING_NS_CC;

float escala = 2.0f;
Sprite* fondo;
float velocidad = 1 * escala;
bool viene_de_arriba = false; //Correcta aparicion del personaje


// Implementación del método createScene
cocos2d::Scene* PokemonFase0::createScene() {
    return PokemonFase0::create(); // Llama a la macro CREATE_FUNC
    
}



void PokemonFase0::setupCamera() {
    cameraNode = Node::create();
    this->addChild(cameraNode);

    auto gameLayer = Node::create();
    cameraNode->addChild(gameLayer);

    //mover lo que quiero que se mueva 
    
    mapaActual->retain();
    mapaActual->removeFromParent();
    gameLayer->addChild(mapaActual, -1);//cargamos el mapa


    personaje->retain();
    personaje->removeFromParent();
    gameLayer->addChild(personaje, 0);//agregamos el personaje 
    

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto camera = Follow::create(personaje, Rect::ZERO);
    cameraNode->runAction(camera);

    fondo->retain();
    fondo->removeFromParent();
    gameLayer->addChild(fondo, -2);
    
   
}

// Implementación del método init
bool PokemonFase0::init() {
    if (!Scene::init()) {
        return false;
    }

    
    

    // Inicializamos el mapa, en este caso, Mapa1
    mapaActual = new Mapa1();  // Aquí puedes usar Mapa1, Mapa2, etc.
    mapaActual->cargarFondo();  // Cargar el fondo del mapa
    this->addChild(mapaActual);  // Agregar el mapa a la escena



    tiles = 32;
    //inicializarPokemones();
    //agregarFondo();

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("entrenador.plist");


    personaje = Sprite::createWithSpriteFrameName("entrenador_caballo_abajo-0.png");
    personaje->setPosition(Director::getInstance()->getVisibleSize() / 2);
    this->addChild(personaje);
    personaje->setVisible(true);



    



    // Almacena el sprite en un miembro de la clase
    //this->personaje = personaje;

    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(PokemonFase0::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(PokemonFase0::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // Agregar el método update para el movimiento
    this->schedule(CC_SCHEDULE_SELECTOR(PokemonFase0::update)); // Programa el método update

    // Aquí puedes inicializar la escena
    
    inicializarElementos();

    agregarFondo();

    escalarTodo();

    setupCamera();
    
    return true;
}

void PokemonFase0::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
    keysPressed.insert(keyCode); // Agrega la tecla al conjunto
}

// Manejo de teclas liberadas
void PokemonFase0::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
    keysPressed.erase(keyCode); // Elimina la tecla del conjunto
}

bool animacionActiva = false;
bool isPressed = false;





// Método update que se llama en cada frame
void PokemonFase0::update(float dt) {
    Vec2 position = personaje->getPosition(); 
    
    if (keysPressed.empty()) {//Ver si se deja de presionar
        personaje->stopAllActions(); //Reseteamos acciones
        if (!animacionActiva) {
           personaje->setSpriteFrame("entrenador_camina_abajo-1.png");
        }
        isPressed = false;
        animacionActiva = false;
        personaje->setPosition(position);
        personaje->setVisible(true);
        
        return;
    }
    
    int xMapa = static_cast<int>(position.x / (escala * tiles));
    int yMapa = static_cast<int>(position.y / (escala * tiles));
    //CCLOG("Posición del personaje: (%f, %f)", position.x, position.y);
    

    // Actualiza la posición del personaje según las teclas presionadas
    if (keysPressed.count(EventKeyboard::KeyCode::KEY_A)) {
        int xNuevoMapa = static_cast<int>(((position.x - velocidad * escala) / escala) / tiles);
        
        if (puedeMoverse(xNuevoMapa,19 - yMapa)) {
            position.x -= velocidad;
        }
        if (animacionActiva == false) {
            crearAnimacionCaminarIzquierda();
            animacionActiva = true;

        }
        isPressed = true;
    }

    else if (keysPressed.count(EventKeyboard::KeyCode::KEY_D)) {
        int xNuevoMapa = static_cast<int>(((position.x + velocidad * escala) / escala) / tiles);
        if (puedeMoverse(xNuevoMapa,19 - yMapa)) {
            position.x += velocidad;
        }
        if (animacionActiva == false) {
            crearAnimacionCaminarDerecha();
            animacionActiva = true;
            
        }
        isPressed = true;
    }
    else if (keysPressed.count(EventKeyboard::KeyCode::KEY_W)) {
        int yNuevoMapa = static_cast<int>(((position.y + velocidad * escala) / escala) / tiles);
        if (puedeMoverse(xMapa, 19 - yNuevoMapa)) {
            position.y += velocidad ;
        }
       
        if (animacionActiva == false) {
            crearAnimacionCaminarArriba();
            animacionActiva = true;

        }
        isPressed = true;
       
    }
    else if (keysPressed.count(EventKeyboard::KeyCode::KEY_S)) {
        int yNuevoMapa = static_cast<int>(((position.y - velocidad * escala) / escala) / tiles);
        if (puedeMoverse(xMapa,19 - yNuevoMapa)) {
            position.y -= velocidad;
        }
       
        if (animacionActiva == false) {
            crearAnimacionCaminarAbajo();
            animacionActiva = true;

        }
        isPressed = true;
        //personaje->setTexture("entrenador_camina_abajo-1.png");

        
    }
    
    float nuevaPosX = clampf(position.x, tiles / 2, (12 * tiles * escala) - tiles / 2);
    float nuevaPosY = clampf(position.y, tiles / 2, (20 * tiles * escala) - tiles / 2);


    int valorCelda = mapaActual->obtenerValorEnPosicion(xMapa, 19 - yMapa);
    if (valorCelda == 17) {
        cambiarMapa("mapa_dos");  // Cambiar al mapa superior
        return;
    }
    else if (valorCelda == 16) {
        cambiarMapa("mapa_uno");  // Cambiar al mapa inferior
        return;
    }
    else if (valorCelda == 15) {
        cambiarMapa("mapa_cero");
    }


    personaje->setPosition(nuevaPosX, nuevaPosY); // Actualiza la posición del personaje

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 cameraPosition = personaje->getPosition() - Vec2(visibleSize.width / 2, visibleSize.height / 2);
    
    
    //mapaActual->actualizar(dt);



}

void PokemonFase0::crearAnimacionCaminarDerecha() {
    Vector<SpriteFrame*> frames;
    frames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("entrenador_camina_derecha-0.png"));
    frames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("entrenador_camina_derecha-1.png"));
    frames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("entrenador_camina_derecha-2.png"));
    // Agrega más frames según tu animación

    if (frames.size() > 0) {
        Animation* animation = Animation::createWithSpriteFrames(frames, 0.1f); // 0.1f es el tiempo entre frames
        Animate* animate = Animate::create(animation);
        personaje->runAction(RepeatForever::create(animate)); // Repite la animación
    }
    else {
        animacionActiva = false;
        CCLOG("No se encontraron frames para la animación.");
    }
}


void PokemonFase0::crearAnimacionCaminarIzquierda() {
    Vector<SpriteFrame*> frames;
    frames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("entrenador_camina_izquierda-2.png"));
    frames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("entrenador_camina_izquierda-1.png"));
    frames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("entrenador_camina_izquierda-0.png"));
    // Agrega más frames según tu animación

    if (frames.size() > 0) {
        Animation* animation = Animation::createWithSpriteFrames(frames, 0.1f); // 0.1f es el tiempo entre frames
        Animate* animate = Animate::create(animation);
        personaje->runAction(RepeatForever::create(animate)); // Repite la animación
    }
    else {
        animacionActiva = false;
        CCLOG("No se encontraron frames para la animación.");
    }
}

void PokemonFase0::crearAnimacionCaminarArriba() {
    Vector<SpriteFrame*> frames;
    frames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("entrenador_camina_arriba-0.png"));
    frames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("entrenador_camina_arriba-1.png"));
    frames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("entrenador_camina_arriba-2.png"));
    // Agrega más frames según tu animación

    if (frames.size() > 0) {
        Animation* animation = Animation::createWithSpriteFrames(frames, 0.1f); // 0.1f es el tiempo entre frames
        Animate* animate = Animate::create(animation);
        personaje->runAction(RepeatForever::create(animate)); // Repite la animación
    }
    else {
        animacionActiva = false;
        CCLOG("No se encontraron frames para la animación.");
    }
}

void PokemonFase0::crearAnimacionCaminarAbajo() {
    Vector<SpriteFrame*> frames;
    frames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("entrenador_camina_abajo-0.png"));
    frames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("entrenador_camina_abajo-1.png"));
    frames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("entrenador_camina_abajo-2.png"));
    // Agrega más frames según tu animación

    if (frames.size() > 0) {
        Animation* animation = Animation::createWithSpriteFrames(frames, 0.1f); // 0.1f es el tiempo entre frames
        Animate* animate = Animate::create(animation);
        personaje->runAction(RepeatForever::create(animate)); // Repite la animación
    }
    else {
        animacionActiva = false;
        CCLOG("No se encontraron frames para la animación.");
    }
}

// Implementación de otros métodos
void PokemonFase0::agregarFondo() {
    fondo = Sprite::create("fondo_arboles_fondo_0.png");

    if (fondo == nullptr) {
        CCLOG("NO SE CARGO EL MAPA");
        return;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();

    fondo->setScale(escala);
    
    float centroX = (12 * tiles * escala) / 2;
    float centroY = (20 * tiles * escala) / 2;

    
    fondo->setPosition(centroX, centroY);
    

    // Agregar el fondo a la escena
    this->addChild(fondo, -2);  // El valor -1 asegura que el fondo esté debajo de otros elementos como el personaje

}

void PokemonFase0::cambiarMapa(const std::string& direccion) {
    CCLOG("Cambiando de mapa hacia %s...", direccion.c_str());

    // Elimina el mapa actual
    if (mapaActual != nullptr) {
        this->removeChild(mapaActual);
        mapaActual->release();
    }

    float previosY = personaje->getPositionY();

    // Cargar un nuevo mapa basado en la dirección
    if (direccion == "mapa_uno") {
        mapaActual = new Mapa2(); // Carga el mapa superior
        viene_de_arriba = true;
    }
    else if (direccion == "mapa_dos") {
        mapaActual = new Mapa3(); // Carga el mapa inferior
    }
    else if (direccion == "mapa_cero") {
        mapaActual = new Mapa1();
        //viene_de_arriba = false;
    }

    // Asegurarse de cargar el nuevo mapa
    if (mapaActual != nullptr) {
        mapaActual->cargarFondo();
        this->addChild(mapaActual);
        mapaActual->setScale(escala);
    }

    // Ajustar la posición inicial del personaje según la dirección
    if (direccion == "mapa_uno") {
        CCLOG("Entrando a Mapa 1 desde Mapa 0...");
        personaje->setPosition(Vec2(tiles * escala * 6, tiles * escala * 2)); // Aparece en la entrada del nuevo mapa
    }
    else if (direccion == "mapa_dos") {
        CCLOG("Entrando a Mapa 2 desde Mapa 0...");
        personaje->setPosition(Vec2(tiles * escala * 6, tiles * escala * 18)); // Aparece en la entrada del nuevo mapa
    }
    else if (direccion == "mapa_cero") {
        if (viene_de_arriba) {
            CCLOG("Personaje colocado en (6, 1) en Mapa 0 (desde abajo).");
            personaje->setPosition(Vec2(tiles*escala * 6, tiles*escala * 3));
            

        }
        else {
            
            personaje->setPosition(Vec2((tiles)*escala * 6, (tiles)*escala * 18)); // Aparece en la entrada del nuevo mapa
            CCLOG("Personaje colocado en (6, 18) en Mapa 0 (desde arriba).");
        }
       
        
    }

    // Reconfigurar la cámara
    setupCamera();
}



bool PokemonFase0::puedeMoverse(int x, int y) {
 
    if (x < 0 || x >= 11 || y < 0 || y >= 20) {
        return false;  // Si las coordenadas están fuera del mapa, no se puede mover
    }

    int valorCelda = mapaActual->obtenerValorEnPosicion(x, y);
    CCLOG("Revisando celda (%d, %d) - Valor: %d", x, y, valorCelda);
    return valorCelda != 1;  // Si la celda es 1 (arboles), no se puede mover
}

void PokemonFase0::inicializarElementos() {
    // Lógica para inicializar elementos de la escena
}

void PokemonFase0::inicializarPokemones() {
    //Pokemon* balbasaur = new Pokemon("Balbasaur", 5, 35, 555, 40, 90, "bulbasaur.png");

    //balbasaur->getSprite()->setPosition(Vec2(100, 200));

   // this->addChild(balbasaur->getSprite());
}

void PokemonFase0::escalarTodo() {
   personaje->setScale(escala);
   mapaActual->setScale(escala);
   fondo->setScale(escala);
   //tiles *= escala;
}
