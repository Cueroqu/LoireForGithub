//
//  Triangle.cpp
//  Loire
//
//  Created by Cuero Zhun Qu on 14-8-1.
//
//

#include "Triangle.h"
#include "Tools.h"

USING_NS_CC;

Triangle::Triangle(){}

void Triangle::set_triangle (cocos2d::Point center, float center_offset, float rotation, bool cocos2d_world) {
    float radian;
    if (cocos2d_world)
        radian = CC_DEGREES_TO_RADIANS(-rotation + 90);
    else
        radian = rotation;
    Point base = Point(cosf(radian), sinf(radian)) * center_offset;
    points[0] = Point(base + center);
    base.rotate(Point::ZERO, CC_DEGREES_TO_RADIANS(120));
    points[1] = Point(base + center);
    base.rotate(Point::ZERO, CC_DEGREES_TO_RADIANS(120));
    points[2] = Point(base + center);
}
