//
//  GameScene.h
//  Proyecto
//
//  Created by Alejandra Esparza Pelcastre 
//

#ifndef GameScene_h
#define GameScene_h

#include "cocos2d.h"

class GameScene : public cocos2d::Scene
{
public:
    
    static cocos2d::Scene* createScene();

    
    virtual bool init() override;
    
    
    virtual void update(float delta) override;
    
    CREATE_FUNC(GameScene);
private:
    void GoToMenu(cocos2d::Ref *sender);
    void initDuckAnimations();
    std::string selectedDuck;
    std::string basePath;
    std::vector<std::string> actions = {
        "idle", "run", "jump", "crouch", "roll", "land", "damage"
    };
    
    cocos2d::ParallaxNode* parallaxNode;
    cocos2d::Sprite* characterSprite;
    bool isButtonPressed = false;
    void createAnimationForAction(const std::string& action, const cocos2d::Vec2& position);
    void createActionButtons();
    void playAnimation(const std::string& action);
    
    void updateLives(int change);
    
    int lives;
    int shields;
    cocos2d::Sprite* lives3;
    cocos2d::Sprite* lives2;
    cocos2d::Sprite* lives1;
    
    void startCatAttackTimer();
    void loadCatAnimations();
    void attackCat(cocos2d::Sprite* cat);
    void spawnCat();
};



#endif /* GameScene_h */
