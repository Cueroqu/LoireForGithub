//
//  Airship.h
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-14.
//
//

#ifndef __Loire__Airship__
#define __Loire__Airship__

#include <iostream>
#include <vector>
#include "cocos2d.h"
#include "Path.h"
#include "Weapon.h"
#include "Boom.h"
#include "ShipBase.h"

#define GET_AIRSHIP(shape) dynamic_cast<Airship *>(shape)

class Airship : public ShipBase
{
public:
    static Airship* create(const std::string &filename);

    int commandIndex;
    cocos2d::Point touch_offset;
    
    virtual void releaseElements() override;
    virtual void movement(float t) override;
    virtual void update(float t) override;
    virtual void get_hit(cocos2d::Sprite * projectile) override;
    void set_percent(int value);
//    void shoot_enemy(float t);
    void reset_for_game();
    void move_others(cocos2d::Point target_direction);
    void move_elements(cocos2d::Point target_direction);
    void move_type_1(float t);
    void move_type_2(float t);
    void set_rotation_type_1(float t, float cocos_angle, float origin);
    bool next_dot_is_closer();
};

#endif /* defined(__Loire__Airship__) */
