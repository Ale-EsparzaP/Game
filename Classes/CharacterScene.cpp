//
//  CharacterScene.cpp
//  Proyecto
//
//  Created by Alejandra Esparza Pelcastre
//

#include "CharacterScene.h"
#include "GameMenu.h"
#include "CharacterManager.h"

USING_NS_CC;

Scene* CharacterScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = CharacterScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool CharacterScene::init() {
    if (!Scene::init()) {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //background
    auto backgroundSprite = Sprite::create("fondoCharacters.png");
    backgroundSprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(backgroundSprite);
    
    // Load sprite sheet and .plist
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("idleBigBlue.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("idleBigPurple.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("idleBigYellow.plist");
    
    
        
    //available characters
    createCharacterAnimations();
    currentCharacterIndex = 0;
    characters[currentCharacterIndex]->setVisible(true);
    
    
    //buttons
    Vector<MenuItem*> OptionItems;
    auto homeItem = MenuItemImage::create("home.png", "homeSelected.png",
                                          CC_CALLBACK_1(CharacterScene::GoToMenu, this));
    auto leftItem = MenuItemImage::create("left.png", "leftSelected.png",
                                          CC_CALLBACK_1(CharacterScene::ShowPreviousCharacter, this));
    auto okItem = MenuItemImage::create("ok.png", "okSelected.png",
                                        CC_CALLBACK_1(CharacterScene::SelectCharacter, this));
    
    auto rightItem = MenuItemImage::create("right.png", "rightSelected.png",
                                          CC_CALLBACK_1(CharacterScene::ShowNextCharacter, this));
    
    homeItem->setPosition(Point(origin.x + 40, origin.y + 190));
    
    leftItem->setPosition(Point(visibleSize.width/2 + origin.x - 100, visibleSize.height/3 + origin.y - 50));
    
    okItem->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/3 + origin.y - 50));
    
    rightItem->setPosition(Point(visibleSize.width/2 + origin.x + 100, visibleSize.height/3 + origin.y - 50));
    
    
    OptionItems.pushBack(homeItem);
    OptionItems.pushBack(leftItem);
    OptionItems.pushBack(okItem);
    OptionItems.pushBack(rightItem);
    
    auto menu = Menu::createWithArray(OptionItems);
    
    //this->addChild(background);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);
    
    return true;
}

void CharacterScene::createCharacterAnimations() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // Character animation frame prefixes
    std::vector<std::string> characterPrefixes = {"yellow", "blue", "purple"};
    
    for (const auto& prefix : characterPrefixes) {
        auto animation = Animation::create();
        for (int i = 1; i <= 4; ++i) { // each character has 4 frames
            std::string frameName = StringUtils::format("%s%d.png", prefix.c_str(), i);
            auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
            animation->addSpriteFrame(frame);
        }
        animation->setDelayPerUnit(0.4f); //  frame speed
        auto animate = Animate::create(animation);
        
        auto characterSprite = Sprite::createWithSpriteFrameName(prefix + "1.png");
        
        //scale, position, visibility
        characterSprite->setScale(3.9f);
        characterSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
        characterSprite->setVisible(false);
        
        //run animation while is visible HOW are garantizando eso?
        characterSprite->runAction(RepeatForever::create(animate));
        this->addChild(characterSprite);
        characters.push_back(characterSprite);
    }
}

void CharacterScene::GoToMenu(cocos2d::Ref* sender){
    //auto scene = GameScene::createScene();
   Director::getInstance( )->replaceScene(TransitionFade::create( 0.5, GameMenu::createScene()));
}

void CharacterScene::ShowNextCharacter(cocos2d::Ref* sender) {
    characters[currentCharacterIndex]->setVisible(false);
    currentCharacterIndex = (currentCharacterIndex + 1) % characters.size();
    characters[currentCharacterIndex]->setVisible(true);
}

void CharacterScene::ShowPreviousCharacter(cocos2d::Ref* sender) {
    characters[currentCharacterIndex]->setVisible(false);
    currentCharacterIndex = (currentCharacterIndex - 1 + characters.size()) % characters.size();
    characters[currentCharacterIndex]->setVisible(true);
}

void CharacterScene::SelectCharacter(cocos2d::Ref* sender) {
    selectedCharacterIndex = currentCharacterIndex; 
    if(selectedCharacterIndex == 0) character = "Yellow";
    
    if(selectedCharacterIndex == 1) character = "Blue";
    
    if(selectedCharacterIndex == 2) character = "Purple";
    
    CharacterManager::getInstance().setSelectedCharacter(character);
        
    log("Character %d selected", selectedCharacterIndex);
    GoToMenu(sender);
}
