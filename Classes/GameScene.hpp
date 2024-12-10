//
//  GameScene.hpp
//  Proyecto
//
//  Created by Alejandra Esparza Pelcastre 
//

#ifndef GameScene_hpp
#define GameScene_hpp

#include "GameScene.h"

USING_NS_CC;

Scene* GameScene::createScene(){
    auto scene = Scene::create();
    
    auto layer = GameScene::create();
    
    return scene;
}

#endif /* GameScene_hpp */
