//
//  Boom.h
//  Loire
//
//  Created by Cuero Zhun Qu on 14-8-5.
//
//

#ifndef __Loire__Boom__
#define __Loire__Boom__

#include <iostream>
#include "cocos2d.h"

class Boom {
public:
    constexpr static float BOOM_TIME = 0.05f;
    constexpr static float BOOM_SIZE = 10.0f;
    Boom(){};
    Boom(cocos2d::Point _p);
    void draw_self();
    std::clock_t trigger_time;
    cocos2d::Point position;
};

#endif /* defined(__Loire__Boom__) */
