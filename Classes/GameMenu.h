//
//  GameMenu.h
//  Proyecto
//
#ifndef GameMenu_h
#define GameMenu_h

#include "cocos2d.h"

class GameMenu : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameMenu);
    
private:
    void GoToGameScene(cocos2d::Ref *sender);
    
    void GoToCharacterScene(cocos2d::Ref *sender);
    
    void GoToGameMode(cocos2d::Ref *sender);
};

#endif 
