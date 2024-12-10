//
//  TrainingScene.cpp
//  Proyecto
//
//  Created by Alejandra Esparza Pelcastre 
//


#include "TrainingScene.h"
#include "GameMenu.h"
#include "GameScene.h"
#include "CharacterManager.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

Scene* TrainingScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld()->setGravity(Vec2(0, -98));
    // 'layer' is an autorelease object
    auto layer = TrainingScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool TrainingScene::init() {
    if (!Scene::init()) {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    auto backgroundSprite = Sprite::create("fondoTraining.png");
    backgroundSprite->setScale(0.8f);
    backgroundSprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(backgroundSprite);
    
    
    auto homeItem = MenuItemImage::create("home.png", "homeSelected.png",
                                          CC_CALLBACK_1(TrainingScene::GoToMenu, this));
    homeItem->setPosition(Point(origin.x + 40, origin.y + 190));
    auto menu = Menu::create(homeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);
    
    lives = 3;
    shields = 0;
    //Sprites contador de vidas
    lives3 = Sprite::create("lives_3.png");
    lives3->setPosition(Vec2(visibleSize.width - lives3->getContentSize().width / 2 - 30, visibleSize.height - lives3->getContentSize().height / 2 + 40));
    this->addChild(lives3, 1);

    lives2 = Sprite::create("lives_2.png");
    lives2->setPosition(lives3->getPosition());
    this->addChild(lives2, 1);
    lives2->setVisible(false);

    lives1 = Sprite::create("lives_1.png");
    lives1->setPosition(lives3->getPosition());
    this->addChild(lives1, 1);
    lives1->setVisible(false);
    
    initDuckAnimations();
    createActionButtons();
    loadCatAnimations();
    
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(TrainingScene::onContactBegin, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    auto mushroom = Sprite::create("mushroom.png");
    mushroom->setName("mushroom");
    mushroom->setScale(0.6);
    auto mushroomBody = PhysicsBody::createBox(mushroom->getContentSize());
    mushroomBody->setGravityEnable(false);
    mushroomBody->setContactTestBitmask(true);
    mushroom->setPhysicsBody(mushroomBody);
    mushroom->setPosition(visibleSize.width/2 + origin.x, visibleSize.height * 0.38); // Cambiar la pos
    this->addChild(mushroom);
    
    startCatAttackTimer();
    
    this->scheduleUpdate();
    return true;
}

void TrainingScene::GoToMenu(cocos2d::Ref* sender){
    //clean loaded animations
    for (const auto& action : actions) {
        //std::string plistPath = basePath + action + ".plist";
        std::string plistPath =  action + selectedDuck + ".plist";
        SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(plistPath);
    }
    Director::getInstance( )->replaceScene(TransitionFade::create( 0.5, GameMenu::createScene()));
}
void TrainingScene::GoToGameScene(cocos2d::Ref* sender){
    for (const auto& action : actions) {
        //std::string plistPath = basePath + action + ".plist";
        std::string plistPath = action + selectedDuck + ".plist";
        SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(plistPath);
    }
    Director::getInstance( )->replaceScene(TransitionFade::create( 0.5, GameScene::createScene()));
}

void TrainingScene::initDuckAnimations() {
    
    selectedDuck = CharacterManager::getInstance().getSelectedCharacter();
    
    //basePath = "Actions" + selectedDuck + "/";

    for (const auto& action : actions) {
        // Construct paths for .plist and .png
        //std::string plistPath = basePath + action + selectedDuck + ".plist";
        //std::string pngPath = basePath + action + selectedDuck + ".png";
        
        std::string plistPath = action + selectedDuck + ".plist";
        std::string pngPath = action + selectedDuck + ".png";
        // Load into the sprite frame cache
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plistPath, pngPath);
        cocos2d::log("Cargando archivo: %s", plistPath.c_str(), pngPath.c_str());
    }
    
    characterSprite = Sprite::createWithSpriteFrameName("duckee_idle1.png");
    characterSprite->setScale(8.0f);
    characterSprite->setPosition(Vec2(Director::getInstance()->getVisibleSize().width * 0.15,
                                          Director::getInstance()->getVisibleSize().height * 0.48));
    auto characterBody = PhysicsBody::createBox(characterSprite->getContentSize() * 0.8f); // Tamaño ajustado
    characterBody->setDynamic(true);
    characterBody->setGravityEnable(false);
    characterBody->setContactTestBitmask(0x01); // Máscara de contacto
    characterSprite->setPhysicsBody(characterBody);

    characterSprite->setName("duck");
    
    if (!characterSprite) {
        cocos2d::log("Error: No se pudo crear el sprite con el frame inicial.");
        return;
    }
    this->addChild(characterSprite);

}


void TrainingScene::createAnimationForAction(const std::string& action, const Vec2& position) {
    // Create a sprite for the first frame of the action
    auto sprite = Sprite::createWithSpriteFrameName("duckee_" + action + "1.png");
    sprite->setPosition(position);
    this->addChild(sprite);

    // Load all frames for the action
    Vector<SpriteFrame*> frames;
    for (int i = 1; i <= 4; ++i) { //  4 frames per action
        std::string frameName = "duckee_" + action + std::to_string(i) + ".png";
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (frame) {
            frames.pushBack(frame);
        }
    }

    // Create the animation
    auto animation = Animation::createWithSpriteFrames(frames, 0.2f);
    auto animate = Animate::create(animation);
    sprite->runAction(RepeatForever::create(animate));
}



void TrainingScene::createActionButtons() {
    // Tamaño de la pantalla
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // Crear botón para avanzar
    //auto moveButton = ui::Button::create("res/action_buttons/izquierda_btn.png");
    auto moveButton = ui::Button::create("izquierda_btn.png");
    moveButton->setPosition(Vec2(visibleSize.width * 0.8, visibleSize.height * 0.6));
    moveButton->addClickEventListener([this](Ref* sender) {
        playAnimation("roll");
    });
    this->addChild(moveButton);

    // Crear botón para saltar
    //auto jumpButton = ui::Button::create("res/action_buttons/saltar_btn.png");
    auto jumpButton = ui::Button::create("saltar_btn.png");
    jumpButton->setPosition(Vec2(visibleSize.width * 0.86, visibleSize.height * 0.75));
    jumpButton->addClickEventListener([this](Ref* sender) {
        playAnimation("jump");
    });
    this->addChild(jumpButton);

    // Crear botón para rodar
    //auto rollButton = ui::Button::create("res/buttons/derecha_btn.png");
    auto rollButton = ui::Button::create("derecha_btn.png");
    rollButton->setPosition(Vec2(visibleSize.width * 0.92, visibleSize.height * 0.6));
    rollButton->addClickEventListener([this](Ref* sender) {
        playAnimation("run");
    });
    this->addChild(rollButton);
}

void TrainingScene::playAnimation(const std::string& action) {
    // Elimina cualquier acción en curso del personaje
    characterSprite->stopAllActions();

    // Carga los frames de la animación correspondiente
    Vector<SpriteFrame*> frames;
    for (int i = 1; i <= 4; ++i) { // Suponiendo 4 frames por acción
        std::string frameName = "duckee_" + action +  std::to_string(i) + ".png";
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (frame) {
            frames.pushBack(frame);
            cocos2d::log("Frame cargado correctamente: %s", frameName.c_str());
        } else {
            cocos2d::log("Frame no encontrado: %s", frameName.c_str());
        }
    }

    // Crear la animación
    auto animation = Animation::createWithSpriteFrames(frames, 0.2f);
    auto animate = Animate::create(animation);
    //characterSprite->runAction(RepeatForever::create(animate));
    if (action == "jump") {
        
            float originalY = characterSprite->getPositionY();
            auto moveHorizontal = MoveBy::create(1.0f, Vec2(50, 0));
            auto jumpUp = JumpBy::create(1.5f, Vec2(0, 0), 250, 0);  // El pato salta 130 unidades hacia arriba
            
            auto spawn = Spawn::createWithTwoActions(animate, jumpUp);
        
            characterSprite->runAction(spawn);
            characterSprite->runAction(moveHorizontal);
        
            auto moveBack = MoveTo::create(0.1f, Vec2(characterSprite->getPositionX(), originalY));
            characterSprite->runAction(moveBack);
    }else{
        auto moveAction = (action == "run") ?
                              MoveBy::create(0.8f, Vec2(100, 0)) :  // Mueve a la derecha si es 'run'
                              MoveBy::create(0.8f, Vec2(50, 0));  // Mueve menos si es 'roll'

        auto spawn = Spawn::createWithTwoActions(animate, moveAction);

        characterSprite->runAction(spawn);
    }
    
}


void TrainingScene::updateLives(int change) {
    if(change < 0 && shields > 0){
        shields-=1;
    }else{
        lives += change;
    }
    
    lives3->setVisible(lives==3);
    lives2->setVisible(lives==2);
    lives1->setVisible(lives==1);
    

    // game over
    if (lives <= 0) {
        CCLOG("Game Over");
        for (const auto& action : actions) {
            //std::string plistPath = basePath + action + ".plist";
            std::string plistPath =  action + selectedDuck + ".plist";
            SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(plistPath);
        }
        Director::getInstance( )->replaceScene(TransitionFade::create( 0.5, GameMenu::createScene()));
    }
}

bool TrainingScene::onContactBegin(PhysicsContact& contact) {
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();
    
    // Verifica qué nodo es el pato
    Node* duckNode = (nodeA && nodeA->getName() == "duck") ? nodeA : nodeB;
    Node* objectNode = (duckNode == nodeA) ? nodeB : nodeA;
    if (!duckNode || !objectNode) {
            return false;
        }

    if (objectNode->getName() == "mushroom") {
        updateLives(-1); // Disminuye vidas
    } else if (objectNode->getName() == "shield") {
        shields += 0;// Aumenta escudos
    }
    this->removeChild(objectNode);
    objects.erase(std::remove(objects.begin(), objects.end(), objectNode), objects.end());
        
    return true;
}

void TrainingScene::update(float delta) {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    Vec2 position = characterSprite->getPosition();

    if (position.x > origin.x + visibleSize.width) {
        position.x = origin.x;
    }

    // Actualizar pos
    characterSprite->setPosition(position);
    
    static float timeSinceLastSpawn = 0;
    timeSinceLastSpawn += delta;
    if (timeSinceLastSpawn >= 2.0f) { //cada 2s
        timeSinceLastSpawn = 0;

        //  hongo o escudo
        std::string type = (rand() % 2 == 0) ? "mushroom" : "shield";
            
        // Genera una posición aleatoria
        float x = origin.x + rand() % (int)visibleSize.width * 0.7f; //no sobre los botones
        float y = origin.y +(rand() % (int)(visibleSize.height * 0.6f)); // Máximo 60% de la altura
        spawnObject(type, Vec2(x, y));
    }
    
    //Eliminar objetos esquivados
    for (auto it = objects.begin(); it != objects.end();) {
        if ((*it)->getPositionX() < characterSprite->getPositionX() || (*it)->getPositionX() > Director::getInstance()->getVisibleSize().width) {
            this->removeChild(*it);
            it = objects.erase(it);
        } else {
            ++it;
        }
    }
}
void TrainingScene::spawnObject(const std::string& type, const Vec2& position) {
    auto object = Sprite::create(type == "mushroom" ? "mushroom.png" : "shield.png");
    object->setName(type);
        
    if (type == "shield") {
        object->setScale(0.5f);
    }
    auto objectBody = PhysicsBody::createBox(object->getContentSize());
    objectBody->setGravityEnable(false);
    objectBody->setContactTestBitmask(true);
    object->setPhysicsBody(objectBody);
    object->setPosition(position);
    this->addChild(object);

    // Agrega el objeto a una lista para administrarlo después
    objects.push_back(object);
}


void TrainingScene::loadCatAnimations() {
    // Cargar las animaciones de correr y ataque del gato
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("runCat.plist", "runCat.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("attackCat.plist", "attackCat.png");
}

void TrainingScene::spawnCat() {
    // Crear sprite del gato
    auto cat = Sprite::create("runCat.png");
    cat->setPosition(Director::getInstance()->getVisibleSize().width - 20,
                     Director::getInstance()->getVisibleSize().height / 2);  // Posición inicial a la derecha de la pantalla
    cat->setScale(0.5);
    // Cargar los frames de la animación de correr
    Vector<SpriteFrame*> runFrames;
    for (int i = 1; i <= 4; ++i) {  // Suponiendo 4 frames
        auto frameName = "run" + std::to_string(i) + ".png";
        auto frame=(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
        if (frame) {
            runFrames.pushBack(frame);
            cocos2d::log("Frame cargado correctamente: %s", frameName.c_str());
        } else {
            cocos2d::log("Frame no encontrado: %s", frameName.c_str());
        }
    }
    auto runAnimation = Animation::createWithSpriteFrames(runFrames, 0.1f);
    auto runAnimate = Animate::create(runAnimation);
    auto runAction = RepeatForever::create(runAnimate);
    cat->runAction(runAction);

    // Crear el cuerpo físico para detectar colisiones
    auto catBody = PhysicsBody::createBox(cat->getContentSize());
    catBody->setGravityEnable(false);
    catBody->setContactTestBitmask(true);
    cat->setPhysicsBody(catBody);

    this->addChild(cat);

    // Mover el gato hacia la izquierda (hacia el pato)
    auto moveAction = MoveTo::create(5.0f, Vec2(0, cat->getPosition().y));  // Mueve en 5 segundos hasta el borde izquierdo
    cat->runAction(Sequence::create(moveAction, CallFunc::create([=]() {
        // Cuando llegue cerca del pato, cambiar a la animación de ataque
        attackCat(cat);
    }), nullptr));

    // Mantener al gato en la pantalla hasta que pase de largo o ataque
}


void TrainingScene::attackCat(Sprite* cat) {
    // Cargar los frames de la animación de ataque
    Vector<SpriteFrame*> attackFrames;
    for (int i = 1; i <= 4; ++i) {
        attackFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("attack" + std::to_string(i) + ".png"));
    }
    auto attackAnimation = Animation::createWithSpriteFrames(attackFrames, 0.2f);
    auto attackAnimate = Animate::create(attackAnimation);
    cat->runAction(attackAnimate);

    // Detectar si el pato tiene escudos
    if (shields > 0) {
        // El pato tiene escudos, no pierde vidas
        shields--;
        //cat->removeFromParent();  // El gato desaparece
    } else {
        // El pato no tiene escudos, pierde una vida
        updateLives(-1);
        cat->runAction(Sequence::create(attackAnimate, CallFunc::create([=]() {
            // Después de la animación de ataque, el gato sigue corriendo hasta salir de la pantalla
            auto moveAction = MoveTo::create(5.0f, Vec2(Director::getInstance()->getVisibleSize().width + 100, cat->getPosition().y));
            cat->runAction(Sequence::create(moveAction, CallFunc::create([=]() {
                cat->removeFromParent();
            }), nullptr));
        }), nullptr));
    }
}

void TrainingScene::startCatAttackTimer() {
    this->scheduleOnce([this](float delta) {
        spawnCat();  // Llamar a la función para que aparezca el gato
    }, 2.0f, "spawn_cat_key");
}


//
//agregar botones y linkearlos a las acciones
//agregar contador de vidas
//agregar interacción con hongos y vidas
//animaciones de gatos
//texto explicativo para el usuario en training scene
//quitar comentarios y logs
