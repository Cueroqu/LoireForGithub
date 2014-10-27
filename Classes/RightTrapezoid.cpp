//
//  Trapezoid.cpp
//  Loire
//
//  Created by Cuero Zhun Qu on 14-8-11.
//
//

#include "RightTrapezoid.h"

RightTrapezoid::RightTrapezoid() {
}

void RightTrapezoid::set_trapezoid(cocos2d::Point left_center, float width, float height) {
    float hheight = height / 2;
    points[0].x = points[3].x = left_center.x;
    points[2].x = points[1].x = left_center.x + width;
//    points[2].x = MAX(points[1].x - height, left_center.x);
    points[1].y = points[0].y = left_center.y + hheight;
    points[2].y = points[3].y = left_center.y - hheight;
}
