//
//  GameMenu.cpp
//  Proyecto
//
//  Created by Alejandra Esparza Pelcastre
//

#include "GameMenu.h"
#include "GameScene.h"
#include "GameMode.h"
#include "CharacterScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene* GameMenu::createScene()
{
    
    auto scene = Scene::create();
    
    auto layer = GameMenu::create();

    scene->addChild(layer);
    
    return scene;
}
/**
 Scene* GameMenu::createScene() {
     return GameMenu::create();
 }
 */

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in Scene.cpp\n");
}

bool GameMenu::init() {
    if (!Layer::init()) {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto backgroundSprite = Sprite::create("fondoHome.png");
    backgroundSprite->setScale(0.9f);
    backgroundSprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(backgroundSprite);
    
    /**
     auto* audio_engine = CocosDenshion::SimpleAudioEngine::getInstance();
         if (!audio_engine->isBackgroundMusicPlaying()) {
             std::string music_file = "background.mp3";
             audio_engine->preloadBackgroundMusic(music_file.c_str());
             audio_engine->playBackgroundMusic(music_file.c_str(),true);
             audio_engine->setBackgroundMusicVolume(0.3f);
         }
     */
    
    
    auto audio =  SimpleAudioEngine::getInstance();
    // continuously play it
    audio->playBackgroundMusic("background.wav", true);
    

    //background->setScale(2.0f);
    //background->setContentSize(Size(background->getContentSize().width*2, background->getContentSize().height*2));
    
    Vector<MenuItem*> MenuItems;
    
    auto playItem = MenuItemImage::create("play.png", "botonMaderaSelected.png",
                                          CC_CALLBACK_1(GameMenu::GoToGameScene, this));
    
    auto exitItem = MenuItemImage::create("exit.png", "botonMaderaSelected.png",
                                          CC_CALLBACK_1(GameMenu::menuCloseCallback, this));
    
    auto modeItem = MenuItemImage::create("gameMode.png", "botonMaderaSelected.png",
                                          CC_CALLBACK_1(GameMenu::GoToGameMode, this));
    
    auto characterItem = MenuItemImage::create("character.png", "botonMaderaSelected.png",
                                               CC_CALLBACK_1(GameMenu::GoToCharacterScene, this));
    
    modeItem->setScale(0.7f);
    modeItem->setPosition(Point(visibleSize.width/2 + origin.x + 150, visibleSize.height/3 + origin.y));
    //playItem->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/3 + origin.y));
    
    playItem->setScale(0.7f);
    playItem->setPosition(Point(visibleSize.width/2 + origin.x + 50, visibleSize.height/3 + origin.y));
    
    exitItem->setScale(0.7f);
    exitItem->setPosition(Point(visibleSize.width/2 + origin.x - 150, visibleSize.height/3 + origin.y));
    
    characterItem->setScale(0.7f);
    characterItem->setPosition(Point(visibleSize.width/2 + origin.x - 50, visibleSize.height/3 + origin.y));
    
    
    MenuItems.pushBack(modeItem);
    MenuItems.pushBack(playItem);
    MenuItems.pushBack(exitItem);
    MenuItems.pushBack(characterItem);
    
    
    auto menu = Menu::createWithArray(MenuItems);
    
    //this->addChild(background);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);
    
    
    return true;
}

void GameMenu::GoToGameScene(cocos2d::Ref* sender){
   Director::getInstance( )->replaceScene(TransitionFade::create( 0.5, GameScene::createScene()));
}

void GameMenu::GoToCharacterScene(cocos2d::Ref* sender){
   Director::getInstance( )->replaceScene(TransitionFade::create( 0.5, CharacterScene::createScene()));
}

void GameMenu::GoToGameMode(cocos2d::Ref* sender){
   Director::getInstance( )->replaceScene(TransitionFade::create( 0.5, GameMode::createScene()));
}

void GameMenu::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
   Director::getInstance()->end();

}
