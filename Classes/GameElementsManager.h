//
//  GameElementsManager.h
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-28.
//
//

#ifndef __Loire__GameElementsManager__
#define __Loire__GameElementsManager__

#include <iostream>
#include <map>
#include <deque>
#include "cocos2d.h"
#include "Boom.h"

class Enemy;
class Airship;

#define GAME_ELEMENT_ZINDEX 10
#define GAME_CONTROL_BACKGROUND_ZINDEX 15
#define GAME_CONTROL_ZINDEX 20

class GameElementsManager {
public:
    static cocos2d::Vector<cocos2d::Sprite *> enemies;
    static std::map<cocos2d::Sprite*, cocos2d::Label*> enemy_label_map;
    static cocos2d::Vector<cocos2d::Sprite *> signs;
    static int passed_sign_nums;
    static std::deque<Boom> booms;
    static void copy_player(Airship * ship);
};

#endif /* defined(__Loire__GameElementsManager__) */
