//
//  GameMode.h
//  Proyecto
//
//  Created by Alejandra Esparza Pelcastre
//

#ifndef GameMode_h
#define GameMode_h

#include "cocos2d.h"

class GameMode : public cocos2d::Scene
{
public:
    
    static cocos2d::Scene* createScene();

    
    virtual bool init();
    
    
    CREATE_FUNC(GameMode);
    
private:
    void GoToGameScene(cocos2d::Ref *sender);
    void GoToMenu(cocos2d::Ref *sender);
    void GoToTrainingScene(cocos2d::Ref *sender);
};
/**
 class GameScene: public cocos2d::Scene
 {
 public:
     static cocos2d::Scene* createScene();
     
     virtual bool init();
     CREATE_FUNC(GameScene);
 };
 
 */


#endif /* GameMode_h */

