//
//  CharacterManager.h
//  Proyecto
//
//  Created by Alejandra Esparza Pelcastre
//

#ifndef CharacterManager_h
#define CharacterManager_h

#include "cocos2d.h"
#include <string>
#include <iostream>

class CharacterManager : public cocos2d::Scene
{
public:
    
    static CharacterManager& getInstance();
    void setSelectedCharacter(const std::string& character);
    const std::string& getSelectedCharacter() const;

    
private:
    std::string selectedCharacter;
    
    CharacterManager() = default;
    CharacterManager(const CharacterManager&) = delete;
    CharacterManager& operator=(const CharacterManager&) = delete;

    void loadAnimations();
};



#endif /* CharacterManager_h */

