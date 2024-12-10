//
//  GameScene.cpp
//  Proyecto
//
//  Created by Alejandra Esparza Pelcastre
//

#include "GameScene.h"
#include "GameMenu.h"
#include "CharacterManager.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool GameScene::init() {
    if (!Scene::init()) {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // Fondo Parallax
    auto parallaxNode = ParallaxNode::create();
    parallaxNode->setPosition(Vec2::ZERO);
    this->addChild(parallaxNode, -1);

    // Tamaño y centro de la pantalla
    Vec2 center(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);

    // Crear fondos
    auto background1 = Sprite::create("fondoNubes.png");
    auto background2 = Sprite::create("middleLayer.png");
    auto background3 = Sprite::create("topLayer.png");
    
    /*
     background1->setScale(visibleSize.width / background1->getContentSize().width,
                               visibleSize.height / background1->getContentSize().height);
     background2->setScale(visibleSize.width / background2->getContentSize().width,
                               visibleSize.height / background2->getContentSize().height);
     background3->setScale(visibleSize.width / background3->getContentSize().width,
                               visibleSize.height / background3->getContentSize().height);
     */
    
   
    // Ajustar punto de anclaje al centro
    background1->setAnchorPoint(Vec2(0.5, 0.5));
    background2->setAnchorPoint(Vec2(0.5, 0.5));
    background3->setAnchorPoint(Vec2(0.5, 0.5));

    // Agregar los fondos al nodo Parallax, centrados en la pantalla
    parallaxNode->addChild(background1, 1, Vec2(0.3f, 0.0f), center);
    parallaxNode->addChild(background2, 1, Vec2(0.6f, 0.0f), center);
    parallaxNode->addChild(background3, 1, Vec2(0.9f, 0.0f), center);

   
    this->parallaxNode = parallaxNode;
    
    lives = 3;
    shields=0;
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
    Vector<MenuItem*> MenuItems;
    auto homeItem = MenuItemImage::create("home.png", "homeSelected.png",
                                          CC_CALLBACK_1(GameScene::GoToMenu, this));
    homeItem->setPosition(Point(origin.x + 40, origin.y + 190));
    
    MenuItems.pushBack(homeItem);
    auto menu = Menu::createWithArray(MenuItems);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);
    
    startCatAttackTimer();
    this->scheduleUpdate();
    
    
    return true;
}

void GameScene::update(float delta) {
    
    // Velocidad del movimiento del fondo
    float backgroundSpeed = 100.0f; // pixeles por segundo

    // Desplaza fondo hacia la izquierda
    parallaxNode->setPositionX(parallaxNode->getPositionX() - backgroundSpeed * delta);

    
    auto position = parallaxNode->getPositionX();
    if (position < -Director::getInstance()->getVisibleSize().width) {
        parallaxNode->setPositionX(0);
    }
}


void GameScene::GoToMenu(cocos2d::Ref* sender){
    //clean loaded animations
    for (const auto& action : actions) {
        //std::string plistPath = basePath + action + ".plist";
        std::string plistPath =  action + selectedDuck + ".plist";
        SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(plistPath);
    }
    Director::getInstance( )->replaceScene(TransitionFade::create( 0.5, GameMenu::createScene()));
}
void GameScene::initDuckAnimations() {
    
    selectedDuck = CharacterManager::getInstance().getSelectedCharacter();
    
    // Define the base path to access the actions
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
                                          Director::getInstance()->getVisibleSize().height * 0.38));
    if (!characterSprite) {
        cocos2d::log("Error: No se pudo crear el sprite con el frame inicial.");
        return;
    }
    this->addChild(characterSprite);

}

//es necesaria?
void GameScene::createAnimationForAction(const std::string& action, const Vec2& position) {
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



void GameScene::createActionButtons() {
    // Tamaño de la pantalla
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // Crear botón para avanzar
    //auto moveButton = ui::Button::create("res/action_buttons/izquierda_btn.png");
    auto moveButton = ui::Button::create("izquierda_btn.png");
    moveButton->setPosition(Vec2(visibleSize.width * 0.8, visibleSize.height * 0.6));
    moveButton->addClickEventListener([this](Ref* sender) {
        playAnimation("roll");
    });
    /*
    moveButton->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type) {
            if (type == ui::Widget::TouchEventType::BEGAN) {
                playAnimation("run");
            } else if (type == ui::Widget::TouchEventType::ENDED) {
                isButtonPressed = false; // Sin interacción, volver a idle
            }
    });
    */
    this->addChild(moveButton);

    // Crear botón para saltar
    //auto jumpButton = ui::Button::create("res/action_buttons/saltar_btn.png");
    auto jumpButton = ui::Button::create("saltar_btn.png");
    jumpButton->setPosition(Vec2(visibleSize.width * 0.86, visibleSize.height * 0.75));
    jumpButton->addClickEventListener([this](Ref* sender) {
        playAnimation("jump");
    });
    
    /*
    jumpButton->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type) {
            if (type == ui::Widget::TouchEventType::BEGAN) {
                playAnimation("jump");
            } else if (type == ui::Widget::TouchEventType::ENDED) {
                isButtonPressed = false;
            }
    });
    */
    this->addChild(jumpButton);

    // Crear botón para rodar
    //auto rollButton = ui::Button::create("res/buttons/derecha_btn.png");
    auto rollButton = ui::Button::create("derecha_btn.png");
    rollButton->setPosition(Vec2(visibleSize.width * 0.92, visibleSize.height * 0.6));
    rollButton->addClickEventListener([this](Ref* sender) {
        playAnimation("run");
    });
    /*
    rollButton->addTouchEventListener([this](Ref* sender, ui::Widget::TouchEventType type) {
            if (type == ui::Widget::TouchEventType::BEGAN) {
                playAnimation("roll");
            } else if (type == ui::Widget::TouchEventType::ENDED) {
                isButtonPressed = false;
            }
    });
     */
    this->addChild(rollButton);
}

void GameScene::playAnimation(const std::string& action) {
    //isButtonPressed = (action != "idle");
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
    characterSprite->runAction(RepeatForever::create(animate));
    
    
    
    if (action == "run" || action == "roll") {
            schedule([this](float delta) {
                    float speed = 100.0f; // Velocidad del fondo
                    parallaxNode->setPositionX(parallaxNode->getPositionX() - speed * delta);
                    
                    // Ciclar el fondo
                    if (parallaxNode->getPositionX() < -Director::getInstance()->getVisibleSize().width) {
                        parallaxNode->setPositionX(0);
                    }

                // Mueve el personaje hacia adelante
                //characterSprite->setPositionX(characterSprite->getPositionX() + speed * delta);
            }, 0.16f, "move_background"); //0.016
    } else if (action == "jump") {
        unschedule("move_background"); // Detener movimiento continuo
        auto jump = JumpBy::create(1.0f, Vec2(0, 0), 150, 1); // Solo animación sin desplazamiento horizontal
        characterSprite->runAction(jump);
    } else {
        unschedule("move_background");
    }
}

void GameScene::loadCatAnimations() {
    // Cargar las animaciones de correr y ataque del gato
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("runCat.plist", "runCat.png");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("attackCat.plist", "attackCat.png");
}

void GameScene::spawnCat() {
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

void GameScene::updateLives(int change) {
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
    }
}

void GameScene::attackCat(Sprite* cat) {
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

void GameScene::startCatAttackTimer() {
    this->scheduleOnce([this](float delta) {
        spawnCat();  // Llamar a la función para que aparezca el gato
    }, 2.0f, "spawn_cat_key");
}
