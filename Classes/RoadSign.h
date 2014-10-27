//
//  RoadSign.h
//  Loire
//
//  Created by Cuero Zhun Qu on 14-8-25.
//
//

#ifndef __Loire__RoadSign__
#define __Loire__RoadSign__

#include <iostream>
#include "cocos2d.h"

#define GET_ROADSIGN(shape) dynamic_cast<RoadSign *>(shape)

class RoadSign : public cocos2d::Sprite {
public:
    static constexpr float RADIUS = 20;
    CREATE_FUNC(RoadSign);
    RoadSign() : passed(false) {};
    void draw_self();
    bool passed;
};

#endif /* defined(__Loire__RoadSign__) */
