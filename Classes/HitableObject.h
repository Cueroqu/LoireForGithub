//
//  HitableObject.h
//  Loire
//
//  Created by Cuero Zhun Qu on 14-8-6.
//
//

#ifndef __Loire__HitableObject__
#define __Loire__HitableObject__

#include <iostream>
#include "cocos2d.h"
#include "MyBodyParser.h"

class HitableObject : public cocos2d::Sprite {
public:
    virtual void get_hit(Sprite * projectile) {};
    float total_hp;
    float current_hp;
};

#endif /* defined(__Loire__HitableObject__) */
