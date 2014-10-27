//
//  Trackings.h
//  Loire
//
//  Created by Cuero Zhun Qu on 14-8-12.
//
//

#ifndef __Loire__Trackings__
#define __Loire__Trackings__

#include <iostream>
#include "cocos2d.h"
#include "Projectile.h"
#include "Tools.h"

#define GET_TRACKINGS(shape) dynamic_cast<Trackings *>(shape)

class Trackings : public Projectile {
public:
    float current_angular_speed;
    static Trackings * createWithTexture(cocos2d::Texture2D * texture);
    cocos2d::Sprite * target;
    virtual void update(float t) override;
    inline float get_rotate_speed() {
        return this->data.rotate;
    }
    inline float get_rotate_acceleration() {
        return this->data.angular_acceleratoin;
    }
};

#endif /* defined(__Loire__Trackings__) */
