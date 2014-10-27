//
//  Player.h
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-16.
//
//

#ifndef __Loire__Player__
#define __Loire__Player__

#include <vector>
#include "cocos2d.h"
#include "Airship.h"
#include "pugixml/pugixml.hpp"

class Weapon;

class Player {
private:
    Player();
    Player(Player const&);
    void operator=(Player const&);
public:
    static const int PLAYER_SHIP_TAG = 1;
    
    static Player& getInstance() {
        static Player _player;
        return _player;
    }
    static void Release() {
        Player& _player = Player::getInstance();
        _player.selected_airship = NULL;
        for (auto ship : _player.airships) {
            ship->releaseElements();
            ship->release();
        }
        _player.airships.clear();
    }
    cocos2d::Vector<Airship *> airships;
    std::vector<Weapon *> idleweapons;
    Airship* selected_airship;
    cocos2d::Sprite* person;
    void load_idle_weapons(pugi::xml_node node);
};

#endif /* defined(__Loire__Player__) */
