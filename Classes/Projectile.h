//
//  Projectile.h
//  Loire
//
//  Created by Cuero Zhun Qu on 14-7-18.
//
//

#ifndef __Loire__Projectile__
#define __Loire__Projectile__

#include <iostream>
#include "cocos2d.h"
#include "Arsenal.h"
#include "ShipBase.h"

class Projectile : public cocos2d::Sprite {
public:
    static Projectile* create();
    static Projectile* createWithTexture(cocos2d::Texture2D * texture) ;

    Projectile();
    ~Projectile();
    
    float get_power();
    bool should_damage_enemy(ShipBase *ship);
    virtual void update(float t) override;
    inline float get_move_speed() {
        return this->data.speed;
    }
    inline float get_linear_acceleration() {
        return this->data.linear_acceleration;
    }
    inline cocos2d::Point get_proj_direction() {
        float pda_in_radians = CC_DEGREES_TO_RADIANS(-Tools::modifyDegreeIntoRange(this->getRotation()));
        return cocos2d::Point(cosf(pda_in_radians), sinf(pda_in_radians));
    }
    CC_SYNTHESIZE(ShipBase *, _owner, Owner);
    CC_SYNTHESIZE_READONLY(cocos2d::Vector<ShipBase *>, _enemiesHit, EnemiesHit);
    ProjectileData data;
    float current_distance;
    float total_distance;
    cocos2d::Point current_linear_speed;
};
#endif /* defined(__Loire__Projectile__) */
