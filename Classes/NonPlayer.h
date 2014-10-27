//
//  NonPlayer.h
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-23.
//
//

#ifndef __Loire__NonPlayer__
#define __Loire__NonPlayer__

#include <iostream>
#include "cocos2d.h"
#include <string>
#include <map>

class NonPlayer {
public:
    static void load_npcs();
    static std::map<std::string, NonPlayer> non_players;
    
    NonPlayer(){};
    NonPlayer(std::string _name, cocos2d::Sprite * _image);
    NonPlayer(std::string _name, const char* filename);
    std::string name;
    cocos2d::Sprite* image;

};

#endif /* defined(__Loire__NonPlayer__) */
