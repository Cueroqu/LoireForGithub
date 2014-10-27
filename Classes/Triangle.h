//
//  Triangle.h
//  Loire
//
//  Created by Cuero Zhun Qu on 14-8-1.
//
//

#ifndef __Loire__Triangle__
#define __Loire__Triangle__

#include <iostream>
#include "cocos2d.h"
#include "math.h"

class Triangle {
public:
    /* degree has same orientation with those in cocos2dx */
    Triangle();
    /* if cocos2d_world, the rotation is got by getRotation(), the unit is degree, clockwise dir is positive
     * if not, the rotation is got by Point::getAngle(), the unit is radian, counter-clockwise dir is positive
     */
    void set_triangle(cocos2d::Point center, float center_offset, float rotation, bool cocos2d_world = true);
    cocos2d::Point points[3];
};

#endif /* defined(__Loire__Triangle__) */
