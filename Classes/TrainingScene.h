//
//  TrainingScene.h
//  Proyecto
//
//  Created by Alejandra Esparza Pelcastre
//

#ifndef TrainingScene_h
#define TrainingScene_h

#include "cocos2d.h"

class TrainingScene : public cocos2d::Scene
{
public:
    
    static cocos2d::Scene* createScene();

    
    virtual bool init() override;
    
    virtual void update(float delta) override;
    
    CREATE_FUNC(TrainingScene);
    
private:
    cocos2d::Sprite* characterSprite;  //cuidado
    void GoToGameScene(cocos2d::Ref *sender);
    void GoToMenu(cocos2d::Ref *sender);
    void initDuckAnimations();
    std::string selectedDuck;
    std::string basePath;
    std::vector<std::string> actions = {
        "idle", "run", "jump", "crouch", "roll", "land", "damage"
    };
    
    void createAnimationForAction(const std::string& action, const cocos2d::Vec2& position);
    void createActionButtons();
    void playAnimation(const std::string& action);
    void updateLives(int change);
    
    int lives;
    int shields;
    cocos2d::Sprite* lives3;
    cocos2d::Sprite* lives2;
    cocos2d::Sprite* lives1;
    
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    void spawnObject(const std::string& type, const cocos2d::Vec2& position);
    
    std::vector<cocos2d::Sprite*> objects;
    
    void startCatAttackTimer();
    void loadCatAnimations();
    void attackCat(cocos2d::Sprite* cat);
    void spawnCat();
};

#endif /* TrainingScene_h */

