//
//  Enemy.h
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-16.
//
//

#ifndef __Loire__Enemy__
#define __Loire__Enemy__

#include <iostream>
#include <vector>
#include "ShipBase.h"
#include "pugixml.hpp"
#include "Boom.h"
#include "Trackings.h"

#define GET_ENEMY(shape) dynamic_cast<Enemy *>(shape)

class Enemy : public ShipBase {
private:
    static const int PATHNUM = 50;
    float step_distance;
    cocos2d::Point path[PATHNUM];
public:
    static Enemy* create(pugi::xml_node node);
    std::vector<cocos2d::Point> move_delta_pool;
    virtual void update(float t) override;
    virtual void movement(float t) override;
    void move_type_1(float t);
    void move_type_2(float t);
    void rotate_type_1(float t, float cocos_angle, float origin);
    void shoot_projectiles();
    void update_position();
    void remove_self_from_game();
    virtual void get_hit(cocos2d::Sprite * projectile) override;
    
    ~Enemy();
    
    int move_type;
};

#endif /* defined(__Loire__Enemy__) */
