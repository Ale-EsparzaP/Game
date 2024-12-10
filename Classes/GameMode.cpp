//
//  GameModeScene.cpp
//  Proyecto
//
//  Created by Alejandra Esparza Pelcastre on 01/12/24.
//

#include "GameMode.h"
#include "GameMenu.h"
#include "GameScene.h"
#include "TrainingScene.h"

USING_NS_CC;

Scene* GameMode::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameMode::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool GameMode::init() {
    if (!Scene::init()) {
        return false;
    }
    
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    /***/
    auto backgroundSprite = Sprite::create("fondoSimple.png");
    backgroundSprite->setScale(0.9f);
    backgroundSprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(backgroundSprite);
    
    Vector<MenuItem*> MenuItems;
    
    auto homeItem = MenuItemImage::create("home.png", "homeSelected.png",
                                          CC_CALLBACK_1(GameMode::GoToMenu, this));
    auto trainingItem = MenuItemImage::create("training.png", "trainingSelected.png",
                                          CC_CALLBACK_1(GameMode::GoToTrainingScene, this));
    
    auto classicalItem = MenuItemImage::create("classical.png", "classicalSelected.png",
                                          CC_CALLBACK_1(GameMode::GoToGameScene, this));
    auto beAcatItem = MenuItemImage::create("beAcat_btn.png", "beAcatSelected.png",
                                          CC_CALLBACK_1(GameMode::GoToGameScene, this));
    
    
    
    homeItem->setPosition(Point(origin.x + 40, origin.y + 190));
    trainingItem->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.width/2 + origin.y - 70));
    classicalItem->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.width/2 + origin.y - 140) );
    beAcatItem->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.width/2 + origin.y - 210));
    
    MenuItems.pushBack(homeItem);
    MenuItems.pushBack(trainingItem);
    MenuItems.pushBack(classicalItem);
    MenuItems.pushBack(beAcatItem);
    
    
    auto menu = Menu::createWithArray(MenuItems);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);
    return true;
}

void GameMode::GoToGameScene(cocos2d::Ref* sender){
   Director::getInstance( )->replaceScene(TransitionFade::create( 0.5, GameScene::createScene()));
}
void GameMode::GoToTrainingScene(cocos2d::Ref* sender){
   Director::getInstance( )->replaceScene(TransitionFade::create( 0.5, TrainingScene::createScene()));
}
void GameMode::GoToMenu(cocos2d::Ref* sender){
   Director::getInstance( )->replaceScene(TransitionFade::create( 0.5, GameMenu::createScene()));
}
