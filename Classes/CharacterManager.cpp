//
//  CharacterManager.cpp
//  Proyecto
//
//  Created by Alejandra Esparza Pelcastre 
//


#include "CharacterManager.h"

// Static method implementation
CharacterManager& CharacterManager::getInstance() {
    static CharacterManager instance;
    return instance;
}

void CharacterManager::setSelectedCharacter(const std::string& character) {
    selectedCharacter = character;
    loadAnimations();
}

const std::string& CharacterManager::getSelectedCharacter() const {
    return selectedCharacter;
}

void CharacterManager::loadAnimations() {
    std::cout << "Loading animations for " << selectedCharacter << std::endl; //actually load animations
}

