//
//  StupidEnemy.h
//  Loire
//
//  Created by Cuero Zhun Qu on 14-8-13.
//
//

#ifndef __Loire__StupidEnemy__
#define __Loire__StupidEnemy__

#include <iostream>
#include "cocos2d.h"
#include "Weapon.h"
#include "ShipBase.h"

class StupidEnemy : public ShipBase {
public:
    static StupidEnemy * create();
    float vertical_direction;
    float horizontal_direction;
    float x_offset;
    float y_offset;
    float current_offset;
    Weapon * abuser;
    float abuser_linear_range;
    float abuser_angle_range;
    cocos2d::Point abuser_origin_position;
    cocos2d::Point stupid_center;
    StupidEnemy() {};
    StupidEnemy(Weapon * w) : abuser(w){};
    void reset_direction_and_bound(Weapon * w);
    virtual void update(float t) override;
};

#endif /* defined(__Loire__StupidEnemy__) */
