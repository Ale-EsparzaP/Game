//
//  CharacterScene.h
//  Proyecto
//
//  Created by Alejandra Esparza Pelcastre on 27/11/24.
//

#ifndef CharacterScene_h
#define CharacterScene_h

#include "cocos2d.h"

class CharacterScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    
    virtual bool init();
    
    
    CREATE_FUNC(CharacterScene);
private:
    void GoToMenu(cocos2d::Ref *sender);
    void createCharacterAnimations();
    void ShowNextCharacter(cocos2d::Ref *sender);
    void ShowPreviousCharacter(cocos2d::Ref *sender);
    void SelectCharacter(cocos2d::Ref *sender);

    std::vector<cocos2d::Sprite*> characters;
    std::string character;
    std::string selectedCharacter;
    int currentCharacterIndex;
    int selectedCharacterIndex;
};

#endif /* CharacterScene_h */

