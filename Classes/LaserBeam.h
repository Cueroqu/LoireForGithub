//
//  LaserBeam.h
//  Loire
//
//  Created by Cuero Zhun Qu on 14-8-20.
//
//

#ifndef __Loire__LaserBeam__
#define __Loire__LaserBeam__

#include <iostream>
#include <vector>
#include <time.h>
#include "Projectile.h"
#include "cocos2d.h"

#define GET_LASERBEAM(shape) dynamic_cast<LaserBeam *>(shape)

class Weapon;

class LaserBeam : public Projectile {
public:
    static LaserBeam * createWithTexture(cocos2d::Texture2D * texture);
    ~LaserBeam() {
    }
    cocos2d::Sprite * target;
    virtual void update(float t) override;
    void unschedule_update();
    void unschedule_update_without_removeself();
    void set_elements_visble(bool visible);
    void remove_elements();
//    void reset();
    std::vector<Sprite *> beams;
    bool working;
    float max_distance;
    Weapon * owner_weapon;
    time_t start_time;
};

#endif /* defined(__Loire__LaserBeam__) */
